/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>  // printf

#include "periph/i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "svm40_i2c.h"

/* TO USE CONSOLE OUTPUT (printf) YOU MAY NEED TO ADAPT THE
 * INCLUDE ABOVE OR DEFINE IT ACCORDING TO YOUR PLATFORM.
 * #define printf(...)
 */

int main(void) {
    int16_t error = 0;

    i2c_t i2c_dev = I2C_DEV(0);

    uint8_t serial_number[26];
    uint8_t serial_number_size = 26;
    error = svm40_get_serial_number(i2c_dev, &serial_number[0], serial_number_size);
    if (error) {
        printf("Error executing svm40_get_serial_number(): %i\n", error);
    } else {
        printf("Serial number: %s\n", serial_number);
    }

    uint8_t firmware_major;
    uint8_t firmware_minor;
    bool firmware_debug;
    uint8_t hardware_major;
    uint8_t hardware_minor;
    uint8_t protocol_major;
    uint8_t protocol_minor;
    error = svm40_get_version(i2c_dev, &firmware_major, &firmware_minor, &firmware_debug,
                              &hardware_major, &hardware_minor, &protocol_major,
                              &protocol_minor);
    if (error) {
        printf("Error executing svm40_get_version(): %i\n", error);
    } else {
        printf("Firmware: %i.%i Debug: %i\n", firmware_major, firmware_minor,
               firmware_debug);
        printf("Hardware: %i.%i\n", hardware_major, hardware_minor);
        printf("Protocol: %i.%i\n", protocol_major, protocol_minor);
    }

    if (firmware_major < 2) {
        printf("Your SVM40 firmware is out of date!\nPlease visit "
               "https://www.sensirion.com/my-sgp-ek/ for more information.\n");
        return -1;
    }

    int16_t t_offset;
    error = svm40_get_temperature_offset_for_rht_measurements(i2c_dev, &t_offset);
    if (error) {
        printf("Error executing "
               "svm40_get_temperature_offset_for_rht_measurements(): %i\n",
               error);
    } else {
        printf("Temperature Offset: %i ticks\n", t_offset);
    }

    // Start Measurement
    error = svm40_start_continuous_measurement(i2c_dev);
    if (error) {
        printf("Error executing svm40_start_continuous_measurement(): %i\n",
               error);
    }

    for (;;) {
        // Read Measurement
        sensirion_i2c_hal_sleep_usec(1000000);
        int16_t voc_index;
        int16_t humidity;
        int16_t temperature;
        error = svm40_read_measured_values_as_integers(i2c_dev, &voc_index, &humidity,
                                                       &temperature);
        if (error) {
            printf("Error executing svm40_read_measured_values_as_integers(): "
                   "%i\n",
                   error);
        } else {
            printf("Voc index: %i (index * 10)\n", voc_index);
            printf("Humidity: %i milli %% RH\n", humidity * 10);
            printf("Temperature: %i milli °C\n", (temperature >> 1) * 10);
        }
    }

    error = svm40_stop_measurement(i2c_dev);
    if (error) {
        printf("Error executing svm40_stop_measurement(): %i\n", error);
    }

    return 0;
}
