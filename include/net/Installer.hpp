#pragma once

#ifndef INSTALLER_HPP
#define INSTALLER_HPP

#include <gccore.h>

enum class InstallResult {
    Ok,
    ConnectFailed,
    DownloadFailed,
    ExtractFailed,
    WriteFailed
};

class Installer {
public:
    InstallResult install(const char* ip);
    f32 getProgress() const;

private:
    InstallResult download(s32 sock, u8** outBuf, u32* outsize);
    InstallResult extract(const u8* buf, u32 size);

    f32 mProgress = 0.0f;
};

#endif