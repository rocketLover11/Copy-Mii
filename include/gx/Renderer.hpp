#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <gccore.h>
#include <malloc.h>
#include <cstring>

class Renderer {
public:
    static Renderer& instance();
    
    void init();
    void beginFrame();
    void endFrame();

    void drawRect(f32 x, f32 y, f32 w, f32 h, u8 r, u8 g, u8 b, u8 a = 255);

    GXRModeObj* getMode() const;
    u32 getWidth() const;  // This will now return 640 or 854
    u32 getHeight() const;
    bool isWidescreen() const;

private:
    Renderer() = default;

    void initVideo();
    void initGX();

    static constexpr u32 FIFO_SIZE = 256 * 1024;

    GXRModeObj* mMode = nullptr;
    void* mFrameBuffer[2] = { nullptr, nullptr };
    void* mFifoBuffer = nullptr;
    u32 mFbIndex = 0;

    u32 mLogicalWidth = 640;
    bool mIsWidescreen = false;
};

#endif // RENDERER_HPP