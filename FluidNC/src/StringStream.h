// Copyright (c) 2021 Stefan de Bruijn
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#pragma once

#include "Print.h"
#include "StringRange.h"

class StringStream : public Print {
    String _s;

public:
    StringStream() : _s("") {}

    size_t write(uint8_t c) override {
        _s.concat(char(c));
        return 1;
    }
    // String& str() { return _s; }
};
