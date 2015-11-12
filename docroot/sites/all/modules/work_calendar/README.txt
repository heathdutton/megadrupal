Description
===========

Work Calendar allows to define yearly working/holiday calendars.

In general, these calendars can define what dates in a year are considered
available or not, for any purpose.

Work calendars are defined by a pattern of week days: choose which week days
a business/office/resource/whatever is open/available.

Currently this project only provides the backend and admin pages to build
calendars, and an API to query for day availability across a year, month,
week or single days.

Installation
============

As usual.

Configuration and usage
=======================

 * Configure the site-wide work calendar at admin/config/regional/work-calendar
 * Create calendars at admin/structure/work-calendars

Integration with date module
----------------------------

 * Enable work_calendar_date module.
 * Create or edit a date field.
 * Choose a work calendar under "More settings and values" > "Work calendar".

API
===

Work calendar entity class provides several methods to query the calendar
for dates availability in a given year/month.

WorkCalendar::getOpenDaysInPeriod($start, $end)
WorkCalendar::getOpenDays($year, $month = NULL)
WorkCalendar::getClosedDaysInPeriod($start, $end)
WorkCalendar::getClosedDays($year, $month = NULL)
WorkCalendar::isOpenDay($year, $month, $day)
WorkCalendar::isClosedDay($year, $month, $day)


In addition, work_calendar.module provides wrapper functions for the entity
methods. The first parameter to all the wrappers is the work calendar name
or instance. If null, the default site-wide work calendar will be used.

work_calendar_get_open_days_in_year($cal = NULL, $year = NULL)
work_calendar_get_open_days_in_month($cal = NULL, $year = NULL, $month = NULL)
work_calendar_get_open_days_in_period($cal = NULL, $start, $end)
work_calendar_day_is_open($cal = NULL, $year = NULL, $month = NULL, $day = NULL)
work_calendar_get_closed_days($cal = NULL, $year = NULL, $month = NULL)
work_calendar_day_is_closed($cal = NULL, $year = NULL, $month = NULL, $day = NULL)
