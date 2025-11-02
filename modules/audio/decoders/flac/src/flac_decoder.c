#define DR_FLAC_IMPLEMENTATION

#include "../include/flac_decoder.h"

TrioAudioBuffer* TrioLoadFlac(const char* path) {

    char* resolvedPath = TrioResolvePath(path);

    drflac* fileptr = drflac_open_file(resolvedPath, NULL);

    if (!fileptr) {
        TrioLog(__func__, TrioGetDefaultLogConfig(),TRIO_ERROR, "Failed to open FLAC file \"%s\"", resolvedPath);
        free(resolvedPath);
        return NULL;
    }

    free(resolvedPath);

    TrioAudioBuffer* buffer = malloc(sizeof(TrioAudioBuffer));
    if (!buffer) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes) for TrioAudioBuffer", (size_t)sizeof(TrioAudioBuffer));
        return NULL;
    }

    buffer->frameCount = fileptr->totalPCMFrameCount;
    buffer->channels = fileptr->channels;
    buffer->sampleRate = fileptr->sampleRate;

    buffer->data = malloc(sizeof(float) * fileptr->totalPCMFrameCount * fileptr->channels);
    if (!buffer->data) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%zu Bytes for TrioAudioBuffer PCM data)", (size_t)sizeof(float) * fileptr->totalPCMFrameCount * fileptr->channels);
        free(buffer);
        return NULL;
    }

    size_t numberOfSamplesActuallyDecoded = drflac_read_pcm_frames_f32(fileptr, fileptr->totalPCMFrameCount, buffer->data);

    return buffer;
}