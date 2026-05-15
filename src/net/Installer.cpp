#include "net/Installer.hpp"
#include "net/Network.hpp"

#include <gccore.h>
#include <zip.h>
#include <fat.h>
#include <dirent.h>
#include <cstring>
#include <cstdlib>
#include <malloc.h>

static constexpr u32 MAX_ZIP_SIZE = 32 * 1024 * 1024;
static constexpr char APPS_DIR[] = "SD:/apps/";

InstallResult Installer::install(const char* ip) {
    mProgress = 0.0f;

    auto& net = Network::instance();
    s32 sock = net.connectTo(ip);
    if (sock < 0) return InstallResult::ConnectFailed;

    u8* buf = nullptr;
    u32 size = 0;
    auto res = download(sock, &buf, &size);
    net.disconnect(sock);

    if (res != InstallResult::Ok) return res;

    mProgress = 0.5f;
    res = extract(buf, size);
    free(buf);

    mProgress = 1.0f;
    return res;
}

InstallResult Installer::download(s32 sock, u8** outBuf, u32* outSize) {
    u32 zipSize = 0;
    if (Network::instance().recieve(sock, &zipSize, sizeof(u32)) < 0) return InstallResult::DownloadFailed;
    if (zipSize == 0 || zipSize > MAX_ZIP_SIZE) return InstallResult::DownloadFailed;

    u8* buf = (u8*)malloc(zipSize);
    if (!buf) return InstallResult::DownloadFailed;

    u32 recieved = 0;
    while (recieved < zipSize) {
        s32 bytes = Network::instance().recieve(sock, buf + recieved, zipSize - recieved);
        if (bytes <= 0) {
            free(buf);
            return InstallResult::DownloadFailed;
        }
        recieved  += bytes;
        mProgress  = (f32)recieved / zipSize * 0.5f;
    }

    *outBuf  = buf;
    *outSize = zipSize;
    return InstallResult::Ok;
}

InstallResult Installer::extract(const u8* buf, u32 size) {
    zip_error_t  error;
    zip_source_t* src = zip_source_buffer_create(buf, size, 0, &error);
    if (!src) return InstallResult::ExtractFailed;

    zip_t* zip = zip_open_from_source(src, ZIP_RDONLY, &error);
    if (!zip) {
        zip_source_free(src);
        return InstallResult::ExtractFailed;
    }

    zip_int64_t count = zip_get_num_entries(zip, 0);
    for (zip_int64_t i = 0; i < count; i++) {
        const char* name = zip_get_name(zip, i, 0);
        if (!name) continue;

        char outPath[256];
        snprintf(outPath, sizeof(outPath), "%s%s", APPS_DIR, name);

        if (name[strlen(name) - 1] == '/') {
            mkdir(outPath, 0777);
            continue;
        }

        zip_file_t* zf = zip_fopen_index(zip, i, 0);
        if (!zf) continue;

        FILE* f = fopen(outPath, "wb");
        if (!f) {
            zip_fclose(zf);
            zip_close(zip);
            return InstallResult::WriteFailed;
        }

        u8    chunk[4096];
        zip_int64_t n;
        while ((n = zip_fread(zf, chunk, sizeof(chunk))) > 0)
            fwrite(chunk, 1, n, f);

        fclose(f);
        zip_fclose(zf);
    }

    zip_close(zip);
    return InstallResult::Ok;
}