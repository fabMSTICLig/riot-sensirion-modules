/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * I2C-Generator: 0.3.0
 * Yaml Version: 0.1.0
 * Template Version: 0.7.0-62-g3d691f9
 */
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

#include "sgp40_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_i2c_hal.h"


int16_t sgp40_measure_raw_signal(i2c_t i2c_dev, uint16_t relative_humidity,
                                 uint16_t temperature, uint16_t* sraw_voc) {
    i2c_acquire(i2c_dev);
    int16_t error;
    uint8_t buffer[8];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x260F);

    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset,
                                                  relative_humidity);
    offset =
        sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset, temperature);

    error = sensirion_i2c_write_data(i2c_dev, SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        i2c_release(i2c_dev);
        return error;
    }

    sensirion_i2c_hal_sleep_usec(30000);

    error = sensirion_i2c_read_data_inplace(i2c_dev, SGP40_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        i2c_release(i2c_dev);
        return error;
    }
    *sraw_voc = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    i2c_release(i2c_dev);
    return NO_ERROR;
}

int16_t sgp40_execute_self_test(i2c_t i2c_dev, uint16_t* test_result) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x280E);

    error = sensirion_i2c_write_data(i2c_dev, SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        i2c_release(i2c_dev);
        return error;
    }

    sensirion_i2c_hal_sleep_usec(320000);

    error = sensirion_i2c_read_data_inplace(i2c_dev, SGP40_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        i2c_release(i2c_dev);
        return error;
    }
    *test_result = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    i2c_release(i2c_dev);
    return NO_ERROR;
}

int16_t sgp40_turn_heater_off(i2c_t i2c_dev) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3615);

    error = sensirion_i2c_write_data(i2c_dev, SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        i2c_release(i2c_dev);
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    i2c_release(i2c_dev);
    return NO_ERROR;
}

int16_t sgp40_get_serial_number(i2c_t i2c_dev, uint16_t* serial_number) {
    int16_t error;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3682);

    error = sensirion_i2c_write_data(i2c_dev, SGP40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        i2c_release(i2c_dev);
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(i2c_dev, SGP40_I2C_ADDRESS, &buffer[0], 6);
    if (error) {
        i2c_release(i2c_dev);
        return error;
    }

    serial_number[0] = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    serial_number[1] = sensirion_common_bytes_to_uint16_t(&buffer[2]);
    serial_number[2] = sensirion_common_bytes_to_uint16_t(&buffer[4]);

    i2c_release(i2c_dev);
    return NO_ERROR;
}