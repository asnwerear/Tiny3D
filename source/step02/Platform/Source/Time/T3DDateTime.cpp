﻿/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2017  Answer Wong
 * For latest info, see https://github.com/asnwerear/Tiny3D
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "T3DDateTime.h"
#include <time.h>
#include <sys/timeb.h>

namespace Tiny3D
{
    DateTime::DateTime()
        : mYear(0)
        , mMonth(0)
        , mDay(0)
        , mHour(0)
        , mMinute(0)
        , mSecond(0)
        , mMillisecond(0)
    {

    }

    DateTime::DateTime(int32_t year, int32_t month, int32_t day, 
        int32_t hour, int32_t minute, int32_t second, int32_t millisecond)
        : mYear(year)
        , mMonth(month)
        , mDay(day)
        , mHour(hour)
        , mMinute(minute)
        , mSecond(second)
        , mMillisecond(millisecond)
    {

    }

    uint64_t DateTime::toMSecsSinceEpoch() const
    {
        tm dt;
        dt.tm_year = mYear - 1900;
        dt.tm_mon = mMonth - 1;
        dt.tm_mday = mDay;
        dt.tm_hour = mHour;
        dt.tm_min = mMinute;
        dt.tm_sec = mSecond;
        dt.tm_isdst = 0;

        time_t t = mktime(&dt);
        uint64_t time = (uint64_t)t * 1000 + mMillisecond;
        return time;
    }

    std::string DateTime::toString() const
    {
        char text[128];
        snprintf(text, 128, "%d-%02d-%02d %02d:%02d:%02d.%03d", 
            mYear, mMonth, mDay, mHour, mMinute, mSecond, mMillisecond);
        std::string s(text);

        return s;
    }

    std::string DateTime::timeToString(TimeFormat eFormat) const
    {
        char text[128];
        switch (eFormat)
        {
        case TF_HH_MM_SS_XXX:
            snprintf(text, 128, "%02d:%02d:%02d.%03d", 
                mHour, mMinute, mSecond, mMillisecond);
            break;

        case TF_HH_MM_SS:
            snprintf(text, 128, "%02d:%02d:%02d", mHour, mMinute, mSecond);
            break;

        case TF_HH_MM:
            snprintf(text, 128, "%02d:%02d", mHour, mMinute);
            break;

        case TF_HHMMSS:
            snprintf(text, 128, "%02d%02d%02d", mHour, mMinute, mSecond);
            break;

        default:
            snprintf(text, 128, "%02d:%02d:%02d.%03d", 
                mHour, mMinute, mSecond, mMillisecond);
            break;
        }

        std::string s(text);
        return s;
    }

    std::string DateTime::dateToString(DateFormat eFormat) const
    {
        char text[128];
        switch (eFormat)
        {
        case DF_YY_MM_DD:
            snprintf(text, 128, "%d-%02d-%02d", mYear, mMonth, mDay);
            break;

        case DF_MM_DD:
            snprintf(text, 128, "%02d-%02d", mMonth, mDay);
            break;

        case DF_YY_MM:
            snprintf(text, 128, "%d-%02d", mYear, mMonth);
            break;
        case DF_YYMMDD:
            snprintf(text, 128, "%d%02d%02d", mYear, mMonth, mDay);
            break;
        default:
            snprintf(text, 128, "%d-%02d-%02d", mYear, mMonth, mDay);
            break;
        }

        std::string s(text);
        return s;
    }

    bool DateTime::operator ==(const DateTime &other) const
    {
        return (other.mYear == mYear 
            && other.mMonth == mMonth 
            && other.mDay == mDay
            && other.mHour == mHour 
            && other.mMinute == mMinute 
            && other.mSecond == mSecond
            && other.mMillisecond == mMinute);
    }

    bool DateTime::operator !=(const DateTime &other) const
    {
        return (other.mYear != mYear 
            || other.mMonth != mMonth 
            || other.mDay != mDay 
            || other.mHour != mHour 
            || other.mMinute != mMinute 
            || other.mSecond != mSecond 
            || other.mMillisecond != mMinute);
    }

    bool DateTime::operator <(const DateTime &other) const
    {
        return (mYear < other.mYear
            || (mYear == other.mYear && mMonth < other.mMonth)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay < other.mDay)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour < other.mHour)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour == other.mHour 
                && mMinute < other.mMinute)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour == other.mHour 
                && mMinute == other.mMinute && mSecond < other.mSecond)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour == other.mHour 
                && mMinute == other.mMinute && mSecond == other.mSecond
                && mMillisecond < other.mMillisecond));
    }

    bool DateTime::operator <=(const DateTime &other) const
    {
        return (*this < other || *this == other);
    }

    bool DateTime::operator >(const DateTime &other) const
    {
        return (mYear > other.mYear
            || (mYear == other.mYear && mMonth > other.mMonth)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay > other.mDay)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour > other.mHour)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour == other.mHour 
                && mMinute > other.mMinute)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour == other.mHour 
                && mMinute == other.mMinute && mSecond > other.mSecond)
            || (mYear == other.mYear && mMonth == other.mMonth 
                && mDay == other.mDay && mHour == other.mHour 
                && mMinute == other.mMinute && mSecond == other.mSecond
                && mMillisecond > other.mMillisecond));
    }

    bool DateTime::operator >=(const DateTime &other) const
    {
        return (*this > other || *this == other);
    }

    DateTime DateTime::currentDateTime()
    {
        timeb timebuffer;
        ftime(&timebuffer);
        time_t time = timebuffer.time;
        int32_t millisecond = timebuffer.millitm;
        int32_t timezone = timebuffer.timezone * 60;
        time = time - timezone;
        tm *t = gmtime(&time);
        return DateTime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
            t->tm_hour, t->tm_min, t->tm_sec, millisecond);
    }

    DateTime DateTime::currentDateTimeUTC()
    {
        timeb timebuffer;
        ftime(&timebuffer);
        time_t time = timebuffer.time;
        int32_t millisecond = timebuffer.millitm;
        tm *t = gmtime(&time);
        return DateTime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
            t->tm_hour, t->tm_min, t->tm_sec, millisecond);
    }

    uint64_t DateTime::currentSecsSinceEpoch()
    {
        timeb timebuffer;
        ftime(&timebuffer);
        time_t time = timebuffer.time;

        return time;
    }

    uint64_t DateTime::currentMSecsSinceEpoch()
    {
        timeb timebuffer;
        ftime(&timebuffer);
        time_t time = timebuffer.time;
        int32_t millisecond = timebuffer.millitm;
        uint64_t t = (uint64_t)time * 1000 + millisecond;
        return t;
    }

    DateTime DateTime::fromMSecsSinceEpoch(uint64_t msecs)
    {
        time_t s = msecs / 1000;
        int32_t ms = msecs % 1000;
        tm *t = localtime(&s);
        return DateTime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
            t->tm_hour, t->tm_min, t->tm_sec, ms);
    }

    DateTime DateTime::fromSecsSinceEpoch(uint64_t sces)
    {
        time_t s = sces;
        tm *t = localtime(&s);
        return DateTime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
            t->tm_hour, t->tm_min, t->tm_sec, 0);
    }
}
