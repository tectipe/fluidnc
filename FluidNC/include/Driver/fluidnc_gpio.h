// Copyright 2022 - Mitch Bradley
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if 0
#    include "src/Pins/PinDetail.h"  // pinnum_t
#else
typedef uint8_t pinnum_t;
#endif

// GPIO interface

void gpio_write(pinnum_t pin, int value);
int  gpio_read(pinnum_t pin);
void gpio_mode(pinnum_t pin, int input, int output, int pullup, int pulldown, int opendrain);
void gpio_set_interrupt_type(pinnum_t pin, int mode);
void gpio_add_interrupt(pinnum_t pin, int mode, void (*callback)(void*), void* arg);
void gpio_remove_interrupt(pinnum_t pin);
void gpio_route(pinnum_t pin, uint32_t signal);

typedef void (*gpio_dispatch_t)(int, void*, int);

void gpio_set_action(int gpio_num, gpio_dispatch_t action, void* arg, int invert);
void gpio_clear_action(int gpio_num);
void poll_gpios();

#ifdef __cplusplus
}
#endif
