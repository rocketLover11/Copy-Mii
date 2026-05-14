#pragma once

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "ui/Button.hpp"
#include "ui/Font.hpp"
#include "ui/Cursor.hpp"
#include "ui/Label.hpp"

#include <gccore.h>

#include <vector>

class Keyboard {
public:
    static constexpr u32 MAX_LENGTH = 15;
    static constexpr f32 KEY_W = 60.0f;
    static constexpr f32 KEY_H = 60.0f;
    static constexpr f32 KEY_PADDING = 8.0f;
    static constexpr u32 KEY_COLS = 6;

    Keyboard(Font& font, Cursor& cursor);

    void open(char* buffer, u32 maxLen = MAX_LENGTH);
    void close();

    void update();
    void draw() const;

    bool isOpen() const;
    bool isDone() const;

private:
    void buildKeys();
    void pressKey(char c);

    Font& mFont;
    Cursor& mCursor;

    std::vector<Button> mKeys;
    Button mBackspace;
    Button mConfirm;
    Label mInputLabel;

    char* mBuffer = nullptr;
    u32 mMaxLen = 0;
    u32 mLength = 0;

    bool mOpen = false;
    bool mDone = false;
};

#endif