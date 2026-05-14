#include "ui/Label.hpp"

#include <gccore.h>

#include <cstring>

Label::Label(Font& font, f32 x, f32 y, const char* text, u8 r, u8 g, u8 b, u8 a) : mFont(font), mX(x), mY(y), mR(r), mG(g), mB(b), mA(a) {
    setText(text);
}

void Label::draw() const {
    mFont.drawText(mX, mY, mText, mR, mG, mB, mA);
}

void Label::setText(const char* text) {
    strncpy(mText, text, sizeof(mText) - 1);
    mText[sizeof(mText) - 1] = '\0';
}

void Label::setPosition(f32 x, f32 y) {
    mX = x;
    mY = y;
}

void Label::setColor(u8 r, u8 g, u8 b, u8 a) {
    mR = r;
    mG = g;
    mB = g;
    mA = a;
}

f32 Label::getWidth() const {
    return mFont.measureText(mText);
}

const char* Label::getText() const { return mText; }