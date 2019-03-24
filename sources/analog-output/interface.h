/*
 * Analog-output interface for UOS.
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

#ifndef __ANALOG_OUTPUT_INTERFACE_H__
#define __ANALOG_OUTPUT_INTERFACE_H__

enum
{
    ERR_AOUT_FAULT            =  0,
    ERR_AOUT_SUCCES           =  1,
    ERR_AOUT_INCORRECT_VALUE  = -1,  // неверное значение
    ERR_AOUT_BAD_VALUE        = -2   // плохое значение (например некорректный указатель)
};

typedef unsigned char analog_data_t;
typedef struct _analog_output_t analog_output_t;

struct _analog_output_t
{
    void * device_data; // Указатель на данные, определяемые низкоуровневым,
                        // платформозависимым драйвером
                        // и необходимы исключительно для его собственной работы.
    int (* write)(analog_output_t* output, analog_data_t value);
    int (* read)(analog_output_t* output, analog_data_t *value);
};

#define  ANALOG_WRITE(OUT,VAL) OUT.write(&(OUT),(VAL))
#define pANALOG_WRITE(OUT,VAL) OUT->write((OUT),(VAL))

#endif /* __ANALOG_OUTPUT_INTERFACE_H__ */
