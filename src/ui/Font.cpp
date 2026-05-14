#include "ui/Font.hpp"
#include "gx/Texture.hpp"

#include <gccore.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstring>
#include <cstdio>

Font::~Font() {
    if (mFace) FT_Done_Face(mFace);
    if (mLibrary) FT_Done_FreeType(mLibrary);
}

bool Font::load(const u8* data, u32 size, u32 pixelSize) {
    if (FT_Init_FreeType(&mLibrary)) {
        printf("Freetype init failed\n");
        return false;
    }

    if (FT_New_Memory_Face(mLibrary, data, size, 0, &mFace)) {
        printf("Failed to load font\n");
        return false;
    }

    mPixelSize = pixelSize;
    FT_Set_Pixel_Sizes(mFace, 0, pixelSize);
    mLineHeight = mFace->size->metrics.height >> 6;

    return bakeAtlas();
}

bool Font::bakeAtlas() {
    u32 bufSize = ATLAS_SIZE * ATLAS_SIZE * 4;
    u8* buf = new u8[bufSize];
    memset(buf, 0, bufSize);

    u32 penX = 0, penY = 0;
    u32 rowH = 0;

    for (u32 c = 32; c < MAX_GLYPHS; c++) {
        if (FT_Load_Char(mFace, c, FT_LOAD_RENDER)) continue;

        FT_Bitmap& bmp = mFace->glyph->bitmap;
        u32 w = bmp.width;
        u32 h = bmp.rows;

        if (penX + w >= ATLAS_SIZE) {
            penX = 0;
            penY += rowH + 1;
            rowH = 0;
        }

        if (penY + h >= ATLAS_SIZE) {
            printf("Font atlas full!\n");
            break;
        }

        for (u32 row = 0; row < h; row++) {
            for (u32 col = 0; col < w; col++) {
                u8 alpha = bmp.buffer[row * bmp.pitch + col];
                u32 idx = ((penY + row) * ATLAS_SIZE + (penX + col)) * 4;
                buf[idx] = 255;
                buf[idx + 1] = 255;
                buf[idx + 2] = 255;
                buf[idx + 3] = alpha;
            }
        }

        Glyph& g = mGlyphs[c];
        g.u0 = (f32)penX / ATLAS_SIZE;
        g.v0 = (f32)penY / ATLAS_SIZE;
        g.u1 = (f32)(penX + w) / ATLAS_SIZE;
        g.v1 = (f32)(penY + h) / ATLAS_SIZE;
        g.bearingX = mFace->glyph->bitmap_left;
        g.bearingY = mFace->glyph->bitmap_top;
        g.advance = mFace->glyph->advance.x >> 6;
        g.width = w;
        g.height = h;

        penX += w + 1;
        if (h > rowH) rowH = h;
    }

    mAtlas.create(ATLAS_SIZE, ATLAS_SIZE);
    mAtlas.upload(buf, ATLAS_SIZE, ATLAS_SIZE);
    delete[] buf;

    mReady = true;
    return true;
}

void Font::drawGlyph(f32 x, f32 y, const Glyph& g, u8 r, u8 g2, u8 b, u8 a) {
    f32 x0 = x + g.bearingX;
    f32 y0 = y - g.bearingY;
    f32 x1 = x0 + g.width;
    f32 y1 = y0 + g.height;

    GX_Begin(GX_QUADS, GX_VTXFMT1, 4);
        GX_Position2f32(x0, y0); GX_Color4u8(r, g2, b, a); GX_TexCoord2f32(g.u0, g.v0);
        GX_Position2f32(x1, y0); GX_Color4u8(r, g2, b, a); GX_TexCoord2f32(g.u1, g.v0);
        GX_Position2f32(x1, y1); GX_Color4u8(r, g2, b, a); GX_TexCoord2f32(g.u1, g.v1);
        GX_Position2f32(x0, y1); GX_Color4u8(r, g2, b, a); GX_TexCoord2f32(g.u0, g.v1);
    GX_End();
}

void Font::drawText(f32 x, f32 y, const char* text, u8 r, u8 g, u8 b, u8 a) {
    if (!mReady) return;

    GX_SetVtxDesc(GX_VA_POS,  GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS,  GX_POS_XY,  GX_F32,  0);
    GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST,  GX_F32,  0);

    GX_SetNumTexGens(1);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

    // modulate texture color with vertex color (for tinting)
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);

    // enable alpha blending so font edges are smooth
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);

    mAtlas.bind(GX_TEXMAP0);

    f32 curX = x;
    for (const char* p = text; *p; p++) {
        u8 c = (u8)*p;
        if (c < 32 || c >= MAX_GLYPHS) continue;
        const Glyph& glyph = mGlyphs[c];
        if (glyph.width > 0)
            drawGlyph(curX, y, glyph, r, g, b, a);
        curX += glyph.advance;
    }

    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetNumTexGens(0);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
}

f32 Font::measureText(const char* text) const {
    if (!mReady) return 0;
    f32 width = 0;
    for (const char* p = text; *p; p++) {
        u8 c = (u8)*p;
        if (c < 32 || c >= MAX_GLYPHS) continue;
        width += mGlyphs[c].advance;
    }
    return width;
}