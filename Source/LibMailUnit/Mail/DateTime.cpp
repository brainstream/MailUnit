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

#include <string>
#include <cstdio>
#include <utility>
#include <boost/regex.hpp>
#include <LibMailUnit/Mail/DateTime.h>

namespace {

constexpr char month_name_jan[] = "Jan";
constexpr char month_name_feb[] = "Feb";
constexpr char month_name_mar[] = "Mar";
constexpr char month_name_apr[] = "Apr";
constexpr char month_name_may[] = "May";
constexpr char month_name_jun[] = "Jun";
constexpr char month_name_jul[] = "Jul";
constexpr char month_name_aug[] = "Aug";
constexpr char month_name_sep[] = "Sep";
constexpr char month_name_oct[] = "Oct";
constexpr char month_name_nov[] = "Nov";
constexpr char month_name_dec[] = "Dec";

constexpr char day_name_mon[] = "Mon";
constexpr char day_name_tue[] = "Tue";
constexpr char day_name_wed[] = "Wed";
constexpr char day_name_thu[] = "Thu";
constexpr char day_name_fri[] = "Fri";
constexpr char day_name_sat[] = "Sat";
constexpr char day_name_sun[] = "Sun";

MMonth parseMonth(const std::string & _month_string)
{
    if(_month_string == month_name_jan)
        return mmonth_jan;
    if(_month_string == month_name_feb)
        return mmonth_feb;
    if(_month_string == month_name_mar)
        return mmonth_mar;
    if(_month_string == month_name_apr)
        return mmonth_apr;
    if(_month_string == month_name_may)
        return mmonth_may;
    if(_month_string == month_name_jun)
        return mmonth_jun;
    if(_month_string == month_name_jul)
        return mmonth_jul;
    if(_month_string == month_name_aug)
        return mmonth_aug;
    if(_month_string == month_name_sep)
        return mmonth_sep;
    if(_month_string == month_name_oct)
        return mmonth_oct;
    if(_month_string == month_name_nov)
        return mmonth_nov;
    if(_month_string == month_name_dec)
        return mmonth_dec;
    return  mmonth_invalid;
}

MDayOfWeek parseDayOfWeek(const std::string & _dow_string)
{
    if(_dow_string == day_name_mon)
        return mdow_mon;
    if(_dow_string == day_name_thu)
        return mdow_thu;
    if(_dow_string == day_name_wed)
        return mdow_wed;
    if(_dow_string == day_name_thu)
        return mdow_thu;
    if(_dow_string == day_name_fri)
        return mdow_fri;
    if(_dow_string == day_name_sat)
        return mdow_sat;
    if(_dow_string == day_name_sun)
        return mdow_sun;
    return mdow_invalid;
}

std::pair<short, short> parseTimeZone(const std::string & _zone_string)
{
    static const size_t sign_pos    = 0;
    static const size_t hours_pos   = 1;
    static const size_t minutes_pos = 3;
    short hours_offset = 0;
    short minutes_offset = 0;
    std::sscanf(&_zone_string.c_str()[hours_pos], "%2hd", &hours_offset);
    std::sscanf(&_zone_string.c_str()[minutes_pos], "%2hd", &minutes_offset);
    if('-' == _zone_string[sign_pos])
    {
        hours_offset = -hours_offset;
        minutes_offset = -minutes_offset;
    }
    return std::make_pair(hours_offset, minutes_offset);
}

} // namespace

MBool muDateTimeParse(const char * _raw_date_time, MDateTime * _date_time)
{
    if(nullptr == _date_time)
    {
        return mfalse;
    }
    boost::regex regex("\\s*"
        "((?<day_of_week>\\w{3})\\s*,\\s*)?"
        "(?<day>\\d{1,2})\\s+"
        "(?<month>\\w{3})\\s+"
        "(?<year>\\d{4})\\s+"
        "(?<hours>\\d{2})+\\s*\\:"
        "(?<minutes>\\d{2})+\\s*"
        "(\\:\\s*(?<seconds>\\d{2}))?\\s*"
        "(?<timezone>[+-]\\d{4})"
        "\\s*");
    boost::cmatch matches;
    if(!boost::regex_match(_raw_date_time, matches, regex))
    {
        return mfalse;
    }
    boost::csub_match day_of_week_match = matches["day_of_week"];
    boost::csub_match year_match = matches["year"];
    boost::csub_match month_match = matches["month"];
    boost::csub_match day_match = matches["day"];
    boost::csub_match hours_match = matches["hours"];
    boost::csub_match minutes_match = matches["minutes"];
    boost::csub_match seconds_match = matches["seconds"];
    boost::csub_match timezone_match = matches["timezone"];
    if(!year_match.matched || !month_match.matched || !day_match.matched ||
       !hours_match.matched || !minutes_match.matched || !timezone_match.matched)
    {
        return mfalse;
    }
    MDayOfWeek day_of_week = mdow_invalid;
    if(day_of_week_match.matched)
    {
        day_of_week = parseDayOfWeek(day_of_week_match);
    }
    unsigned short year = 0;
    std::sscanf(year_match.str().c_str(), "%4hu", &year);
    MMonth month = parseMonth(month_match);
    if(mmonth_invalid == month)
    {
        return mfalse;
    }
    unsigned short day = 0;
    std::sscanf(day_match.str().c_str(), "%2hu", &day);
    unsigned short hours = 0;
    std::sscanf(hours_match.str().c_str(), "%2hu", &hours);
    unsigned short minutes = 0;
    std::sscanf(minutes_match.str().c_str(), "%2hu", &minutes);
    unsigned short seconds = 0;
    if(seconds_match.matched)
    {
        std::sscanf(seconds_match.str().c_str(), "%2hu", &seconds);
    }
    std::pair<short, short> timezone_offset = parseTimeZone(timezone_match);
    _date_time->day_of_week = day_of_week;
    _date_time->year = year;
    _date_time->month = month;
    _date_time->day = day;
    _date_time->hours = hours;
    _date_time->minutes = minutes;
    _date_time->seconds = seconds;
    _date_time->timezone_offset_hours = timezone_offset.first;
    _date_time->timezone_offset_minutes = timezone_offset.second;
    return mtrue;
}


