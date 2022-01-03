#include "FileSystem.h"
#include "SDCard.h"
#include "Machine/MachineConfig.h"  // config
#include "Uart.h"                   // Uart0

#if 0
FileSystem::FileSystem(const String& fs, String& dirname, String& filename) _tail(dirname + "/" + filename), _dirname(dirname), _filename(filename) {
    _isSD = fs == "sd";
    if (_isSD) {
        SDCard::State state = config->_sdCard->begin(SDCard::State::Busy);
        if (state != SDCard::State::Idle) {
            _isSD = false;
            throw state;
        }
    }
}
FileSystem::FileSystem(String& path) {
}
#endif
FileSystem::FileSystem(const String& fullPath) {
    _isSD = fullPath.startsWith("/sd");
    if (_isSD) {
        SDCard::State state = config->_sdCard->begin(SDCard::State::Busy);
        if (state != SDCard::State::Idle) {
            _isSD = false;
            throw state;
        }
        _hasSubdirs = true;
    }
    auto loc = fullPath.indexOf('/', 1);
    _path    = fullPath.substring(loc);

    if (_hasSubdirs) {
        _dir = _path;
        loc  = _dir.lastIndexOf('/');
        if (loc > 0) {
            _dir = _dir.substring(0, loc);
        } else {
            _dir = "/";
        }

        _file = _path;
        loc   = _file.lastIndexOf('/');
        _file = _file.substring(loc + 1);
    } else {
        _dir  = "/";
        _file = _path.substring(1);
    }
    Uart0 << "path " << _path << " dir " << _dir << " file " << _file << '\n';
}

FileSystem::~FileSystem() {
    if (_isSD) {
        config->_sdCard->end();
    }
}

FS& FileSystem::theFS() {
    return _isSD ? (FS&)SD : (FS&)SPIFFS;
}

// The Arduino FS class does not include totalBytes() and usedBytes() methods;
// they are in the derived classes SPIFFS and SD, with different return types.

uint64_t FileSystem::totalBytes() {
    return _isSD ? SD.totalBytes() : (uint64_t)SPIFFS.totalBytes();
}
uint64_t FileSystem::usedBytes() {
    return _isSD ? SD.usedBytes() : (uint64_t)SPIFFS.usedBytes();
}

void FileSystem::listDirJSON(const char* dirname, size_t levels, JSONencoder& j) {
    File root = theFS().open(dirname);

    File file = root.openNextFile();
    j.begin_array("files");
    while (file) {
        const char* tailName = strrchr(file.name(), '/');
        tailName             = tailName ? tailName + 1 : file.name();
        log_info(file.name() << " " << tailName);
        if (file.isDirectory() && levels) {
            j.begin_array(tailName);
            listDirJSON(file.name(), levels - 1, j);
            j.end_array();
        } else {
            j.begin_object();
            j.member("name", tailName);
            j.member("size", file.isDirectory() ? -1 : file.size());
            j.end_object();
        }
        file = root.openNextFile();
    }
    j.end_array();
}

void FileSystem::listJSON(const String& path, Print& out) {
    JSONencoder j(false, out);
    j.begin();
    listDirJSON(path.c_str(), 0, j);
    j.member("path", path);
    j.member("status", "Ok");
    size_t total = totalBytes();
    size_t used  = usedBytes();
    j.member("total", formatBytes(total));
    j.member("used", formatBytes(used));
    j.member("occupation", String(100 * used / total));
    j.end();
}
