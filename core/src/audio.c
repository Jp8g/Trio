#include "../include/audio.h"

const char* MAResultToString(ma_result result)
{
    switch(result) {
        case MA_SUCCESS: return "MA_SUCCESS";
        case MA_ERROR: return "MA_ERROR";
        case MA_INVALID_ARGS: return "MA_INVALID_ARGS";
        case MA_INVALID_OPERATION: return "MA_INVALID_OPERATION";
        case MA_OUT_OF_MEMORY: return "MA_OUT_OF_MEMORY";
        case MA_OUT_OF_RANGE: return "MA_OUT_OF_RANGE";
        case MA_ACCESS_DENIED: return "MA_ACCESS_DENIED";
        case MA_DOES_NOT_EXIST: return "MA_DOES_NOT_EXIST";
        case MA_ALREADY_EXISTS: return "MA_ALREADY_EXISTS";
        case MA_TOO_MANY_OPEN_FILES: return "MA_TOO_MANY_OPEN_FILES";
        case MA_INVALID_FILE: return "MA_INVALID_FILE";
        case MA_TOO_BIG: return "MA_TOO_BIG";
        case MA_PATH_TOO_LONG: return "MA_PATH_TOO_LONG";
        case MA_NAME_TOO_LONG: return "MA_NAME_TOO_LONG";
        case MA_NOT_DIRECTORY: return "MA_NOT_DIRECTORY";
        case MA_IS_DIRECTORY: return "MA_IS_DIRECTORY";
        case MA_DIRECTORY_NOT_EMPTY: return "MA_DIRECTORY_NOT_EMPTY";
        case MA_AT_END: return "MA_AT_END";
        case MA_NO_SPACE: return "MA_NO_SPACE";
        case MA_BUSY: return "MA_BUSY";
        case MA_IO_ERROR: return "MA_IO_ERROR";
        case MA_INTERRUPT: return "MA_INTERRUPT";
        case MA_UNAVAILABLE: return "MA_UNAVAILABLE";
        case MA_ALREADY_IN_USE: return "MA_ALREADY_IN_USE";
        case MA_BAD_ADDRESS: return "MA_BAD_ADDRESS";
        case MA_BAD_SEEK: return "MA_BAD_SEEK";
        case MA_BAD_PIPE: return "MA_BAD_PIPE";
        case MA_DEADLOCK: return "MA_DEADLOCK";
        case MA_TOO_MANY_LINKS: return "MA_TOO_MANY_LINKS";
        case MA_NOT_IMPLEMENTED: return "MA_NOT_IMPLEMENTED";
        case MA_NO_MESSAGE: return "MA_NO_MESSAGE";
        case MA_BAD_MESSAGE: return "MA_BAD_MESSAGE";
        case MA_NO_DATA_AVAILABLE: return "MA_NO_DATA_AVAILABLE";
        case MA_INVALID_DATA: return "MA_INVALID_DATA";
        case MA_TIMEOUT: return "MA_TIMEOUT";
        case MA_NO_NETWORK: return "MA_NO_NETWORK";
        case MA_NOT_UNIQUE: return "MA_NOT_UNIQUE";
        case MA_NOT_SOCKET: return "MA_NOT_SOCKET";
        case MA_NO_ADDRESS: return "MA_NO_ADDRESS";
        case MA_BAD_PROTOCOL: return "MA_BAD_PROTOCOL";
        case MA_PROTOCOL_UNAVAILABLE: return "MA_PROTOCOL_UNAVAILABLE";
        case MA_PROTOCOL_NOT_SUPPORTED: return "MA_PROTOCOL_NOT_SUPPORTED";
        case MA_PROTOCOL_FAMILY_NOT_SUPPORTED: return "MA_PROTOCOL_FAMILY_NOT_SUPPORTED";
        case MA_ADDRESS_FAMILY_NOT_SUPPORTED: return "MA_ADDRESS_FAMILY_NOT_SUPPORTED";
        case MA_SOCKET_NOT_SUPPORTED: return "MA_SOCKET_NOT_SUPPORTED";
        case MA_CONNECTION_RESET: return "MA_CONNECTION_RESET";
        case MA_ALREADY_CONNECTED: return "MA_ALREADY_CONNECTED";
        case MA_NOT_CONNECTED: return "MA_NOT_CONNECTED";
        case MA_CONNECTION_REFUSED: return "MA_CONNECTION_REFUSED";
        case MA_NO_HOST: return "MA_NO_HOST";
        case MA_IN_PROGRESS: return "MA_IN_PROGRESS";
        case MA_CANCELLED: return "MA_CANCELLED";
        case MA_MEMORY_ALREADY_MAPPED: return "MA_MEMORY_ALREADY_MAPPED";
        case MA_CRC_MISMATCH: return "MA_CRC_MISMATCH";
        case MA_FORMAT_NOT_SUPPORTED: return "MA_FORMAT_NOT_SUPPORTED";
        case MA_DEVICE_TYPE_NOT_SUPPORTED: return "MA_DEVICE_TYPE_NOT_SUPPORTED";
        case MA_SHARE_MODE_NOT_SUPPORTED: return "MA_SHARE_MODE_NOT_SUPPORTED";
        case MA_NO_BACKEND: return "MA_NO_BACKEND";
        case MA_NO_DEVICE: return "MA_NO_DEVICE";
        case MA_API_NOT_FOUND: return "MA_API_NOT_FOUND";
        case MA_INVALID_DEVICE_CONFIG: return "MA_INVALID_DEVICE_CONFIG";
        case MA_LOOP: return "MA_LOOP";
        case MA_BACKEND_NOT_ENABLED: return "MA_BACKEND_NOT_ENABLED";
        case MA_DEVICE_NOT_INITIALIZED: return "MA_DEVICE_NOT_INITIALIZED";
        case MA_DEVICE_ALREADY_INITIALIZED: return "MA_DEVICE_ALREADY_INITIALIZED";
        case MA_DEVICE_NOT_STARTED: return "MA_DEVICE_NOT_STARTED";
        case MA_DEVICE_NOT_STOPPED: return "MA_DEVICE_NOT_STOPPED";
        case MA_FAILED_TO_INIT_BACKEND: return "MA_FAILED_TO_INIT_BACKEND";
        case MA_FAILED_TO_OPEN_BACKEND_DEVICE: return "MA_FAILED_TO_OPEN_BACKEND_DEVICE";
        case MA_FAILED_TO_START_BACKEND_DEVICE: return "MA_FAILED_TO_START_BACKEND_DEVICE";
        case MA_FAILED_TO_STOP_BACKEND_DEVICE: return "MA_FAILED_TO_STOP_BACKEND_DEVICE";

        default: return "Unknown";
    }
}

void DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    (void)pInput;

    float* out = (float*)pOutput;

    TrioMixer* mixer = &((TrioAudioDevice*)pDevice->pUserData)->mixer;

    for (uint32_t streamIndex = 0; streamIndex < mixer->count; streamIndex++) {

        TrioAudioStream* audioStream = mixer->streams[streamIndex];

        if (!audioStream->playing) continue;

        double streamPosition = audioStream->pos;
        uint32_t streamFrameCount = audioStream->buffer->frameCount;

        double streamPositionStep = (double)audioStream->buffer->sampleRate / pDevice->sampleRate;

        for (uint32_t frame = 0; frame < frameCount; frame++) {

            if (streamPosition >= streamFrameCount) break;

            uint32_t streamPositionFloor = (uint32_t)streamPosition;
            if (streamPositionFloor >= audioStream->buffer->frameCount - 1)
                streamPositionFloor = audioStream->buffer->frameCount - 1;
            
            double weightToNextSample = streamPosition - (double)streamPositionFloor;

            for (uint32_t channel = 0; channel < pDevice->playback.channels; channel++) {
                uint32_t dataIndex = streamPositionFloor;

                if (audioStream->buffer->channels != 1) dataIndex = streamPositionFloor * audioStream->buffer->channels + channel;

                uint32_t nextFramePosition = streamPositionFloor + 1;

                if (nextFramePosition >= streamFrameCount) nextFramePosition = streamFrameCount - 1;

                uint32_t nextSampleIndex = nextFramePosition * audioStream->buffer->channels + channel;

                float interpolatedSample = (audioStream->buffer->data[dataIndex] * (1 - weightToNextSample)) + (audioStream->buffer->data[nextSampleIndex] * weightToNextSample);

                out[frame * pDevice->playback.channels + channel] += interpolatedSample;
            }
            
            streamPosition += streamPositionStep;
        }

        audioStream->pos = streamPosition;
    }
}

TrioAudioDevice* TrioInitAudioDevice(uint32_t initialMixerStreamCapacity, uint32_t periodSizeInFrames, uint32_t channels, uint32_t sampleRate) {

    if (initialMixerStreamCapacity < 1) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "initialMixerStreamCapacity cannot be less than 1, received value %d", initialMixerStreamCapacity);
        return NULL;
    }

    TrioAudioDevice* device = calloc(1, sizeof(TrioAudioDevice));

    if (!device) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioDevice", sizeof(TrioAudioDevice));
        return NULL;
    }

    device->mixer.capacity = initialMixerStreamCapacity;
    device->mixer.count = 0;
    device->mixer.streams = malloc(sizeof(TrioAudioStream*) * initialMixerStreamCapacity);

    if (!device->mixer.streams) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioDevice mixer streams", (size_t)sizeof(TrioAudioStream*) * initialMixerStreamCapacity);
        return NULL;
    }

    ma_device_config config = ma_device_config_init(ma_device_type_playback);

    config.playback.format   = ma_format_f32;
    config.periodSizeInFrames= periodSizeInFrames;
    config.playback.channels = channels;
    config.sampleRate        = sampleRate;
    config.dataCallback      = DataCallback;
    config.pUserData         = device;

    if (ma_device_init(NULL, &config, &device->device) != MA_SUCCESS) {
        free(device->mixer.streams);
        free(device);
        return NULL;
    }

    return device;
}

bool TrioStartAudioDevice(TrioAudioDevice* device) {
    ma_result result = ma_device_start(&device->device);

    if (result != MA_SUCCESS) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to start AudioDevice: %s", MAResultToString(result));
        return false;
    }

    return true;
}

TrioAudioStream* TrioCreateAudioStream(TrioAudioStream* stream, TrioAudioBuffer* buffer, double pos, bool playImmediately) {
    if (!stream) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to create audio stream: stream is null");
        return NULL;
    }

    stream->buffer = buffer;
    stream->pos = pos;
    stream->playing = playImmediately;

    return stream;
}

void TrioAddStreamToDevice(TrioAudioDevice* device, TrioAudioStream* audioStream) {
    if (device->mixer.count >= device->mixer.capacity) {
        device->mixer.capacity *= 2;
        TrioAudioStream** temp = realloc(device->mixer.streams, device->mixer.capacity * sizeof(TrioAudioStream*));
        if (temp) {
            device->mixer.streams = temp;
        }
        else {
            TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed reallocate memory (%zu Bytes) for TrioAudioDevice mixer streams", (size_t)sizeof(TrioAudioStream*) * device->mixer.capacity);
            return;
        }
    }

    device->mixer.streams[device->mixer.count] = audioStream;
    device->mixer.count += 1;
}

void TrioCloseAudioDevice(TrioAudioDevice* device) {
    ma_device_uninit(&device->device);

    free(device->mixer.streams);
    free(device);
}