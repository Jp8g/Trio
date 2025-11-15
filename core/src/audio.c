#include "../include/audio.h"
#include "log.h"
#include <pthread.h>

void writeCallback(struct SoundIoOutStream* soundioOutStream, int frameCountMin, int frameCountMax) {
    const struct SoundIoChannelLayout *layout = &soundioOutStream->layout;
    struct SoundIoChannelArea *areas;

    float secondsPerFrame = 1.0f / soundioOutStream->sample_rate;
    int framesLeft = frameCountMax;

    while (framesLeft > 0) {
        int frameCount = framesLeft;

        int err = soundio_outstream_begin_write(soundioOutStream, &areas, &frameCount);

        if (err) {
            TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to begin writing to sound OutStream: %s", soundio_strerror(err));
            return;
        }

        if (!frameCount) break;

        for (int frame = 0; frame < frameCount; frame += 1) {
            for (int channel = 0; channel < layout->channel_count; channel++) {
                *(float*)(areas[channel].ptr + areas[channel].step * frame) = 0;
            }
        }

        TrioMixer* mixer = &((TrioAudioDevice*)soundioOutStream->userdata)->mixer;

        for (uint32_t streamIndex = 0; streamIndex < mixer->count; streamIndex++) {

            TrioAudioStream* audioStream = mixer->streams[streamIndex];

            if (!audioStream->playing) continue;

            double streamPosition = audioStream->pos;
            uint32_t streamFrameCount = audioStream->buffer->frameCount;

            double streamPositionStep = (double)audioStream->buffer->sampleRate / soundioOutStream->sample_rate;

            for (int32_t frame = 0; frame < frameCount; frame++) {

                if (streamPosition >= streamFrameCount) break;

                uint32_t streamPositionFloor = (uint32_t)streamPosition;
                if (streamPositionFloor >= audioStream->buffer->frameCount - 1)
                    streamPositionFloor = audioStream->buffer->frameCount - 1;
                
                double weightToNextSample = streamPosition - (double)streamPositionFloor;

                for (int32_t channel = 0; channel < layout->channel_count; channel++) {
                    uint32_t dataIndex = streamPositionFloor;

                    if (audioStream->buffer->channels != 1) dataIndex = streamPositionFloor * audioStream->buffer->channels + channel;

                    uint32_t nextFramePosition = streamPositionFloor + 1;

                    if (nextFramePosition >= streamFrameCount) nextFramePosition = streamFrameCount - 1;

                    uint32_t nextSampleIndex = nextFramePosition * audioStream->buffer->channels + channel;

                    float interpolatedSample = (audioStream->buffer->data[dataIndex] * (1 - weightToNextSample)) + (audioStream->buffer->data[nextSampleIndex] * weightToNextSample);

                    float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                    
                    *ptr += interpolatedSample;
                }
                
                streamPosition += streamPositionStep;
            }

            audioStream->pos = streamPosition;
        }

        frameCountMax = fmodf(frameCountMax + secondsPerFrame * frameCount, 1.0f);

        err = soundio_outstream_end_write(soundioOutStream);

        if (err) {
            TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to end writing to sound OutStream: %s", soundio_strerror(err));
            return;
        }

        framesLeft -= frameCount;
    }
}

void errorCallback(struct SoundIoOutStream* outStream, int err) {} //Literally why does Libsoundio break for no reason? (Dummy callback to avoid default abort)

TrioAudioContext* TrioCreateAudioContext(void) {
    TrioAudioContext* audioContext = soundio_create();

    if (!audioContext) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to create audio context");
        return NULL;
    }

    int err = soundio_connect(audioContext);

    if (err) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to connect audio context: %s", soundio_strerror(err));
        return NULL;
    }

    return audioContext;
}

int32_t TrioGetDefaultAudioOutputDeviceIndex(TrioAudioContext* audioContext) {
    soundio_flush_events(audioContext);

    int defaultOutputDeviceIndex = soundio_default_output_device_index(audioContext);

    if (defaultOutputDeviceIndex < 0) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to get default output device index");
        return -1;
    }

    return defaultOutputDeviceIndex;
}

