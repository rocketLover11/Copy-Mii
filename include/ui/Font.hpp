#pragma once

#ifndef FONT_HPP
#define FONT_HPP

#include <gccore.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "gx/Texture.hpp"

struct Glyph {
    f32 u0, v0, u1, v1;
    s16 bearingX, bearingY;
    s16 advance;
    u16 width, height;
};

class Font {
public:
    static constexpr u32 ATLAS_SIZE = 512;
    static constexpr u32 MAX_GLYPHS = 128;

    Font() = default;
    ~Font();

    bool load(const u8* data, u32 size, u32 pixelSize = 24);
    void drawText(f32 x, f32 y, const char* text, u8 r = 255, u8 g = 255, u8 b = 255, u8 a = 255);

    f32 measureText(const char* text) const;

private:
    bool bakeAtlas();
    void drawGlyph(f32 x, f32 y, const Glyph& glyph, u8 r, u8 g, u8 b, u8 a);

    FT_Library mLibrary = nullptr;
    FT_Face mFace = nullptr;

    Texture mAtlas;
    Glyph mGlyphs[MAX_GLYPHS];
    u32 mLineHeight = 0;
    u32 mPixelSize = 0;
    bool mReady = false;
};

#endif