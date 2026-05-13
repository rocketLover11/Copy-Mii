#include "gx/Renderer.hpp"

Renderer& Renderer::instance() {
    static Renderer renderer;
    return renderer;
}

void Renderer::init() {
    initVideo();
    initGX();
}

void Renderer::initVideo() {
    VIDEO_Init();
    mMode = VIDEO_GetPreferredMode(nullptr);

    mFrameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mMode));
    mFrameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mMode));

    VIDEO_Configure(mMode);
    VIDEO_SetNextFramebuffer(mFrameBuffer[mFbIndex]);
    VIDEO_SetBlack(false);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (mMode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();
}

void Renderer::initGX() {
    mFifoBuffer = memalign(32, FIFO_SIZE);
    memset(mFifoBuffer, 0, FIFO_SIZE);

    GX_Init(mFifoBuffer, FIFO_SIZE);

    GXColor background = { 175, 194, 224, 255 };
    GX_SetCopyClear(background, GX_MAX_Z24);

    GX_SetViewport(0, 0, mMode->fbWidth, mMode->efbHeight, 0, 1);

    f32 yscale = GX_GetYScaleFactor(mMode->efbHeight, mMode->xfbHeight);
    u32 xfbHeight = GX_SetDispCopyYScale(yscale);
    GX_SetScissor(0, 0, mMode->fbWidth, mMode->efbHeight);
    GX_SetDispCopySrc(0, 0, mMode->fbWidth, mMode->efbHeight);
    GX_SetDispCopyDst(mMode->fbWidth, xfbHeight);
    GX_SetCopyFilter(mMode->aa, mMode->sample_pattern, GX_TRUE, mMode->vfilter);
    GX_SetFieldMode(mMode->field_rendering, ((mMode->viHeight == 2 * mMode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

    GX_SetCullMode(GX_CULL_NONE);
    GX_CopyDisp(mFrameBuffer[mFbIndex], GX_TRUE);
    GX_SetDispCopyGamma(GX_GM_1_0);

    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

    GX_SetNumChans(1);
    GX_SetNumTexGens(0);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

    Mtx44 proj;
    guOrtho(proj, 0, mMode->efbHeight, 0, mMode->fbWidth, 0, 300);
    GX_LoadProjectionMtx(proj, GX_ORTHOGRAPHIC);

    Mtx view;
    guMtxIdentity(view);
    GX_LoadPosMtxImm(view, GX_PNMTX0);
}

void Renderer::beginFrame() {
    Mtx view;
    guMtxIdentity(view);
    GX_LoadPosMtxImm(view, GX_PNMTX0);
}

void Renderer::endFrame() {
    GX_DrawDone();
    mFbIndex ^= 1;
    GX_CopyDisp(mFrameBuffer[mFbIndex], GX_TRUE);
    VIDEO_SetNextFramebuffer(mFrameBuffer[mFbIndex]);
    VIDEO_Flush();
    VIDEO_WaitVSync();
}

void Renderer::drawRect(f32 x, f32 y, f32 w, f32 h, u8 r, u8 g, u8 b, u8 a) {
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
    GX_Position2f32(x, y);
    GX_Color4u8(r, g, b, a);
    GX_Position2f32(x + w, y);
    GX_Color4u8(r, g, b, a);
    GX_Position2f32(x + w, y + h);
    GX_Color4u8(r, g, b, a);
    GX_Position2f32(x, y + h);
    GX_Color4u8(r, g, b, a);
    GX_End();
}

GXRModeObj* Renderer::getMode() const { return mMode; }
u32 Renderer::getWidth() const { return mMode->fbWidth; }
u32 Renderer::getHeight() const { return mMode->efbHeight; }