# CopyMii

A Wii homebrew app that lets you browse and install Homebrew Channel apps directly on your Wii over the network. A host computer serves archived HBC apps (`.zip`) via a local server, and the Wii downloads and extracts them straight to your SD card.

## How it works

1. Run the server on your host computer, pointing it at a folder of `.zip` HBC app archives
2. Launch CopyMii on your Wii
3. Browse the list of available apps and install them directly to `SD:/apps/`

Each `.zip` should contain a single folder in standard HBC layout:

```
your_app_name/
├── boot.dol
├── meta.xml
└── icon.png
```

---

## Building

### Prerequisites

- [devkitPro](https://devkitpro.org) with `wii-dev`, `ppc-zlib`, and `ppc-libzip` installed
- CMake 3.20+

#### Install devkitPro (Debian/Ubuntu)

```bash
wget -U "dkp-apt" https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x ./install-devkitpro-pacman
sudo ./install-devkitpro-pacman
```

#### Install required packages

```bash
sudo dkp-pacman -S wii-dev ppc-zlib ppc-libzip
```

#### Environment variables

Add to your `~/.bashrc` or `~/.zshrc`:

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPRO/tools/bin:$DEVKITPPC/bin:$PATH
```

Then reload: `source ~/.bashrc`

---

### Build the Wii app

```bash
mkdir build && cd build
/opt/devkitpro/portlibs/wii/bin/powerpc-eabi-cmake -S .. -B .
make -j$(nproc)
```

Or with CMake presets:

```bash
cmake --preset wii
cmake --build build -j$(nproc)
```

This produces `CopyMii.dol`. Copy it to `SD:/apps/CopyMii/boot.dol` on your SD card.

---

### Run the server

> **Note:** The server is currently a Python script. A native C++ CLI tool is planned.

```bash
cd server
python3 server.py
```

By default it serves on port `8080` from the current directory. Place your `.zip` app archives in the same folder.

---

## Project structure

```
CopyMii/
├── src/
│   └── Main.cpp
├── include/
├── server/
│   └── server.py
└── CMakeLists.txt
```

---

## Status

Early development — work in progress.