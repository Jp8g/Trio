#pragma once

#define MA_NO_ENGINE
#define MA_NO_NODE_GRAPH
#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MA_DEBUG_OUTPUT

#include "../../external/miniaudio/miniaudio.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct AudioStream {
    float* data;
    uint64_t frameCount;
    uint64_t pos;
    uint32_t channels;
    float volume;
    bool loop;
} AudioStream;

typedef struct Mixer {
    AudioStream** streams;
    uint32_t count;
    uint32_t capacity;
} Mixer;

typedef struct TrioAudioDevice {
    ma_device device;
    Mixer mixer;
} TrioAudioDevice;

#ifdef __cplusplus
extern "C" {
#endif

TrioAudioDevice* TrioInitAudioDevice(uint32_t initialMixerStreamCapacity);
void TrioStartAudioDevice(TrioAudioDevice* device);
void TrioAddStreamToDevice(TrioAudioDevice* device, AudioStream* audioStream);
void TrioCloseAudioDevice(TrioAudioDevice* device);
const char* TrioResultToString(ma_result result);

#ifdef __cplusplus
}
#endif