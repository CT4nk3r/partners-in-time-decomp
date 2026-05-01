# Tool Setup Guide

This document explains how to install all tools required for the decompilation project.

## 1. Python 3.10+

**Windows:**
- Download from https://www.python.org/downloads/
- During installation, check "Add Python to PATH"

**Verify:**
```bash
python --version
```

## 2. Git

**Windows:**
- Download from https://git-scm.com/download/win

**Verify:**
```bash
git --version
```

## 3. devkitPro + devkitARM

devkitARM provides the ARM cross-compiler toolchain needed to compile code targeting the Nintendo DS hardware.

### Windows Installation

1. Download the **devkitPro installer** from:
   https://github.com/devkitPro/installer/releases

2. Run `devkitProUpdater.exe` as Administrator

3. In the component selection, ensure **devkitARM** is checked (for NDS development)

4. After installation, open the **MSYS2** shell (Start Menu → devkitPro → MSYS2) and run:
   ```bash
   pacman -Syu
   pacman -S nds-dev
   ```

5. Add to your system PATH (the installer usually does this):
   - `C:\devkitPro\devkitARM\bin`
   - `C:\devkitPro\tools\bin`

### Linux Installation

```bash
# Add devkitPro repository
wget https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x ./install-devkitpro-pacman
sudo ./install-devkitpro-pacman

# Install devkitARM and NDS tools
sudo dkp-pacman -S nds-dev
```

### macOS Installation

```bash
# Install devkitPro pacman
brew install --cask devkitpro-pacman

# Install devkitARM
sudo dkp-pacman -S nds-dev
```

### Environment Variables

Add these to your shell profile (`.bashrc`, `.zshrc`, or Windows System Environment):

```bash
export DEVKITPRO=/opt/devkitpro     # Linux/macOS
# or
# set DEVKITPRO=C:\devkitPro        # Windows

export DEVKITARM=${DEVKITPRO}/devkitARM
export PATH=${DEVKITARM}/bin:${DEVKITPRO}/tools/bin:$PATH
```

**Verify:**
```bash
arm-none-eabi-gcc --version
ndstool --version
```

## 4. Ghidra (Reverse Engineering)

Ghidra is used for analyzing the ROM binaries and producing the initial decompilation.

### Prerequisites: Java 17+

1. Download JDK 17+ from https://adoptium.net/
2. Install and add to PATH

**Verify:**
```bash
java --version
```

### Install Ghidra

1. Download the latest release from https://ghidra-sre.org/
2. Extract the ZIP to a location like `C:\ghidra` or `/opt/ghidra`
3. Run `ghidraRun.bat` (Windows) or `ghidraRun` (Linux/macOS) to launch the GUI

### Ghidra NDS Setup

When importing a binary (ARM9 or ARM7):

| Setting | ARM9 Value | ARM7 Value |
|---------|-----------|-----------|
| Format | Raw Binary | Raw Binary |
| Language | ARM:LE:32:v5t | ARM:LE:32:v4t |
| Base Address | 0x02000000 | 0x037F8000 |

### Headless Analysis (optional)

For batch analysis without the GUI:
```bash
analyzeHeadless <projectDir> <projectName> -import arm9.bin \
    -processor ARM:LE:32:v5t \
    -loader BinaryLoader \
    -loader-baseAddr 0x02000000
```

## 5. GNU Make

### Windows
- Included with devkitPro's MSYS2 environment
- Or install via [chocolatey](https://chocolatey.org/): `choco install make`

### Linux
```bash
sudo apt install build-essential    # Debian/Ubuntu
sudo dnf install make               # Fedora
```

### macOS
```bash
xcode-select --install
```

**Verify:**
```bash
make --version
```

## Summary Checklist

| Tool | Command to Verify | Required |
|------|-------------------|----------|
| Python 3.10+ | `python --version` | ✅ |
| Git | `git --version` | ✅ |
| devkitARM | `arm-none-eabi-gcc --version` | ✅ |
| ndstool | `ndstool` | ✅ |
| Java 17+ | `java --version` | ✅ (for Ghidra) |
| Ghidra | Launch GUI or `analyzeHeadless` | ✅ |
| Make | `make --version` | ✅ |
