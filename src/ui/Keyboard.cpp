#include "ui/Keyboard.hpp"
#include "gx/Renderer.hpp"

#include <gccore.h>

#include <cstring>
#include <vector>

static const char KEY_CHARS[] = "123456789.0";
static constexpr u32 KEY_COUNT = 11;

Keyboard::Keyboard(Font& font, Cursor& cursor) : mFont(font), mCursor(cursor), mBackspace(font, 0, 0, KEY_W * 2, KEY_H, "DEL"), mConfirm(font, 0, 0, KEY_W * 2, KEY_H, "OK"), mInputLabel(font, 0, 0, "") {
    mBackspace.setColors(30, 60, 60, 10, 40, 40, 255, 255, 255);
    mConfirm.setColors(30, 60, 60, 10, 40, 40, 255, 255, 255);

    buildKeys();
}

void Keyboard::buildKeys() {
    auto& r = Renderer::instance();

    u32 rows = (KEY_COUNT + KEY_COLS - 1) / KEY_COLS;
    f32 totalW = KEY_COLS * (KEY_W + KEY_PADDING) - KEY_PADDING;
    f32 startX = (r.getWidth() - totalW) / 2.0f;
    f32 startY = (r.getHeight() - (rows * (KEY_H + KEY_PADDING))) / 2.0f + 40;

    mKeys.clear();
    for (u32 i = 0; i < KEY_COUNT; i++) {
        char text[2] = { KEY_CHARS[i], '\0' };
        u32 col = i % KEY_COLS;
        u32 row = i / KEY_COLS;
        f32 x = startX + col * (KEY_W + KEY_PADDING);
        f32 y = startY + row * (KEY_H + KEY_PADDING);
        mKeys.emplace_back(mFont, x, y, KEY_W, KEY_H, text);
    }

    f32 bottomY = startY + rows * (KEY_H + KEY_PADDING);
    mBackspace.setPosition(startX, bottomY);
    mConfirm.setPosition(startX + totalW - KEY_W * 2, bottomY);

    f32 panelX = startX - KEY_PADDING * 2;
    f32 panelY = startY - KEY_H - KEY_PADDING * 2;
    mInputLabel.setPosition(panelX + 10, panelY + KEY_H / 2.0f + 8);
}

void Keyboard::open(char* buffer, u32 maxLen) {
    mBuffer = buffer;
    mMaxLen = maxLen;
    mLength = strlen(buffer);
    mOpen = true;
    mDone = false;
    mInputLabel.setText(buffer);
}

void Keyboard::close() {
    mOpen = false;
}

void Keyboard::pressKey(char c) {
    if (mLength < mMaxLen - 1) {
        mBuffer[mLength++] = c;
        mBuffer[mLength] = '\0';
        mInputLabel.setText(mBuffer);
    }
}

void Keyboard::update() {
    if (!mOpen) return;

    for (auto& key : mKeys) {
        key.update(mCursor);
        if (key.isPressed()) {
            pressKey(key.getText()[0]);
            return;
        }
    }

    mBackspace.update(mCursor);
    if (mBackspace.isPressed()) {
        if (mLength > 0) {
            mBuffer[--mLength] = '\0';
            mInputLabel.setText(mBuffer);
        }
        return;
    }

    mConfirm.update(mCursor);
    if (mConfirm.isPressed()) {
        mDone = true;
        mOpen = false;
    }
}

void Keyboard::draw() const {
    if (!mOpen) return;

    auto& r = Renderer::instance();
    auto& k0 = mKeys[0];
    auto& klast = mKeys.back();

    f32 totalW = KEY_COLS * (KEY_W + KEY_PADDING) - KEY_PADDING;
    f32 panelX = k0.getX() - KEY_PADDING * 2;
    f32 panelY = k0.getY() - KEY_H - KEY_PADDING * 3;
    f32 panelH = (klast.getY() + KEY_H) - k0.getY() + KEY_H + KEY_PADDING * 2 + KEY_H + KEY_PADDING * 4;

    r.drawRect(panelX, panelY, totalW + KEY_PADDING * 4, panelH, 20, 20, 40, 230);
    r.drawRect(panelX + KEY_PADDING, panelY + KEY_PADDING, totalW + KEY_PADDING * 2, KEY_H, 40, 40, 70, 255);

    mInputLabel.draw();

    for (const auto& key : mKeys) key.draw();

    mBackspace.draw();
    mConfirm.draw();
}

bool Keyboard::isOpen() const { return mOpen; }
bool Keyboard::isDone() const { return mDone; }