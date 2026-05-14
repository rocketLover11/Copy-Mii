#include "ui/InputBox.hpp"
#include "ui/Font.hpp"
#include "ui/Cursor.hpp"
#include "ui/Keyboard.hpp"
#include "gx/Renderer.hpp"

#include <gccore.h>

#include <cstring>

InputBox::InputBox(Font& font, Cursor& cursor, Keyboard& keyboard, f32 x, f32 y, f32 w, f32 h, const char* placeholder) : mFont(font), mCursor(cursor), mKeyboard(keyboard), mX(x), mY(y), mW(w), mH(h) {
    mBuffer[0] = '\0';
    strncpy(mPlaceholder, placeholder, sizeof(mPlaceholder) - 1);
    mPlaceholder[sizeof(mPlaceholder) - 1] = '\0';
}

void InputBox::setValue(const char* value) {
    strncpy(mBuffer, value, sizeof(mBuffer) - 1);
    mBuffer[sizeof(mBuffer) - 1] = '\0';
}

void InputBox::update() {
    if (mCursor.hover(mX, mY, mW, mH) && mCursor.released()) {
        mFocused = true;
        mKeyboard.open(mBuffer, sizeof(mBuffer));
    }

    if (mFocused && mKeyboard.isDone()) {
        mFocused = false;
    }
}

void InputBox::draw() const {
    auto& r = Renderer::instance();

    r.drawRect(mX, mY, mW, mH, 40, 40, 70, 255);

    u8 br = mFocused ? 100 : 180;
    u8 bg = mFocused ? 180 : 180;
    u8 bb = mFocused ? 255 : 220;
    r.drawRect(mX, mY, mW, 2, br, bg, bb, 255);
    r.drawRect(mX, mY, 2, mH, br, bg, bb, 255);
    r.drawRect(mX, mY + mH - 2, mW, 2, br, bg, bb, 255);
    r.drawRect(mX + mW - 2, mY, 2, mH, br, bg, bb, 255);

    f32 textY = mY + mH / 2.0f + 8;
    if (mBuffer[0] != '\0') {
        mFont.drawText(mX + 10, textY, mBuffer, 255, 255, 255);
    } else {
        mFont.drawText(mX + 10, textY, mPlaceholder, 120, 120, 140);
    }

    if (mFocused) {
        f32 textW = mFont.measureText(mBuffer);
        r.drawRect(mX + 10 + textW + 2, mY + 8, 2, mH - 16, 255, 255, 255, 255);
    }
}

const char* InputBox::getValue() const { return (char*)mBuffer; }
bool InputBox::isFocused() const { return mFocused; }