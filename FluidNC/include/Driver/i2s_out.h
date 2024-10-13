// Copyright (c) 2020 - Michiyasu Odaki
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "Driver/fluidnc_gpio.h"

/* Assert */
#if defined(I2S_OUT_NUM_BITS)
#    if (I2S_OUT_NUM_BITS != 16) && (I2S_OUT_NUM_BITS != 32)
#        error "I2S_OUT_NUM_BITS should be 16 or 32"
#    endif
#else
#    define I2S_OUT_NUM_BITS 32
#endif

// #    define I2SO(n) (I2S_OUT_PIN_BASE + n)

// The longest pulse that we allow when using I2S.  It is affected by the
// FIFO depth and could probably be a bit longer, but empirically this is
// enough for all known stepper drivers.
const uint32_t I2S_MAX_USEC_PER_PULSE = 20;

typedef struct {
    /*
        I2S bitstream (32-bits): Transfers from MSB(bit31) to LSB(bit0) in sequence
        ------------------time line------------------------>
             Left Channel                    Right Channel
        ws   ________________________________~~~~...
        bck  _~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~...
        data vutsrqponmlkjihgfedcba9876543210
             XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                             ^
                                Latches the X bits when ws is switched to High
        If I2S_OUT_PIN_BASE is set to 128,
        bit0:Expanded GPIO 128, 1: Expanded GPIO 129, ..., v: Expanded GPIO 159
    */
    pinnum_t ws_pin;
    pinnum_t bck_pin;
    pinnum_t data_pin;
    uint32_t pulse_period;  // aka step rate.
    uint32_t init_val;
} i2s_out_init_t;

/*
  Initialize I2S out by parameters.
  return -1 ... already initialized
*/
int i2s_out_init(i2s_out_init_t* init_param);

/*
  Read a bit state from the internal pin state var.
  pin: expanded pin No. (0..31)
*/
uint8_t i2s_out_read(pinnum_t pin);

/*
   Set a bit in the internal pin state var. (not written electrically)
   pin: expanded pin No. (0..31)
   val: bit value(0 or not 0)
*/
void i2s_out_write(pinnum_t pin, uint8_t val);

/*
   Push the I2S output value that was constructed by a series of
   i2s_out_write() calls to the I2S FIFO so it will be shifted out
   count: Number of repetitions
*/
void i2s_out_push_fifo(int count);

/*
  Dynamically delay until the Shift Register Pin changes
  according to the current I2S processing state and mode.
 */
void i2s_out_delay();

/*
   Reference: "ESP32 Technical Reference Manual" by Espressif Systems
     https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf
 */

#ifdef __cplusplus
}
#endif
