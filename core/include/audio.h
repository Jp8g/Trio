#pragma once

#include "../../external/libsoundio_static/soundio/soundio.h"
#include "log.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct SoundIo TrioAudioContext;

typedef struct TrioAudioBuffer {
    float* data;
    uint32_t frameCount;
    uint32_t channels;
    uint32_t sampleRate;
} TrioAudioBuffer;

typedef struct TrioAudioStream {
    struct TrioAudioBuffer* buffer;
    double pos;
    bool playing;
} TrioAudioStream;

typedef struct TrioMixer {
    TrioAudioStream** streams;
    uint32_t count;
    uint32_t capacity;
} TrioMixer;

typedef struct TrioAudioDevice {
    struct SoundIoDevice* device;
    struct SoundIoOutStream* outStream;
    TrioMixer mixer;
} TrioAudioDevice;

#ifdef __cplusplus
extern "C" {
#endif

TrioAudioContext* TrioCreateAudioContext(void);
int32_t TrioGetDefaultAudioOutputDeviceIndex(TrioAudioContext* audioContext);
TrioAudioDevice* TrioCreateAudioDevice(TrioAudioContext* audioContext, int32_t deviceIndex, uint32_t initialMixerStreamCapacity);
TrioAudioStream* TrioCreateAudioStream(TrioAudioStream* stream, TrioAudioBuffer* buffer, double pos, bool playImmediately);
void TrioAddAudioStreamToAudioDevice(TrioAudioDevice* device, TrioAudioStream* audioStream);
void TrioRemoveAudioStreamFromAudioDevice(TrioAudioDevice* device, TrioAudioStream* audioStream);
void TrioDeleteAudioStream(TrioAudioStream* audioStream);
void TrioDeleteAudioBuffer(TrioAudioBuffer* audioBuffer);
void TrioCloseAudioDevice(TrioAudioDevice* device);
void TrioCloseAudioContext(TrioAudioContext* context);

#ifdef __cplusplus
}
#endif