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

#include <boost/test/unit_test.hpp>
#include <Include/LibMailUnit/Mail.h>

namespace LibMailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(DateTime)

BOOST_AUTO_TEST_CASE(parse)
{
    const char valid_raw_dow[]             = "Sat, 12 Jul 2014 20:11:15 -0430";
    const char valid_raw_dow_without_sec[] = " Sat, 12 Jul 2014 20:11 +0430";
    const char valid_raw_dow_without_dow[] = "12 Jul 2014 20:11:15 -0430 ";
    const char invalid_raw_dow_1[]         = "Sat, 12 Jul 2014 20:11:15";
    const char invalid_raw_dow_2[]         = "Sat, 12 Jul 2014 20 -0430";
    const char invalid_raw_dow_3[]         = "Sat, 12 2014 20:11:15 -0430";
    const char invalid_raw_dow_4[]         = "Sat, Jul 2014 20:11:15 -0430";

    MDateTime date_time;
    memset(&date_time, 0, sizeof(MDateTime));
    BOOST_CHECK_EQUAL(mtrue, muDateTimeParse(valid_raw_dow, &date_time));
    BOOST_CHECK_EQUAL(mdow_sat, date_time.day_of_week);
    BOOST_CHECK_EQUAL(12, date_time.day);
    BOOST_CHECK_EQUAL(mmonth_jul, date_time.month);
    BOOST_CHECK_EQUAL(2014, date_time.year);
    BOOST_CHECK_EQUAL(20, date_time.hours);
    BOOST_CHECK_EQUAL(11, date_time.minutes);
    BOOST_CHECK_EQUAL(15, date_time.seconds);
    BOOST_CHECK_EQUAL(-4, date_time.timezone_offset_hours);
    BOOST_CHECK_EQUAL(-30, date_time.timezone_offset_minutes);

    memset(&date_time, 0, sizeof(MDateTime));
    BOOST_CHECK_EQUAL(mtrue, muDateTimeParse(valid_raw_dow_without_sec, &date_time));
    BOOST_CHECK_EQUAL(mdow_sat, date_time.day_of_week);
    BOOST_CHECK_EQUAL(12, date_time.day);
    BOOST_CHECK_EQUAL(mmonth_jul, date_time.month);
    BOOST_CHECK_EQUAL(2014, date_time.year);
    BOOST_CHECK_EQUAL(20, date_time.hours);
    BOOST_CHECK_EQUAL(11, date_time.minutes);
    BOOST_CHECK_EQUAL(0, date_time.seconds);
    BOOST_CHECK_EQUAL(4, date_time.timezone_offset_hours);
    BOOST_CHECK_EQUAL(30, date_time.timezone_offset_minutes);

    memset(&date_time, 0, sizeof(MDateTime));
    BOOST_CHECK_EQUAL(mtrue, muDateTimeParse(valid_raw_dow_without_dow, &date_time));
    BOOST_CHECK_EQUAL(mdow_invalid, date_time.day_of_week);
    BOOST_CHECK_EQUAL(12, date_time.day);
    BOOST_CHECK_EQUAL(mmonth_jul, date_time.month);
    BOOST_CHECK_EQUAL(2014, date_time.year);
    BOOST_CHECK_EQUAL(20, date_time.hours);
    BOOST_CHECK_EQUAL(11, date_time.minutes);
    BOOST_CHECK_EQUAL(15, date_time.seconds);
    BOOST_CHECK_EQUAL(-4, date_time.timezone_offset_hours);
    BOOST_CHECK_EQUAL(-30, date_time.timezone_offset_minutes);

    BOOST_CHECK_EQUAL(mfalse, muDateTimeParse(valid_raw_dow, nullptr));
    BOOST_CHECK_EQUAL(mfalse, muDateTimeParse(invalid_raw_dow_1, &date_time));
    BOOST_CHECK_EQUAL(mfalse, muDateTimeParse(invalid_raw_dow_2, &date_time));
    BOOST_CHECK_EQUAL(mfalse, muDateTimeParse(invalid_raw_dow_3, &date_time));
    BOOST_CHECK_EQUAL(mfalse, muDateTimeParse(invalid_raw_dow_4, &date_time));
}

BOOST_AUTO_TEST_CASE(toUnixTime)
{
    MDateTime dt = { };
    dt.year = 2014;
    dt.month = mmonth_jun;
    dt.day = 1;
    dt.hours = 21;
    dt.minutes = 31;
    dt.seconds = 33;
    BOOST_CHECK_EQUAL(1401658293, muDateTimeToUnixTime(&dt));
    dt.timezone_offset_hours = 4;
    BOOST_CHECK_EQUAL(1401643893, muDateTimeToUnixTime(&dt));
    dt.timezone_offset_minutes = 30;
    BOOST_CHECK_EQUAL(1401642093, muDateTimeToUnixTime(&dt));
}

BOOST_AUTO_TEST_CASE(fromUnixTime)
{
    MDateTime dt = { };
    muUnixTimeToDateTime(1401658293, &dt);
    BOOST_CHECK_EQUAL(2014, dt.year);
    BOOST_CHECK_EQUAL(mmonth_jun, dt.month);
    BOOST_CHECK_EQUAL(1, dt.day);
    BOOST_CHECK_EQUAL(mdow_sun, dt.day_of_week);
    BOOST_CHECK_EQUAL(21, dt.hours);
    BOOST_CHECK_EQUAL(31, dt.minutes);
    BOOST_CHECK_EQUAL(33, dt.seconds);
    BOOST_CHECK_EQUAL(0, dt.timezone_offset_hours);
    BOOST_CHECK_EQUAL(0, dt.timezone_offset_minutes);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace LibMailUnit
