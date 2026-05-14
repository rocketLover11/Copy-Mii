#pragma once

#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include "ui/Font.hpp"
#include "ui/Cursor.hpp"
#include "ui/Keyboard.hpp"
#include "gx/Renderer.hpp"

#include <gccore.h>

class InputBox {
public:
    InputBox(Font& font, Cursor& cursor, Keyboard& keyboard, f32 x, f32 y, f32 w, f32 h, const char* placeholder = "");

    void update();
    void draw() const;

    const char* getValue() const;
    void setValue(const char* value);

    bool isFocused() const;

private:
    Font& mFont;
    Cursor& mCursor;
    Keyboard& mKeyboard;

    f32 mX, mY, mW, mH;
    char mBuffer[16];
    char mPlaceholder[64];

    bool mFocused = false;
};

#endif