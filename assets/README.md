# assets/

This directory holds the **generated** MLPIT asset pack.

| File | Description |
|------|-------------|
| `mlpit.assets` | All game assets extracted from your NDS ROM |

## How it gets here

On first launch, the PC port automatically extracts your ROM:

```
roms/baserom.nds  ──(first run)──►  assets/mlpit.assets
```

After that you may delete `roms/baserom.nds` — only `mlpit.assets` is
needed at runtime.

You can also run the extractor manually:

```sh
python tools/extract_assets.py
# or specify paths:
python tools/extract_assets.py roms/baserom.nds assets/mlpit.assets
```

## ⚠️ Do not redistribute

`mlpit.assets` is derived from a copyrighted Nintendo game.
It is listed in `.gitignore` and must never be committed to this repo.
