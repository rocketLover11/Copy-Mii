#include <gccore.h>
#include <wiiuse/wpad.h>
#include "gx/Renderer.hpp"

int main() {
    auto& renderer = Renderer::instance();
    renderer.init();
    WPAD_Init();

    while (true) {
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;

        renderer.beginFrame();
        renderer.drawRect(240, 180, 160, 120, 100, 180, 255);
        renderer.endFrame();
    }

    return 0;
}