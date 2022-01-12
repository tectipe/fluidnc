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
        _fileIsDir   = _path.endsWith("/");
        auto pathLen = _path.length();
        if (_fileIsDir && pathLen > 1) {
            _path = _path.substring(0, pathLen - 1);
        }

        loc   = _path.lastIndexOf('/');
        _file = _path.substring(loc + 1);
        if (loc > 0) {
            _dir = _path.substring(0, loc);
        } else {
            _dir = "/";
        }
    } else {
        _dir  = "/";
        _file = _path.substring(1);
    }
    Uart0 << "path: " << _path << " dir: " << _dir << " file: " << _file << '\n';
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

bool FileSystem::tailName(File& file, String& name) {
    const char* fn = file.name();
    const char* tn = strrchr(fn, '/');
    if (_hasSubdirs) {
        name = tn ? (tn + 1) : fn;
        return true;
    } else {
        Uart0 << "NoSubdirs fn " << fn << " tn " << tn << " path " << _path << '\n';
        if (isDirectory(file)) {
            name = fn;
            name = name.substring(0, name.length() - 2);
            if (_path == name) {
                return false;
            }
            name = name.substring(1);
            return true;
        } else {
            name = (tn + 1);
            return true;
        }
    }
}

bool FileSystem::isDirectory(File& file) {
    if (_hasSubdirs) {
        return file.isDirectory();
    } else {
        const char* tn = strrchr(file.name(), '/');
        return tn && tn[1] == '.' && tn[2] == '\0';
    }
}

void FileSystem::listDirJSON(const char* dirname, size_t levels, JSONencoder& j) {
    File root = theFS().open(dirname);

    File file = root.openNextFile();
    j.begin_array("files");
    while (file) {
        String tail;
        bool   showFile = tailName(file, tail);
        if (showFile) {
            //        log_info(file.name() << " " << tailName);
            if (isDirectory(file) && levels) {
                j.begin_array(tail.c_str());
                listDirJSON(file.name(), levels - 1, j);
                j.end_array();
            } else {
                j.begin_object();
                j.member("name", tail.c_str());
                j.member("size", isDirectory(file) ? -1 : file.size());
                j.end_object();
            }
        }
        file = root.openNextFile();
    }
    j.end_array();
}

void FileSystem::listJSON(const String& path, const String& status, Print& out) {
    JSONencoder j(true, out);
    j.begin();
    listDirJSON(path.c_str(), 0, j);
    j.member("path", path);
    j.member("status", status);
    size_t total = totalBytes();
    size_t used  = usedBytes();
    j.member("total", formatBytes(total));
    j.member("used", formatBytes(used));
    j.member("occupation", String(100 * used / total));
    j.end();
}

bool FileSystem::deleteRecursive(const String& path) {
    bool result = true;
    File file   = theFS().open(path);
    if (!file) {
        return false;
    }
    if (!file.isDirectory()) {
        file.close();
        //return if success or not
        return theFS().remove(path);
    }
    file.rewindDirectory();
    while (true) {
        File entry = file.openNextFile();
        if (!entry) {
            break;
        }
        String entryPath = entry.name();
        if (entry.isDirectory()) {
            entry.close();
            if (!deleteRecursive(entryPath)) {
                result = false;
            }
        } else {
            entry.close();
            if (!theFS().remove(entryPath)) {
                result = false;
                break;
            }
        }
        // COMMANDS::wait(0);  //wdtFeed
    }
    file.close();
    return result ? theFS().rmdir(path) : false;
}

bool FileSystem::deleteRecursive() {
    return deleteRecursive(_path);
}
