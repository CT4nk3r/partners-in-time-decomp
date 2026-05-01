/**
 * Game Property System — Tag-based property getter/setter
 */

#ifndef GAME_PROP_H
#define GAME_PROP_H

#include "types.h"

void GameProp_Set(u32 tag, int value, int param2, int param3);
u32  GameProp_Get(u32 tag, int param2, int param3);

#endif /* GAME_PROP_H */
