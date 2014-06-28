/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                               *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#include <cstdio>
#include <boost/regex.hpp>
#include <MailUnit/Server/Email/DateTime.h>

/*
 * RFC 5322 3.3 - http://tools.ietf.org/html/rfc5322#section-3.3
 *
 * date-time       =       [ day-of-week "," ] date FWS time [CFWS]
 * day-of-week     =       ([FWS] day-name) / obs-day-of-week
 * day-name        =       "Mon" / "Tue" / "Wed" / "Thu" /
 *                         "Fri" / "Sat" / "Sun"
 * date            =       day month year
 * year            =       4*DIGIT / obs-year
 * month           =       (FWS month-name FWS) / obs-month
 * month-name      =       "Jan" / "Feb" / "Mar" / "Apr" /
 *                         "May" / "Jun" / "Jul" / "Aug" /
 *                         "Sep" / "Oct" / "Nov" / "Dec"
 * day             =       ([FWS] 1*2DIGIT) / obs-day
 * time            =       time-of-day FWS zone
 * time-of-day     =       hour ":" minute [ ":" second ]
 * hour            =       2DIGIT / obs-hour
 * minute          =       2DIGIT / obs-minute
 * second          =       2DIGIT / obs-second
 * zone            =       (( "+" / "-" ) 4DIGIT) / obs-zone
*/

#define MONTH_JAN "Jan"
#define MONTH_FEB "Feb"
#define MONTH_MAR "Mar"
#define MONTH_APR "Apr"
#define MONTH_MAY "May"
#define MONTH_JUN "Jun"
#define MONTH_JUL "Jul"
#define MONTH_AUG "Aug"
#define MONTH_SEP "Sep"
#define MONTH_OCT "Oct"
#define MONTH_NOV "Nov"
#define MONTH_DEC "Dec"

namespace {

boost::date_time::months_of_year parseMonth(const std::string & _month_string)
{
    if(_month_string == MONTH_JAN)
        return boost::date_time::Jan;
    if(_month_string == MONTH_FEB)
        return boost::date_time::Feb;
    if(_month_string == MONTH_MAR)
        return boost::date_time::Mar;
    if(_month_string == MONTH_APR)
        return boost::date_time::Apr;
    if(_month_string == MONTH_MAY)
        return boost::date_time::May;
    if(_month_string == MONTH_JUN)
        return boost::date_time::Jun;
    if(_month_string == MONTH_JUL)
        return boost::date_time::Jul;
    if(_month_string == MONTH_AUG)
        return boost::date_time::Aug;
    if(_month_string == MONTH_SEP)
        return boost::date_time::Sep;
    if(_month_string == MONTH_NOV)
        return boost::date_time::Nov;
    if(_month_string == MONTH_DEC)
        return boost::date_time::Dec;
    return  boost::date_time::NotAMonth;
}

/*
 * RFC 5322 3.3 - http://tools.ietf.org/html/rfc5322#page-15
 *
 * The zone specifies the offset from Coordinated Universal Time (UTC,
 * formerly referred to as "Greenwich Mean Time") that the date and
 * time-of-day represent.  The "+" or "-" indicates whether the
 * time-of-day is ahead of (i.e., east of) or behind (i.e., west of)
 * Universal Time.  The first two digits indicate the number of hours
 * difference from Universal Time, and the last two digits indicate the
 * number of minutes difference from Universal Time.
*/
boost::shared_ptr<boost::local_time::custom_time_zone> parseTimeZone(const std::string & _zone_string)
{
    static const size_t sign_pos    = 0;
    static const size_t hours_pos   = 1;
    static const size_t minutes_pos = 3;
    int8_t hours_offset = 0;
    int8_t minutes_offset = 0;
    std::sscanf(&_zone_string.c_str()[hours_pos], "%2hhd", &hours_offset);
    std::sscanf(&_zone_string.c_str()[minutes_pos], "%2hhd", &minutes_offset);
    if('-' == _zone_string[sign_pos])
    {
        hours_offset = -hours_offset;
        minutes_offset -minutes_offset;
    }
    std::string zone_name = _zone_string.substr(0, minutes_pos) +
        ":" + _zone_string.substr(minutes_pos);
    boost::local_time::time_zone_names tz_names(zone_name, zone_name, zone_name, zone_name);
    boost::posix_time::time_duration offset(hours_offset, minutes_offset, 0);
    boost::local_time::dst_adjustment_offsets dst_adjustments(
        boost::posix_time::time_duration(0, 0, 0),
        boost::posix_time::time_duration(0, 0, 0),
        boost::posix_time::time_duration(0, 0, 0));
    return boost::shared_ptr<boost::local_time::custom_time_zone>(
        new boost::local_time::custom_time_zone(tz_names, offset, dst_adjustments, nullptr));
}

} // namespace

MailUnit::Email::DateTimePtr MailUnit::Email::parseDateTime(const std::string & _input)
{
    boost::regex regex(
        "((?<day_of_week>\\w{3})\\s*,\\s*)?" // we'll ingore this field
        "(?<day>\\d{1,2})\\s+"
        "(?<month>\\w{3})\\s+"
        "(?<year>\\d{4})\\s+"
        "(?<hours>\\d{2})+\\s*\\:"
        "(?<minutes>\\d{2})+\\s*"
        "(\\:\\s*(?<seconds>\\d{2}))?\\s*"
        "(?<timezone>[+-]\\d{4})");
    boost::smatch matches;
    if(!boost::regex_match(_input, matches, regex))
    {
        return nullptr;
    }
    boost::ssub_match year_match = matches["year"];
    boost::ssub_match month_match = matches["month"];
    boost::ssub_match day_match = matches["day"];
    boost::ssub_match hours_match = matches["hours"];
    boost::ssub_match minutes_match = matches["minutes"];
    boost::ssub_match seconds_match = matches["seconds"];
    boost::ssub_match timezone_match = matches["timezone"];
    if(!year_match.matched || !month_match.matched || !day_match.matched ||
       !hours_match.matched || !minutes_match.matched || !timezone_match.matched)
    {
        return nullptr;
    }
    uint32_t year = 0;
    std::sscanf(year_match.str().c_str(), "%4u", &year);
    boost::date_time::months_of_year month = parseMonth(month_match.str());
    uint8_t day = 0;
    std::sscanf(day_match.str().c_str(), "%2hhu", &day);
    uint8_t hours = 0;
    std::sscanf(hours_match.str().c_str(), "%2hhu", &hours);
    uint8_t minutes = 0;
    std::sscanf(minutes_match.str().c_str(), "%2hhu", &minutes);
    uint8_t seconds = 0;
    if(seconds_match.matched)
        std::sscanf(seconds_match.str().c_str(), "%2hhu", &seconds);
    boost::gregorian::date date(year, month, day);
    boost::posix_time::time_duration duration(hours, minutes, seconds);
    return std::make_shared<DateTime>(date, duration, parseTimeZone(timezone_match.str()), false);
}
