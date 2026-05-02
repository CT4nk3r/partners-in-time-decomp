/*
 * ad_decompress.h — AlphaDream decompression for M&L series assets
 */
#ifndef AD_DECOMPRESS_H
#define AD_DECOMPRESS_H

#include <stdint.h>

/*
 * ad_decompress — Decompress AlphaDream-encoded data.
 *
 * src:      pointer to compressed data
 * src_len:  length of compressed data
 * out_size: receives the decompressed size (may be NULL)
 *
 * Returns a malloc'd buffer with decompressed data, or NULL on failure.
 * Caller must free() the returned buffer.
 */
uint8_t *ad_decompress(const uint8_t *src, uint32_t src_len, uint32_t *out_size);

#endif /* AD_DECOMPRESS_H */
