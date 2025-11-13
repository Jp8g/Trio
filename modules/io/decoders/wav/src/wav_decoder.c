#define DR_WAV_IMPLEMENTATION

#include "../include/wav_decoder.h"

TrioAudioBuffer* TrioLoadWav(const char* path) {
    drwav file;

    char* resolvedPath = TrioResolvePath(path);
    
    if (!drwav_init_file(&file, resolvedPath, NULL)) {
        TrioLog(__func__, TrioGetDefaultLogConfig(),TRIO_ERROR, "Failed to initialise WAV file \"%s\"", resolvedPath);
        free(resolvedPath);
        return NULL;
    }

    free(resolvedPath);

    TrioAudioBuffer* buffer = malloc(sizeof(TrioAudioBuffer));
    if (!buffer) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioBuffer (TrioAudioBuffer* buffer)", (size_t)sizeof(TrioAudioBuffer));
        drwav_uninit(&file);
        return NULL;
    }

    buffer->frameCount = file.totalPCMFrameCount;
    buffer->channels = file.channels;
    buffer->sampleRate = file.sampleRate;

    buffer->data = malloc(sizeof(float) * file.totalPCMFrameCount * file.channels);
    if (!buffer->data) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioBuffer PCM data (float* data)", (size_t)sizeof(float) * file.totalPCMFrameCount * file.channels);
        free(buffer);
        drwav_uninit(&file);
        return NULL;
    }

    size_t numberOfSamplesActuallyDecoded = drwav_read_pcm_frames_f32(&file, file.totalPCMFrameCount, buffer->data);

    drwav_uninit(&file);

    return buffer;
}