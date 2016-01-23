This module provides a "Rooms availability" filter and a "Booking form" field
to improve the Views integration for Rooms (https://drupal.org/project/rooms).

Rooms Views allows to filter Bookable units Views by their availability
providing a exposed filter to enter the Arrival and Departure dates.

Booking form field displays a booking form out of the box to book each unit.
This field requires the availability filter to be able to determine the Arrival
and Departure dates for the booking.


Installation
------------

1. Follow the normal Drupal module installation procedures.


Usage
------------
Usage is straightforward. Select "Bookable Units: Rooms Availability" from the
list of available filters and then configure the default Arrival and Departure
dates.

To include the Booking form, select "Bookable Units: Booking form" from the list
of available fields and it will work out of the box.

Note: By now, Booking form field is only available when the price calculation
is set to "Per unit per night".


Authors/Credits
---------------

* Author: [plopesc](http://drupal.org/user/282415)
* Development sponsored by [Bluespark](http://bluespark.com).
