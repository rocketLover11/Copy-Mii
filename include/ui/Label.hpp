#pragma once

#ifndef LABEL_HPP
#define LABEL_HPP

#include <gccore.h>

#include "Font.hpp"

class Label {
public:
    Label(Font& font, f32 x, f32 y, const char* text, u8 r = 255, u8 g = 255, u8 b = 255, u8 a = 255);

    void draw() const;

    void setText(const char* text);
    void setPosition(f32 x, f32 y);
    void setColor(u8 r, u8 g, u8 b, u8 a = 255);

    f32 getWidth() const;
    const char* getText() const;

private:
    Font& mFont;
    f32 mX, mY;
    u8 mR, mG, mB, mA;
    char mText[128];
};

#endif // LABEL_HPP