#pragma once

#define MA_NO_ENGINE
#define MA_NO_NODE_GRAPH
#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MA_DEBUG_OUTPUT

#include "../../external/miniaudio/miniaudio.h"
#include "../../external/dr-libs/dr_wav.h"
#include "log.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct TrioAudioStream {
    float* data;
    double pos;
    uint32_t frameCount;
    uint32_t sampleRate;
    uint32_t channels;
} TrioAudioStream;

typedef struct TrioMixer {
    TrioAudioStream** streams;
    uint32_t count;
    uint32_t capacity;
} TrioMixer;

typedef struct TrioAudioDevice {
    ma_device device;
    TrioMixer mixer;
} TrioAudioDevice;

#ifdef __cplusplus
extern "C" {
#endif

TrioAudioDevice* TrioInitAudioDevice(uint32_t initialMixerStreamCapacity);
void TrioStartAudioDevice(TrioAudioDevice* device);
TrioAudioStream* TrioLoadWav(const char* path);
void TrioAddStreamToDevice(TrioAudioDevice* device, TrioAudioStream* audioStream);
void TrioCloseAudioDevice(TrioAudioDevice* device);

#ifdef __cplusplus
}
#endif