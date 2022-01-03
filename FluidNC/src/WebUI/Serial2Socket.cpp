// Copyright (c) 2014 Luc Lebosse. All rights reserved.
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Serial2Socket.h"

namespace WebUI {
    Serial_2_Socket Serial2Socket;
}

#ifdef ENABLE_WIFI
#    include "WebServer.h"
#    include <WebSocketsServer.h>
#    include <WiFi.h>

namespace WebUI {
    Serial_2_Socket::Serial_2_Socket() : Channel("websocket"), _web_socket(nullptr), _TXbufferSize(0), _rxbuf("") {}

    void Serial_2_Socket::begin(long speed) {
        _TXbufferSize = 0;
        _rxbuf        = "";
    }

    void Serial_2_Socket::end() { _TXbufferSize = 0; }

    long Serial_2_Socket::baudRate() { return 0; }

    bool Serial_2_Socket::attachWS(WebSocketsServer* web_socket) {
        if (web_socket) {
            _web_socket   = web_socket;
            _TXbufferSize = 0;
            return true;
        }
        return false;
    }

    bool Serial_2_Socket::detachWS() {
        _web_socket = NULL;
        return true;
    }

    Serial_2_Socket::operator bool() const { return true; }

    int Serial_2_Socket::available() { return _rxbuf.length(); }

    size_t Serial_2_Socket::write(uint8_t c) {
        if (!_web_socket) {
            return 0;
        }
        write(&c, 1);
        return 1;
    }

    size_t Serial_2_Socket::write(const uint8_t* buffer, size_t size) {
        if ((buffer == NULL) || (!_web_socket)) {
            if (buffer == NULL) {
                //                log_i("[SOCKET]No buffer");
            }
            if (!_web_socket) {
                //                log_i("[SOCKET]No socket");
            }
            return 0;
        }

        if (_TXbufferSize == 0) {
            _lastflush = millis();
        }

        //send full line
        if (_TXbufferSize + size > TXBUFFERSIZE) {
            flush();
        }

        //need periodic check to force to flush in case of no end
        for (int i = 0; i < size; i++) {
            _TXbuffer[_TXbufferSize] = buffer[i];
            _TXbufferSize++;
        }
        //        log_i("[SOCKET]buffer size %d", _TXbufferSize);
        handle_flush();
        return size;
    }

    int Serial_2_Socket::peek(void) {
        if (_rxbuf.length() > 0) {
            return _rxbuf[0];
        } else {
            return -1;
        }
    }

    bool Serial_2_Socket::push(String s) {
        if ((s.length() + _rxbuf.length()) > RXBUFFERSIZE) {
            return false;
        }
        _rxbuf += s;
        return true;
    }
    bool Serial_2_Socket::push(const char* s) {
        if ((strlen(s) + _rxbuf.length()) > RXBUFFERSIZE) {
            return false;
        }
        _rxbuf.concat(s);
        return true;
    }
    bool Serial_2_Socket::push(const uint8_t* data, size_t length) {
        if ((length + _rxbuf.length()) > RXBUFFERSIZE) {
            return false;
        }
        while (length--) {
            _rxbuf.concat((char)(*data++));
        }
        return true;
    }

    int Serial_2_Socket::read(void) {
        handle_flush();
        if (_rxbuf.length()) {
            char v = _rxbuf[0];
            _rxbuf.remove(0, 1);
            return (int)v;
        }
        return -1;
    }

    void Serial_2_Socket::handle_flush() {
        if (_TXbufferSize > 0 && ((_TXbufferSize >= TXBUFFERSIZE) || ((millis() - _lastflush) > FLUSHTIMEOUT))) {
            //            log_i("[SOCKET]need flush, buffer size %d", _TXbufferSize);
            flush();
        }
    }
    void Serial_2_Socket::flush(void) {
        if (_TXbufferSize > 0) {
            //            log_i("[SOCKET]flush data, buffer size %d", _TXbufferSize);
            _web_socket->broadcastBIN(_TXbuffer, _TXbufferSize);

            //refresh timout
            _lastflush = millis();

            //reset buffer
            _TXbufferSize = 0;
        }
    }

    Serial_2_Socket::~Serial_2_Socket() {
        if (_web_socket) {
            detachWS();
        }
        _TXbufferSize = 0;
    }
}
#endif
