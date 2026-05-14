#pragma once

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <gccore.h>

#include "ui/Font.hpp"
#include "ui/Cursor.hpp"
#include "gx/Renderer.hpp"

class Button {
public:
    Button(Font& font, f32 x, f32 y, f32 w, f32 h, const char* text);

    void update(Cursor& cursor);
    void draw() const;

    bool isPressed() const;
    bool isHovered() const;

    void setPosition(f32 x, f32 y);
    void setText(const char* text);
    void setColors(u8 bgR, u8 bgG, u8 bgB, u8 hoverR, u8 hoverG, u8 hoverB, u8 textR = 255, u8 textG = 255, u8 textB = 255);

    f32 getX() const;
    f32 getY() const;
    f32 getW() const;
    f32 getH() const;

private:
    Font& mFont;

    f32 mX, mY, mW, mH;
    char mText[64];

    u8 mBgR, mBgG, mBgB;
    u8 mHoverR, mHoverG, mHoverB;
    u8 mTextR, mTextG, mTextB;

    bool mPressed = false;
    bool mHovered = false;
};

#endif