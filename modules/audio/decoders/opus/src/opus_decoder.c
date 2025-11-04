#include "../include/opus_decoder.h"

TrioAudioBuffer* TrioLoadOpus(const char* path) {
    int err;

    char* resolvedPath = TrioResolvePath(path);

    OggOpusFile* opusFile = op_open_file(resolvedPath, &err);
    if (!opusFile || err != OPUS_OK) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed to open Opus file \"%s\"", resolvedPath);
        free(resolvedPath);
        return NULL;
    }

    free(resolvedPath);

    const OpusHead* opusHead = op_head(opusFile, -1);
    uint64_t total_samples = op_pcm_total(opusFile, -1);

    TrioAudioBuffer* audioBuffer = malloc(sizeof(TrioAudioBuffer));
    if (!audioBuffer) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioBuffer (TrioAudioBuffer* buffer)", (size_t)sizeof(TrioAudioBuffer));
        op_free(opusFile);
        return NULL;
    }

    audioBuffer->data = malloc(sizeof(float) * total_samples * opusHead->channel_count);
    audioBuffer->channels = opusHead->channel_count;
    audioBuffer->sampleRate = 48000;

    if (!audioBuffer->data) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioBuffer PCM data (float* data)", (size_t)sizeof(float) * total_samples * opusHead->channel_count);
        op_free(opusFile);
        return NULL;
    }

    opus_int64 total_decoded = 0;
    while (1) {
        int frames = op_read_float(opusFile, audioBuffer->data + total_decoded * opusHead->channel_count, (int)(total_samples - total_decoded), NULL);
        if (frames <= 0) break;
        total_decoded += frames;
    }
    
    audioBuffer->frameCount = total_decoded;

    op_free(opusFile);

    return audioBuffer;
}