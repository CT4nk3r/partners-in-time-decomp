/*
 * ad_decompress.c — AlphaDream decompression (M&L series custom format)
 *
 * Format discovered via MnL-Modding/mnllib.py (mnllib/bis/compression.py).
 * Header: varint(uncompressed_size), varint(num_blocks - 1)
 * Each block: u16 block_size, then command bytes:
 *   Each command byte has 4 commands (2 bits each, LSB first):
 *     0 = end of block
 *     1 = literal byte
 *     2 = LZ back-reference (12-bit offset, 4-bit length + 2)
 *     3 = RLE (count + 2, value byte)
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Decode AlphaDream variable-length integer. */
static uint32_t ad_decode_varint(const uint8_t *data, uint32_t *pos)
{
    uint8_t b = data[*pos]; (*pos)++;
    uint32_t size = b >> 6;
    uint32_t result = b & 0x3F;
    for (uint32_t i = 0; i < size; i++) {
        result |= (uint32_t)data[*pos] << ((i + 1) * 6);
        (*pos)++;
    }
    return result;
}

/*
 * ad_decompress — Decompress AlphaDream-encoded data.
 *
 * src:      pointer to compressed data
 * src_len:  length of compressed data
 * out_size: receives the decompressed size
 *
 * Returns a malloc'd buffer with decompressed data, or NULL on failure.
 * Caller must free() the returned buffer.
 */
uint8_t *ad_decompress(const uint8_t *src, uint32_t src_len, uint32_t *out_size)
{
    if (!src || src_len < 4) return NULL;

    uint32_t pos = 0;
    uint32_t uncomp_size = ad_decode_varint(src, &pos);
    uint32_t num_blocks  = ad_decode_varint(src, &pos) + 1;

    if (uncomp_size == 0 || uncomp_size > 0x200000) {
        /* Sanity check: reject > 2MB decompressed output */
        return NULL;
    }

    uint8_t *out = (uint8_t *)malloc(uncomp_size);
    if (!out) return NULL;

    uint32_t out_pos = 0;

    for (uint32_t blk = 0; blk < num_blocks; blk++) {
        if (pos + 2 > src_len) break;
        uint16_t block_size = (uint16_t)src[pos] | ((uint16_t)src[pos + 1] << 8);
        pos += 2;
        uint32_t block_end = pos + block_size;
        if (block_end > src_len) block_end = src_len;

        int done = 0;
        for (int iter = 0; iter < 256 && !done && pos < block_end; iter++) {
            uint8_t cmd_byte = src[pos++];
            for (int c = 0; c < 4; c++) {
                uint8_t cmd = cmd_byte & 0x03;
                switch (cmd) {
                case 0: /* end of block */
                    done = 1;
                    break;
                case 1: /* literal byte */
                    if (pos < block_end && out_pos < uncomp_size)
                        out[out_pos++] = src[pos++];
                    break;
                case 2: { /* LZ back-reference */
                    if (pos + 1 >= block_end) { done = 1; break; }
                    uint8_t d1 = src[pos++];
                    uint8_t d2 = src[pos++];
                    uint32_t back_off = d1 | ((uint32_t)(d2 & 0xF0) << 4);
                    uint32_t length   = (d2 & 0x0F) + 2;
                    uint32_t src_pos  = out_pos - back_off;
                    for (uint32_t j = 0; j < length && out_pos < uncomp_size; j++)
                        out[out_pos++] = out[src_pos + j];
                    break;
                }
                case 3: { /* RLE */
                    if (pos + 1 >= block_end) { done = 1; break; }
                    uint32_t count = (uint32_t)src[pos++] + 2;
                    uint8_t  val   = src[pos++];
                    for (uint32_t j = 0; j < count && out_pos < uncomp_size; j++)
                        out[out_pos++] = val;
                    break;
                }
                }
                if (done) break;
                cmd_byte >>= 2;
            }
        }
        pos = block_end;
    }

    if (out_size) *out_size = out_pos;
    return out;
}
