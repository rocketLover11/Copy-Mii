#include <gccore.h>
#include <ogc/conf.h>
#include <wiiuse/wpad.h>
#include <fat.h>

#include "gx/Renderer.hpp"
#include "ui/Cursor.hpp"
#include "ui/Button.hpp"
#include "ui/Label.hpp"
#include "ui/Font.hpp"
#include "ui/Keyboard.hpp"
#include "ui/InputBox.hpp"
#include "net/Network.hpp"
#include "net/Installer.hpp"

extern "C" {
    extern const u8  _binary_Helvetica_ttf_start[];
    extern const u32 _binary_Helvetica_ttf_size;
}

void setStatus(Label& label, Font& font, Renderer& renderer, const char* text,
               u8 r = 255, u8 g = 255, u8 b = 255) {
    label.setText(text);
    label.setPosition(renderer.centerX(font.measureText(text)), 60);
    label.setColor(r, g, b);
}

void downloadApp(Label& statusLabel, Font& font, Renderer& renderer, const char* ip) {
    setStatus(statusLabel, font, renderer, "Connecting...", 255, 255, 100);

    Installer installer;
    InstallResult result = installer.install(ip);

    switch (result) {
        case InstallResult::Ok:
            setStatus(statusLabel, font, renderer, "Installed successfully!", 100, 255, 100);
            break;
        case InstallResult::ConnectFailed:
            setStatus(statusLabel, font, renderer, "Connection failed.", 255, 80, 80);
            break;
        case InstallResult::DownloadFailed:
            setStatus(statusLabel, font, renderer, "Download failed.", 255, 80, 80);
            break;
        case InstallResult::ExtractFailed:
            setStatus(statusLabel, font, renderer, "Extraction failed.", 255, 80, 80);
            break;
        case InstallResult::WriteFailed:
            setStatus(statusLabel, font, renderer, "Write to SD failed.", 255, 80, 80);
            break;
    }
}

int main() {
    auto& renderer = Renderer::instance();
    renderer.init();

    fatInitDefault();

    WPAD_Init();
    WPAD_SetVRes(WPAD_CHAN_0, renderer.getWidth(), renderer.getHeight());
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    Font helvetica;
    helvetica.load(_binary_Helvetica_ttf_start, (u32)_binary_Helvetica_ttf_size);

    auto& net = Network::instance();
    net.init();

    Cursor cursor;
    Keyboard keyboard(helvetica, cursor);

    Label title(helvetica,
                renderer.centerX(helvetica.measureText("CopyMii")), 30,
                "CopyMii", 255, 255, 255, 255);
    Label exitLabel(helvetica,
                    renderer.centerX(helvetica.measureText("Press HOME to Exit")),
                    renderer.getHeight() - 25,
                    "Press HOME to Exit", 255, 255, 255, 255);
    Label statusLabel(helvetica,
                      renderer.centerX(helvetica.measureText("")), 60,
                      "", 255, 255, 255, 255);
    InputBox ipBox(helvetica, cursor, keyboard,
                   renderer.centerX(240), renderer.centerY(50), 240, 50,
                   "Enter server IP");
    Button confirmButton(helvetica,
                         renderer.centerX(240), renderer.centerY(50) + 55,
                         240, 50, "Confirm");

    while (true) {
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;

        cursor.update();
        keyboard.update();
        ipBox.update();

        if (!keyboard.isOpen()) {
            confirmButton.update(cursor);
            if (confirmButton.isPressed()) {
                const char* ip = ipBox.getValue();
                if (ip[0] != '\0') {
                    downloadApp(statusLabel, helvetica, renderer, ip);
                } else {
                    setStatus(statusLabel, helvetica, renderer,
                              "Please enter an IP.", 255, 80, 80);
                }
            }
        }

        renderer.beginFrame();

        title.draw();
        exitLabel.draw();
        statusLabel.draw();
        ipBox.draw();
        confirmButton.draw();
        
        keyboard.draw();
        cursor.draw();

        renderer.endFrame();
    }

    return 0;
}