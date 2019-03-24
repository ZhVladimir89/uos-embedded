/*
 * DUMMY analog output device for UOS.
 *
 * Copyright (C) 2019 Vladimir Zhuravlev, <vladimir.rgrtu@mail.ru>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "COPYING.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "COPY-UOS.txt" for details.
 */

#include <runtime/lib.h>
#include <kernel/uos.h>
#include "dummy-analog-output.h"

static mutex_t _mutex;
static analog_data_t dummy_analog_value;

static int dummy_write(analog_output_t* output, analog_data_t value)
{
    mutex_lock(&_mutex);
//  *((analog_data_t*)output->device_data) = value;
    dummy_analog_value = value; // one for all threads
    mutex_unlock(&_mutex);
    return ERR_AOUT_SUCCES;
}

static int dummy_read(analog_output_t* output, analog_data_t *value)
{
    mutex_lock(&_mutex);
//  *value = *((analog_data_t*)output->device_data);
    *value = dummy_analog_value; // one for all threads
    mutex_unlock(&_mutex);
    return ERR_AOUT_SUCCES;
}

static analog_output_t dummy_out =
{
    .device_data = &dummy_analog_value,
    .write       = dummy_write,
    .read        = dummy_read
};

analog_output_t *get_dummy_analog_output(void)
{
    return &dummy_out;
}
