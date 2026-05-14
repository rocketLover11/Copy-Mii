#include <gccore.h>
#include <wiiuse/wpad.h>

#include "gx/Renderer.hpp"
#include "ui/Font.hpp"

extern "C" {
    extern const u8 _binary_Helvetica_ttf_start[];
    extern const u32 _binary_Helvetica_ttf_size;
}

int main() {
    auto& renderer = Renderer::instance();
    renderer.init();
    WPAD_Init();

    Font font;
    font.load(_binary_Helvetica_ttf_start, (u32)&_binary_Helvetica_ttf_size);

    while (true) {
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;

        renderer.beginFrame();
        renderer.drawRect(240, 180, 160, 120, 100, 180, 255);
        font.drawText(100, 100, "Hello, Wii!", 255, 255, 255);
        renderer.endFrame();
    }

    return 0;
}