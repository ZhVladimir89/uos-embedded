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

#ifndef __DUMMY_ANALOG_OUTPUT_H__
#define __DUMMY_ANALOG_OUTPUT_H__
#include "interface.h"

// Return a dummy analog-output device
analog_output_t *get_dummy_analog_output(void);

#endif /* __DUMMY_ANALOG_OUTPUT_H__ */
