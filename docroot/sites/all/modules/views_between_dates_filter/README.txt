Between Dates Views Filter
by eosrei

This Views filter provides the between date functionality you are looking for:
display all nodes(comparing start and end dates) occurring on a specified date.

Creating this functionality with standard Date filters requires two filters:
start date less than or equal (<=) to the supplied date and end date greater
than or equal (>=) to the supplied date. The problem is two filters means users
must enter a date range, when technically "Show me all events in September." is
a range.

Effectively, it is the opposite of the standard "between" operator, which
compares one date field with two supplied dates.

Usage:
* Install the module
* Add the filter "Between Dates" to your View
* Select the start and end fields to compare.
* Adjust granularity to filter by year/month/date/hour/second.
