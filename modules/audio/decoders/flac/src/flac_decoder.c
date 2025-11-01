#include "../include/flac_decoder.h"

TrioAudioBuffer* TrioLoadFlac(const char* path) {
    drflac* fileptr = drflac_open_file(path, NULL);

    if (!fileptr) {

        char* cwd = getcwd(NULL, 0);

        TrioLog(__func__, TrioGetDefaultLogConfig(),TRIO_ERROR, "Failed to open FLAC file \"%s\" from working directory \"%s\"", path, cwd);

        if (cwd) free(cwd);

        return NULL;
    }

    TrioAudioBuffer* buffer = malloc(sizeof(TrioAudioBuffer));
    if (!buffer) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%d Bytes)", sizeof(TrioAudioBuffer));
        return NULL;
    }

    buffer->frameCount = fileptr->totalPCMFrameCount;
    buffer->channels = fileptr->channels;
    buffer->sampleRate = fileptr->sampleRate;

    buffer->data = malloc(sizeof(float) * fileptr->totalPCMFrameCount * fileptr->channels);
    if (!buffer->data) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%d Bytes)", sizeof(float) * fileptr->totalPCMFrameCount * fileptr->channels);
        free(buffer);
        return NULL;
    }

    size_t numberOfSamplesActuallyDecoded = drflac_read_pcm_frames_f32(fileptr, fileptr->totalPCMFrameCount, buffer->data);

    return buffer;
}