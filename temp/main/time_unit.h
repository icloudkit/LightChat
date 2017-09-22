/*
 * Copyright (C) 2015 The CloudKit Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// 2017/5/31.
//

#ifndef LIGHTCHAT_TIME_UNIT_H
#define LIGHTCHAT_TIME_UNIT_H

/**
 * NANOSECONDS Time unit representing one thousandth of a microsecond
 * MICROSECONDS Time unit representing one thousandth of a millisecond
 * MILLISECONDS Time unit representing one thousandth of a second
 * SECONDS Time unit representing one second
 * MINUTES Time unit representing sixty seconds
 * HOURS Time unit representing sixty minutes
 * DAYS Time unit representing twenty four hours
 */
typedef enum {
    NANOSECONDS,
    MICROSECONDS,
    MILLISECONDS,
    SECONDS,
    MINUTES,
    HOURS,
    DAYS
} time_unit;


#endif //LIGHTCHAT_TIME_UNIT_H



