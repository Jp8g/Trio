#include "../include/mp3_decoder.h"

TrioAudioBuffer* TrioLoadMp3(const char* path) {
    drmp3 file;
    if (!drmp3_init_file(&file, path, NULL)) {

        char* cwd = getcwd(NULL, 0);

        TrioLog(__func__, TrioGetDefaultLogConfig(),TRIO_ERROR, "Failed to initialise MP3 file \"%s\" from working directory \"%s\"", path, cwd);

        if (cwd) free(cwd);

        return NULL;
    }

    TrioAudioBuffer* buffer = malloc(sizeof(TrioAudioBuffer));
    if (!buffer) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%d Bytes)", sizeof(TrioAudioBuffer));
        drmp3_uninit(&file);
        return NULL;
    }

    buffer->frameCount = file.totalPCMFrameCount;
    buffer->channels = file.channels;
    buffer->sampleRate = file.sampleRate;

    buffer->data = malloc(sizeof(float) * file.totalPCMFrameCount * file.channels);
    if (!buffer->data) {
        TrioLog(__func__, TrioGetDefaultLogConfig(), TRIO_ERROR, "Failed allocate memory (%d Bytes)", sizeof(float) * file.totalPCMFrameCount * file.channels);
        free(buffer);
        drmp3_uninit(&file);
        return NULL;
    }

    size_t numberOfSamplesActuallyDecoded = drmp3_read_pcm_frames_f32(&file, file.totalPCMFrameCount, buffer->data);

    drmp3_uninit(&file);

    return buffer;
}