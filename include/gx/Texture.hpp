#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <gccore.h>


class Texture {
public:
    Texture() = default;
    ~Texture();

    bool create(u16 width, u16 height);

    void upload(const u8* data, u16 width, u16 height);

    void bind(u8 slot = GX_TEXMAP0) const;

    u16 getWidth() const;
    u16 getHeight() const;
    bool isValid() const;
private:
    GXTexObj mTexObj;;
    void* mData = nullptr;
    u16 mWidth = 0;
    u16 mHeight = 0;

    static u16 padSize(u16 size, u16 tile);
};

#endif