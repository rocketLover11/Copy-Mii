#include "ui/Cursor.hpp"

#include <gccore.h>
#include <wiiuse/wpad.h>
#include "gx/Renderer.hpp"

void Cursor::update() {
    WPADData* data = WPAD_Data(0);

    if (data->ir.valid) {
        mVisible = true;
        mX = data->ir.x;
        mY = data->ir.y;
        mPressed = WPAD_ButtonsDown(0) & WPAD_BUTTON_A;
        mReleased = WPAD_ButtonsUp(0) & WPAD_BUTTON_A;
    } else {
        mVisible = false;
        mPressed = false;
        mReleased = false;
    }
}

void Cursor::draw() const {
    if (!mVisible) return;

    auto& r = Renderer::instance();

    r.drawRect(mX - 10, mY - 10, 20, 20, 255, 255, 255, 80);
    r.drawRect(mX - 3, mY - 3, 6, 6, 255, 255, 255, 255);
}

f32 Cursor::getX() const { return mX; }
f32 Cursor::getY() const { return mY; }
bool Cursor::isVisible() const { return mVisible; }

bool Cursor::hover(f32 x, f32 y, f32 w, f32 h) const { return mVisible && mX >= x && mX <= x + w && mY >= y && mY <= y + h; }

bool Cursor::pressed() const { return mPressed; }
bool Cursor::released() const { return mReleased; }