#pragma once

#include "../../../../../core/include/audio.h"
#include "../../../../../core/include/io.h"
#include "../../../../../external/dr-libs/dr_mp3.h"

#ifdef __cplusplus
extern "C" {
#endif

TrioAudioBuffer* TrioLoadMp3(const char* path);

#ifdef __cplusplus
}
#endif