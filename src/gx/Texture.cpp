#include "gx/Texture.hpp"

#include <cstdlib>
#include <cstring>
#include <malloc.h>

Texture::~Texture() {
    if (mData) {
        free(mData);
        mData = nullptr;
    }
}

bool Texture::create(u16 width, u16 height) {
    mWidth = padSize(width, 4);
    mHeight = padSize(height, 4);

    u32 size = mWidth * mHeight * 4;
    mData = memalign(32, size);
    if (!mData) return false;

    memset(mData, 0, size);
    DCFlushRange(mData, size);

    GX_InitTexObj(&mTexObj, mData, mWidth, mHeight, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GX_InitTexObjLOD(&mTexObj, GX_LINEAR, GX_LINEAR, 0, 0, 0, GX_FALSE, GX_FALSE, GX_ANISO_1);
    return true;
}

void Texture::upload(const u8* data, u16 width, u16 height) {
    if (!mData) return;

    u16 tw = padSize(width, 4);
    u16 th = padSize(height, 4);

    u8* dst = (u8*)mData;
    memset(dst, 0, tw * th * 4);

    for (u16 y = 0; y < height; y++) {
        for (u16 x = 0; x < width; x++) {
            const u8* src = data + (y * width + x) * 4;
            u8 r = src[0], g = src[1], b = src[2], a = src[3];

            u32 tileX = x / 4;
            u32 tileY = y / 4;
            u32 tilesW = tw / 4;
            u32 tileIdx = tileY * tilesW + tileX;
            u32 pixelInTile = (y % 4) * 4 + (x % 4);

            u8* tile = dst + tileIdx * 32;
            tile[pixelInTile * 2]     = a;  // AR block
            tile[pixelInTile * 2 + 1] = r;
            tile[16 + pixelInTile * 2]     = g;  // GB block
            tile[16 + pixelInTile * 2 + 1] = b;
        }
    }

    DCFlushRange(mData, tw * th * 4);
    GX_InitTexObj(&mTexObj, mData, tw, th,
                  GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
}

void Texture::bind(u8 slot) const {
    if (!mData) return;
    GX_LoadTexObj(const_cast<GXTexObj*>(&mTexObj), slot);
}

u16 Texture::getWidth() const { return mWidth; }
u16 Texture::getHeight() const { return mHeight; }
bool Texture::isValid() const { return mData != nullptr; }