TrioAudioDevice* TrioCreateAudioDevice(TrioAudioContext* audioContext, int32_t deviceIndex, uint32_t initialMixerStreamCapacity) {

    if (initialMixerStreamCapacity < 1) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "initialMixerStreamCapacity cannot be less than 1, received value %d", initialMixerStreamCapacity);
        return NULL;
    }

    TrioAudioDevice* audioDevice = calloc(1, sizeof(TrioAudioDevice));

    if (!audioDevice) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioDevice", sizeof(TrioAudioDevice));
        return NULL;
    }

    audioDevice->mixer.capacity = initialMixerStreamCapacity;
    audioDevice->mixer.count = 0;
    audioDevice->mixer.streams = malloc(sizeof(TrioAudioStream*) * initialMixerStreamCapacity);

    if (!audioDevice->mixer.streams) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioDevice mixer streams", (size_t)sizeof(TrioAudioStream*) * initialMixerStreamCapacity);
        return NULL;
    }

    struct SoundIoDevice* soundioDevice = soundio_get_output_device(audioContext, deviceIndex);

    if (!soundioDevice) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to create audio context");
        free(audioDevice->mixer.streams);
        free(audioDevice);
        return NULL;
    }

    audioDevice->device = soundioDevice;

    struct SoundIoOutStream *outStream = soundio_outstream_create(soundioDevice);
    outStream->userdata = audioDevice;
    outStream->write_callback = writeCallback;
    outStream->error_callback = errorCallback;

    int32_t err = soundio_outstream_open(outStream);

    if (err) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return NULL;
    }

    if (outStream->layout_error) {
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outStream->layout_error));
        return NULL;
    }

    err = soundio_outstream_start(outStream);

    if (err) {
        fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
        return NULL;
    }

    return audioDevice;
}

TrioAudioStream* TrioCreateAudioStream(TrioAudioStream* audioStream, TrioAudioBuffer* audioBuffer, double pos, bool playImmediately) {
    audioStream = malloc(sizeof(TrioAudioStream));
    if (!audioStream) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioDevice mixer streams", sizeof(TrioAudioStream));
        return NULL;
    }

    audioStream->buffer = audioBuffer;
    audioStream->pos = pos;
    audioStream->playing = playImmediately;

    return audioStream;
}

void TrioAddAudioStreamToAudioDevice(TrioAudioDevice* audioDevice, TrioAudioStream* audioStream) {
    if (audioDevice->mixer.count >= audioDevice->mixer.capacity) {
        audioDevice->mixer.capacity *= 2;

        TrioAudioStream** temp = realloc(audioDevice->mixer.streams, audioDevice->mixer.capacity * sizeof(TrioAudioStream*));
        if (temp) {
            audioDevice->mixer.streams = temp;
        } else {
            TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed reallocate memory (%zu Bytes) for TrioAudioDevice mixer streams", (size_t)sizeof(TrioAudioStream*) * audioDevice->mixer.capacity);
            return;
        }
    }

    audioDevice->mixer.streams[audioDevice->mixer.count] = audioStream;
    audioDevice->mixer.count += 1;
}

void TrioRemoveAudioStreamFromAudioDevice(TrioAudioDevice* audioDevice, TrioAudioStream* audioStream) {
    for (uint32_t i = 0; i < audioDevice->mixer.count; i++) {
        if (audioDevice->mixer.streams[i] == audioStream) {
            audioDevice->mixer.streams[i] = audioDevice->mixer.streams[audioDevice->mixer.count - 1];
            audioDevice->mixer.count -= 1;
            return;
        }
    }
}

void TrioDeleteAudioStream(TrioAudioStream* audioStream) {
    if (audioStream) {
        free(audioStream);
    }
}

void TrioDeleteAudioBuffer(TrioAudioBuffer* audioBuffer) {
    if (audioBuffer) {
        if (audioBuffer->data) {
            free(audioBuffer->data);
        }

        free(audioBuffer);
    }
}

void TrioCloseAudioDevice(TrioAudioDevice* audioDevice) {
    if (audioDevice) {
        soundio_outstream_destroy(audioDevice->outStream);
        soundio_device_unref(audioDevice->device);

        free(audioDevice->mixer.streams);
        free(audioDevice);
    }
}