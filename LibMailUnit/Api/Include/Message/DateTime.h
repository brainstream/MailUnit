/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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
 * @brief API for working with date and time related message headers.
 *
 * @anchor rfc-datetime-id
 * @htmlinclude RFC/DateSpec.html
*/

#ifndef __LIBMU_PUBAPI_DATETIME_H__
#define __LIBMU_PUBAPI_DATETIME_H__

#include <time.h>
#include "../Def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The month
 * @ingroup date_time
 */
typedef enum
{
    mu_month_invalid = 0,  /**< The invalid month value */
    mu_month_jan     = 1,  /**< January */
    mu_month_feb     = 2,  /**< February */
    mu_month_mar     = 3,  /**< March */
    mu_month_apr     = 4,  /**< April */
    mu_month_may     = 5,  /**< May */
    mu_month_jun     = 6,  /**< June */
    mu_month_jul     = 7,  /**< July */
    mu_month_aug     = 8,  /**< August */
    mu_month_sep     = 9,  /**< September */
    mu_month_oct     = 10, /**< October */
    mu_month_nov     = 11, /**< November */
    mu_month_dec     = 12  /**< December */
} MU_Month;

/**
 * @brief The day of week
 * @ingroup date_time
 */
typedef enum
{
    mu_dow_invalid = 0, /**< The invalid day of week value */
    mu_dow_mon     = 1, /**< Monday */
    mu_dow_tue     = 2, /**< Tuesday */
    mu_dow_wed     = 3, /**< Wednesday */
    mu_dow_thu     = 4, /**< Thursday */
    mu_dow_fri     = 5, /**< Friday */
    mu_dow_sat     = 6, /**< Saturday */
    mu_dow_sun     = 7  /**< Sunday */
} MU_DayOfWeek;

/**
 * @brief Representation of date and time.
 * @ingroup date_time
 */
typedef struct
{
    MU_DayOfWeek day_of_week;      /**< Day of week */
    unsigned short year;           /**< Year */
    MU_Month month;                /**< Mont */
    unsigned short day;            /**< Day */
    unsigned short hours;          /**< Hours */
    unsigned short minutes;        /**< Minutes */
    unsigned short seconds;        /**< Seconds */
    short timezone_offset_hours;   /**< Hours in timezone offset. Can have a negative value. */
    short timezone_offset_minutes; /**< Minutes in timezone offset. Can have a negative value. */
} MU_DateTime;

/**
 * @brief Parses string described in @ref rfc-datetime-id "RFC"
 * @param _raw_date_time
 *     String from a mail header.
 * @param _date_time
 *     A pointer to a variable that will contain a result. <i>Must not be NULL.</i>
 * @return
 *     In success case returns @ref MU_Bool::mu_true and @ref MU_Bool::mu_false otherwise.
 *     The @a _date_time object will not be modified if parsing failed.
 * @ingroup date_time
 */
MU_API MU_Bool MU_CALL muDateTimeParse(const char * _raw_date_time, MU_DateTime * _date_time);

/**
 * @brief Convert @ref MU_DateTime value to the UNIX time format
 * @param _date_time
 *     Value to convert.
 * @return
 *     The UNIX time representation of @a _date_time.
 * @sa muUnixTimeToDateTime
 * @ingroup date_time
 */
MU_API time_t MU_CALL muDateTimeToUnixTime(const MU_DateTime * _date_time);

/**
 * @brief Convert UNIX time value to @ref MU_DateTime
 * @param _unix_time
 *     Date and time in the UNIX time format.
 * @param _date_time
 *     A pointer to a variable that will contain a result. <i>Must not be NULL.</i>
 * @sa muDateTimeToUnixTime
 * @ingroup date_time
 */
MU_API void MU_CALL muUnixTimeToDateTime(time_t _unix_time, MU_DateTime * _date_time);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIBMU_PUBAPI_DATETIME_H__ */
