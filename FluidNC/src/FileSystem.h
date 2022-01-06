#pragma once

// This FileSystem abstraction works around some problems in the Arduino
// framework's FS class.  In particular, FS does not abstract the totalBytes()
// and usedBytes() methods, leaving them to be defined in SD and SPIFFS
// derived classes with different return types.
// The FS class in the similar framework for Espressif8266 fixes that, but
// it is unclear when if ever it will be fixed for Espressif32.

// FileSystem also adds a method to create directory listings in JSON format.

#include <SD.h>
#include <SPIFFS.h>
#include <cstdint>

#include "WebUI/JSONEncoder.h"
#include "Uart.h"

using namespace fs;
class FileSystem {
private:
    bool _isSD;
    bool _hasSubdirs = false;
    bool deleteRecursive(const String& path);

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

    bool mkdir() {
        // We would naively hope that .mkdir would fail on filesystems
        // that do not support subdirectories, but due to a subtle SPIFF
        // bug, that is not the case.  The lowest level opendir() function
        // in the ESP IDK ignores the name argument and always succeeds,
        // which causes the Arduino FS framework to think that mkdir is
        // unnecessary and successful.
        return _hasSubdirs && theFS().mkdir(_path);
    }
    bool rmdir() { return _hasSubdirs && theFS().rmdir(_path); }

    bool tailName(File& file, String& name);
    bool isDirectory(File& file);

    void listDirJSON(const char* dirname, size_t levels, JSONencoder&);
    void listJSON(const String& path, const String& status, Print& out);

    bool deleteRecursive();
};
