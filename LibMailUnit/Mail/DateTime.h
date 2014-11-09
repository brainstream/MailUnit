/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Library.                                                      *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

/**
 * @file
 * @brief The date and time mail header format parser and coresponding data types.
 *
 * @anchor rfc-datetime-id
 * @htmlinclude RFC/DateSpec.html
*/

#include "../Def.h"

/**
 * @brief The month
 */
typedef enum
{
    mmonth_invalid = 0,  /**< The invalid month value */
    mmonth_jan     = 1,  /**< January */
    mmonth_feb     = 2,  /**< February */
    mmonth_mar     = 3,  /**< March */
    mmonth_apr     = 4,  /**< April */
    mmonth_may     = 5,  /**< May */
    mmonth_jun     = 6,  /**< June */
    mmonth_jul     = 7,  /**< July */
    mmonth_aug     = 8,  /**< August */
    mmonth_sep     = 9,  /**< September */
    mmonth_oct     = 10, /**< October */
    mmonth_nov     = 11, /**< November */
    mmonth_dec     = 12  /**< December */
} MMonth;

/**
 * @brief The day of week
 */
typedef enum
{
    mdow_invalid = 0, /**< The invalid day of week value */
    mdow_mon     = 1, /**< Monday */
    mdow_tue     = 2, /**< Tuesday */
    mdow_wed     = 3, /**< Wednesday */
    mdow_thu     = 4, /**< Thursday */
    mdow_fri     = 5, /**< Friday */
    mdow_sat     = 6, /**< Saturday */
    mdow_sun     = 7  /**< Sunday */
} MDayOfWeek;

/**
 * @brief Representation of date and time.
 */
typedef struct
{
    MDayOfWeek day_of_week;        /**< Day of week */
    unsigned short year;           /**< Year */
    MMonth month;                  /**< Mont */
    unsigned short day;            /**< Day */
    unsigned short hours;          /**< Hours */
    unsigned short minutes;        /**< Minutes */
    unsigned short seconds;        /**< Seconds */
    short timezone_offset_hours;   /**< Hours in timezone offset. Can have a negative value. */
    short timezone_offset_minutes; /**< Minutes in timezone offset. Can have a negative value. */
} MDateTime;

/**
 * @brief Parses string described in @ref rfc-datetime-id "RFC"
 * @param _raw_date_time
 *     String from a mail header.
 * @param _date_time
 *     Pointer to the object to hold a result of parsing. <i>Must not be a NULL.</i>
 * @return
 *     In success case returns @ref MBool::mtrue and @ref MBool::mfalse otherwise.
 *     The @a _date_time object will not be modified if parsing failed.
 */
MUAPI MBool muDateTimeParse(const char * _raw_date_time, MDateTime * _date_time);
