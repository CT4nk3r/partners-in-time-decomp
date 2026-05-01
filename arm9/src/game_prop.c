/**
 * Game Property System
 *
 * Tag-based property setter/getter for game objects.
 * Properties are identified by a 16-bit tag where the upper 4 bits
 * encode the property type/storage and the lower 12 bits are the index.
 *
 * Tag types:
 *   0x1000 — Direct u32 array (context + idx*4 + 4)
 *   0x2000 — Bitfield in global state (+0x48)
 *   0x3000 — External handler (func_0x0208102c)
 *   0x4000 — External handler (func_0x02078f8c)
 *   0x5000 — Global u32 array (state + idx*4)
 *   0x6000 — Global u8 array (state + idx + 0xd0)
 *   0x7000 — External handler (func_0x02083eac)
 *   0x8000 — Direct u32 array (param2 + idx*4 + 0x10)
 *   0x9000 — Direct u32 array (context + idx*4 + 0x24)
 *   0xA000 — (unused)
 *   0xB000 — Bitfield in context (+100)
 *   0xC000 — Global u32 array (state + idx*4 + 0x40)
 *   0xD000 — Bitfield in global state (+0x50)
 *   0xE000 — Bitfield in global state (+0x1F0)
 *
 * Decompiled from:
 *   FUN_020055c0 @ 0x020055C0 (828 bytes) - GameProp_Set
 *   FUN_02005900 @ 0x02005900 (616 bytes) - GameProp_Get
 */

#include "types.h"
#include "game_prop.h"

// Global game state pointer (shared with game_main)
extern u8 *gGameStateBase;  // DAT_020058fc / DAT_02005b68

// External dispatch functions (overlays or other modules)
extern void GameProp_ExternalSet3000(void);  // func_0x02078f8c
extern void GameProp_ExternalSet7000(void);  // func_0x02083eac
extern u32  GameProp_ExternalGet3000(int param2, int param3, u32 tag); // func_0x0208102c
extern u32  GameProp_ExternalGet4000(void);  // func_0x02079044
extern u32  GameProp_ExternalGet7000(void);  // func_0x02083ef4

#define TAG_TYPE(tag)  ((tag) & 0xF000)
#define TAG_INDEX(tag) ((tag) & 0x0FFF)

/**
 * GameProp_Set — Set a game property by tag
 * Original: FUN_020055c0 @ 0x020055C0
 *
 * @param tag     Property tag (type | index)
 * @param value   Value to set
 * @param param2  Context pointer (varies by tag type)
 * @param param3  Context pointer (varies by tag type)
 */
void GameProp_Set(u32 tag, int value, int param2, int param3)
{
    u32 type, idx;

    // 0xE000: bitfield in global state at +0x1F0
    if ((tag & 0xE000) == 0xE000) {
        idx = (tag & 0xFFFF1FFF) >> 5;
        if (value == 0) {
            *(u32 *)(gGameStateBase + 0x1F0 + idx * 4) &= ~(1 << (tag & 0x1F));
        } else {
            *(u32 *)(gGameStateBase + 0x1F0 + idx * 4) |= (1 << (tag & 0x1F));
        }
        return;
    }

    type = TAG_TYPE(tag);
    idx  = TAG_INDEX(tag);

    switch (type) {
    case 0x1000:
        *(int *)(param3 + idx * 4 + 4) = value;
        break;

    case 0x2000:
        if (value == 0) {
            *(u32 *)(gGameStateBase + 0x48 + (idx >> 5) * 4) &= ~(1 << (tag & 0x1F));
        } else {
            *(u32 *)(gGameStateBase + 0x48 + (idx >> 5) * 4) |= (1 << (tag & 0x1F));
        }
        break;

    case 0x3000:
    case 0x4000:
        GameProp_ExternalSet3000();
        break;

    case 0x5000:
        *(int *)(gGameStateBase + idx * 4) = value;
        break;

    case 0x6000:
        *(u8 *)(gGameStateBase + idx + 0xD0) = (u8)value;
        break;

    case 0x7000:
        GameProp_ExternalSet7000();
        break;

    case 0x8000:
        *(int *)(param2 + idx * 4 + 0x10) = value;
        break;

    case 0x9000:
        *(int *)(param3 + idx * 4 + 0x24) = value;
        break;

    case 0xB000:
        if (value == 0) {
            *(u32 *)(param3 + 100 + (idx >> 5) * 4) &= ~(1 << (tag & 0x1F));
        } else {
            *(u32 *)(param3 + 100 + (idx >> 5) * 4) |= (1 << (tag & 0x1F));
        }
        break;

    case 0xC000:
        *(int *)(gGameStateBase + idx * 4 + 0x40) = value;
        break;

    case 0xD000:
        if (value == 0) {
            *(u32 *)(gGameStateBase + 0x50 + (idx >> 5) * 4) &= ~(1 << (tag & 0x1F));
        } else {
            *(u32 *)(gGameStateBase + 0x50 + (idx >> 5) * 4) |= (1 << (tag & 0x1F));
        }
        break;

    default:
        break;
    }
}

/**
 * GameProp_Get — Get a game property by tag
 * Original: FUN_02005900 @ 0x02005900
 *
 * @param tag     Property tag (type | index)
 * @param param2  Context pointer
 * @param param3  Context pointer
 * @return        Property value
 */
u32 GameProp_Get(u32 tag, int param2, int param3)
{
    u32 type, idx;

    // 0xE000: bitfield in global state at +0x1F0
    if ((tag & 0xE000) == 0xE000) {
        return (*(u32 *)(gGameStateBase + ((tag & 0xFFFF1FFF) >> 5) * 4 + 0x1F0)
                >> (tag & 0x1F)) & 1;
    }

    type = TAG_TYPE(tag);
    idx  = TAG_INDEX(tag);

    switch (type) {
    case 0x1000:
        return *(u32 *)(param3 + idx * 4 + 4);

    case 0x2000:
        return (*(u32 *)(gGameStateBase + (idx >> 5) * 4 + 0x48) >> (idx & 0x1F)) & 1;

    case 0x3000:
        return GameProp_ExternalGet3000(param2, param3, tag);

    case 0x4000:
        return GameProp_ExternalGet4000();

    case 0x5000:
        return *(u32 *)(gGameStateBase + idx * 4);

    case 0x6000:
        return (u32)*(u8 *)(gGameStateBase + idx + 0xD0);

    case 0x7000:
        return GameProp_ExternalGet7000();

    case 0x8000:
        return *(u32 *)(param2 + idx * 4 + 0x10);

    case 0x9000:
        return *(u32 *)(param3 + idx * 4 + 0x24);

    case 0xB000:
        return (*(u32 *)(param3 + (idx >> 5) * 4 + 100) >> (idx & 0x1F)) & 1;

    case 0xC000:
        return *(u32 *)(gGameStateBase + idx * 4 + 0x40);

    case 0xD000:
        return (*(u32 *)(gGameStateBase + (idx >> 5) * 4 + 0x50) >> (idx & 0x1F)) & 1;

    default:
        return 0;
    }
}
