
--------------------------------------------------------------------------------
                           Commerce Booking
--------------------------------------------------------------------------------

Maintainers:
 * Robert Mumford (rlmumford), rlmumford@googlemail.net
 * Andrew Belcher (andrewbelcher), belcher.andrew@googlemail.com


This module provides tools to use Drupal Commerce as a powerful event booking
system. Any entity can be an 'event' and a single booking can contain any
number of tickets.


Installation
-------------

 * Copy the whole commerce_booking directory to your modules directory and
   activate the module.


Setup
-----

 * Create a new content type for your events (/admin/structure/content_types)
   and add to it a 'Price' field and a 'Date' field.

 * Next add a 'commerce_booking' field and select the date and price field on
   the field options page.

 * You can optionally add a terms and conditions field to the entity. The text
   on this field will be shown on the first step of the checkout process.

 * You can optionally add a Deposit (commerce_price) field to the entity. This
   will allow bookers to optionally pay only the deposit on a ticket to secure
   their place.

 -- The above steps can be performed on any entity to turn it into an event. --


Usage
-----

 * Add a new Event (using /node/add/content or other) and in the Booking
   Settings make sure the 'Open' option is selected.

 * When viewing the Event there is now a 'Book Now' button which will take you
   through the booking process.

 * The booking process makes extensive use of the Commerce Checkout System and
   if you also have a Commerce Shop on your site it is reccommended that you
   install a module that will allow you to have multiple checkout processes
   (There is a patch against the commerce_order_types module available for
   this).


--------------------------------------------------------------------------------
                      Commerce Booking Flexible Pricing
--------------------------------------------------------------------------------

Maintainers:
 * Robert Mumford (rlmumford), rlmumford@googlemail.net
 * Andrew Belcher (andrewbelcher), belcher.andrew@googlemail.com

This module provides the ability to have complex pricing structures for your
events. Using a combination of a date field, rules and a multiple valued price
field, you can add ticket-classes and booking windows.

Terminology
-----------

 * Booking Window - a booking window is a period of time while a particular
   pricing structure applie. Booking Windows are active until their cut-off
   date is reached, at which point the next booking window will come into
   effect.

   Booking Windows are ideal if you want to use Early Bird discounts for an
   event.

 * Ticket Class - a ticket class is a set of criteria a ticket must meet to
   have a particular price. In Commerce Booking, Ticket Classes are rules
   condition sets. A ticket will be assigned the first ticket class who's
   criteria it fulfills.

   Common examples of Ticket Classes are: Concessions, Under-5's or Students.

Installation
------------

 * Once commerce_booking is installed, activate the commerce booking flexible
   pricing module.

 * Follow the Installation instructions as above.

 * Ensure the price field on your event entity is set to 'Unlimited'
   cardinality.

 * Ensure the price field on your event entity has the widget set as the
   Flexible Pricing Widget. The flexible pricing widget turns the price form
   element into a table, where the column headings are the 'Booking Windows' and
   the row headings are the 'Ticket Classes'.

 * By default there is one 'Standard' booking window that applies until the
   event start date. To have more Booking Windows on an event, add a
   commerce_booking_windows field to you event entity.


Usage
-----

 * Go to 'admin/commerce_booking/ticket_classes' to add ticket classes. Ticket
   classes are rules condition sets that can use any information on the ticket
   entity to decided whether or not a ticket fits into a particular class.

 * Booking windows can be added on an event by event basis.

 * Once the Booking Windows and Ticket Classes have been configured you can
   enter a price for every ticket class and booking window combination in the
   table provided.


--------------------------------------------------------------------------------
                               General notes
--------------------------------------------------------------------------------

  * This module has been built by the developers of the Party module. To get
    the best results from your drupal booking system we reccomend you install
    Party and the Party Commerce Booking integration module.
