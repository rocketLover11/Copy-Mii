#include <gccore.h>
#include <ogc/conf.h>
#include <wiiuse/wpad.h>

#include "gx/Renderer.hpp"
#include "ui/Cursor.hpp"
#include "ui/Button.hpp"
#include "ui/Label.hpp"
#include "ui/Font.hpp"
#include "ui/Keyboard.hpp"
#include "ui/InputBox.hpp"

extern "C" {
    extern const u8 _binary_Helvetica_ttf_start[];
    extern const u32 _binary_Helvetica_ttf_size;
}

int main() {
    auto& renderer = Renderer::instance();
    renderer.init();

    WPAD_Init();
    WPAD_SetVRes(WPAD_CHAN_0, renderer.getWidth(), renderer.getHeight());
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    Font helvetica;
    helvetica.load(_binary_Helvetica_ttf_start, (u32)&_binary_Helvetica_ttf_size);

    Cursor cursor;
    Keyboard keyboard(helvetica, cursor);

    Label title(helvetica, renderer.centerX(helvetica.measureText("CopyMii")), 25, "CopyMii", 255, 255, 255, 255);
    Label exitLabel(helvetica, renderer.centerX(helvetica.measureText("Press HOME to Exit")), renderer.getHeight() - 25, "Press HOME to Exit", 255, 255, 255, 255);
    InputBox ipBox(helvetica, cursor, keyboard, renderer.centerX(240), renderer.centerY(50), 240, 50, "Enter server IP");
    Button confirmButton(helvetica, renderer.centerX(240), renderer.centerY(50) + 55, 240, 50, "Confirm");

    while (true) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if (pressed & WPAD_BUTTON_HOME) break;

        cursor.update();
        keyboard.update();
        ipBox.update();

        if (!keyboard.isOpen()) {
            confirmButton.update(cursor);
        }

        renderer.beginFrame();

        title.draw();
        exitLabel.draw();
        ipBox.draw();
        confirmButton.draw();
        
        keyboard.draw();
        cursor.draw();
        
        renderer.endFrame();
    }

    return 0;
}