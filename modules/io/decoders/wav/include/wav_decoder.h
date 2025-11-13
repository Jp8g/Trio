#pragma once

#include "../../../../../core/include/audio.h"
#include "../../../../../core/include/io.h"
#include "../../../../../external/dr-libs/dr_wav.h"

#ifdef __cplusplus
extern "C" {
#endif

TrioAudioBuffer* TrioLoadWav(const char* path);

#ifdef __cplusplus
}
#endif
