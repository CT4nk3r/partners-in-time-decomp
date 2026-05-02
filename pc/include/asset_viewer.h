/*
 * asset_viewer.h — MLPIT_VIEWER mode
 *
 * When enabled (env MLPIT_VIEWER=1), the host skips the SIGSEGV-prone game
 * thread entirely and runs an interactive explorer over every entry in
 * assets/mlpit.assets.  Top screen shows a 4x4 thumbnail grid; bottom
 * screen shows metadata for the selected entry.
 *
 * Sub-modes:
 *   MLPIT_VIEWER_CYCLE=1   auto-advance one entry/sec (full-screen)
 *   MLPIT_VIEWER_DUMP_DIR  if set, dump rendered thumbnails as PPM there
 */
#ifndef ASSET_VIEWER_H
#define ASSET_VIEWER_H

#include <stdbool.h>

bool viewer_is_enabled(void);
int  viewer_run(void);   /* event loop; returns process exit code */

#endif
