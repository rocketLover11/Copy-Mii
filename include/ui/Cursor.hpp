#pragma once

#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <gccore.h>
#include <wiiuse/wpad.h>
#include "gx/Renderer.hpp"

class Cursor {
public:
    void update();
    void draw() const;

    f32 getX() const;
    f32 getY() const;
    bool isVisible() const;

    bool hover(f32 x, f32 y, f32 w, f32 h) const;

    bool pressed() const;
    bool released() const;

private:
    f32 mX = 0;
    f32 mY = 0;
    bool mVisible = false;
    bool mPressed = false;
    bool mReleased = false;
};

#endif