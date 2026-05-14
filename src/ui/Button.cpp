#include "ui/Button.hpp"
#include "ui/Font.hpp"
#include "ui/Cursor.hpp"
#include "gx/Renderer.hpp"

#include <gccore.h>

#include <cstring>

Button::Button(Font& font, f32 x, f32 y, f32 w, f32 h, const char* text) : mFont(font), mX(x), mY(y), mW(w), mH(h), mBgR(0), mBgG(0), mBgB(0), mHoverR(100), mHoverG(100), mHoverB(100), mTextR(255), mTextG(255), mTextB(255) {
    setText(text);
}

void Button::setPosition(f32 x, f32 y) {
    mX = x; mY = y;
}

void Button::setText(const char* text) {
    strncpy(mText, text, sizeof(mText) - 1);
    mText[sizeof(mText) - 1] = '\0';
}

void Button::setColors(u8 bgR, u8 bgG, u8 bgB, u8 hoverR, u8 hoverG, u8 hoverB, u8 textR, u8 textG, u8 textB) {
    mBgR = bgR;
    mBgG = bgG;
    mBgB = bgB;
    mHoverR = hoverR;
    mHoverG = hoverG;
    mHoverB = hoverB;
    mTextR = textR;
    mTextG = textG;
    mTextB = textB;
}

bool Button::isPressed() const { return mPressed; }
bool Button::isHovered() const { return mHovered; }

void Button::update(Cursor& cursor) {
    mHovered = cursor.hover(mX, mY, mW, mH);
    mPressed = mHovered && cursor.released();
}

void Button::draw() const {
    auto& r = Renderer::instance();

    if (mHovered) {
        r.drawRect(mX, mY, mW, mH, mHoverR, mHoverG, mHoverB, 255);
    } else {
        r.drawRect(mX, mY, mW, mH, mBgR, mBgG, mBgB, 255);
    }

    r.drawRect(mX, mY, mW, 2, 180, 180, 220, 255);
    r.drawRect(mX, mY, 2, mH, 180, 180, 220, 255);
    r.drawRect(mX, mY + mH - 2, mW, 2, 180, 180, 220, 255);
    r.drawRect(mX + mW - 2, mY, 2, mH, 180, 180, 220, 255);

    f32 textW = mFont.measureText(mText);
    f32 textX = mX + (mW - textW) / 2.0f;
    f32 textY = mY + mH / 2.0f + 8;
    mFont.drawText(textX, textY, mText, mTextR, mTextG, mTextB);
}

f32 Button::getX() const { return mX; }
f32 Button::getY() const { return mY; }
f32 Button::getW() const { return mW; }
f32 Button::getH() const { return mH; }

char* Button::getText() { return (char*)mText; }