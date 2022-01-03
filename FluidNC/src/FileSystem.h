#pragma once

#include <SD.h>
#include <SPIFFS.h>
#include <cstdint>

#include "WebUI/JSONEncoder.h"

using namespace fs;
class FileSystem {
private:
    bool _isSD;
    bool _hasSubdirs = false;

public:
    FileSystem(const String& path);
    ~FileSystem();

    FS& theFS();

    String _path;
    String _dir;
    String _file;

    uint64_t totalBytes();
    uint64_t usedBytes();

    bool exists() { return theFS().exists(_path); }
    bool remove() { return theFS().remove(_path); }

    //    bool rename(const char* pathFrom, const char* pathTo) { return theFS().rename(pathFrom, pathTo); }
    //    bool rename(const String& pathFrom, const String& pathTo) { return theFS().rename(pathFrom, pathTo); }

    bool mkdir() { return theFS().mkdir(_path); }
    bool rmdir() { return theFS().rmdir(_path); }

    void listDirJSON(const char* dirname, size_t levels, JSONencoder&);
    void listJSON(const String& path, Print& out);
};
