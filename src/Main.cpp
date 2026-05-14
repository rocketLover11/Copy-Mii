#include <gccore.h>
#include <ogc/conf.h>
#include <wiiuse/wpad.h>

#include "gx/Renderer.hpp"
#include "ui/Cursor.hpp"
#include "ui/Button.hpp"
#include "ui/Label.hpp"
#include "ui/Font.hpp"

extern "C" {
    extern const u8 _binary_Helvetica_ttf_start[];
    extern const u32 _binary_Helvetica_ttf_size;
}

int main() {
    auto& renderer = Renderer::instance();
    renderer.init();

    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    WPAD_SetVRes(WPAD_CHAN_0, renderer.getWidth(), renderer.getHeight());

    Font helvetica;
    helvetica.load(_binary_Helvetica_ttf_start, (u32)&_binary_Helvetica_ttf_size);

    Label title(helvetica, 100, 100, "Test", 255, 255, 255, 255);
    Label buttonTestLabel(helvetica, 500, 100, "", 255, 255, 255, 255);
    Button button(helvetica, 100, 200, 200, 100, "Button");

    Cursor cursor;

    while (true) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if (pressed & WPAD_BUTTON_HOME) break;

        cursor.update();
        button.update(cursor);

        if (button.isPressed()) buttonTestLabel.setText("Button works");

        renderer.beginFrame();
        
        float centerX = renderer.getWidth() / 2.0f;
        float centerY = renderer.getHeight() / 2.0f;
        renderer.drawRect(centerX - 80, centerY - 60, 160, 120, 100, 180, 255);

        title.draw();
        button.draw();
        buttonTestLabel.draw();

        cursor.draw();
        
        renderer.endFrame();
    }

    return 0;
}