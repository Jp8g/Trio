#include "../../../../../core/include/audio.h"
#include "../../../../../core/include/path.h"
#include "../../../../../external/opusfile/include/opusfile.h"

#ifdef __cplusplus
extern "C" {
#endif

TrioAudioBuffer* TrioLoadOpus(const char* path);

#ifdef __cplusplus
}
#endif
