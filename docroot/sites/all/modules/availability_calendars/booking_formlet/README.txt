/**
 * README for Availability Calendar Booking Formlet module.
 *
 * @author Erwin Derksen (fietserwin: http://drupal.org/user/750928)
 */

The availability calendar booking formlet module defines a booking formlet
field that cooperates with Availability Calendar fields. It offers a small form
that can receive an arrival and departure date by reacting to click events on
Availability Calendar fields. If both dates are filled in, the submit button is
enabled and the user can continue to the real booking form.

This form is called a formlet as it is a small minimal form that only receives
the arrival and departure dates. The full booking form, where the visitor can
enter his/her name and other details is to defined separately. Think off an "Add
to cart" form where one can enter the quantity and click on "add to cart" which
brings you to the full cart form.


Dependencies
------------
This sub-module depends on the Availability Calendar module.
Also the form will typically be posted to a webform. So installing the webform
module is recommended.


Limitations
-----------
This module will not:
- Change the calendar. When clicking on the departure date the calendar will be
  updated visually, but this change will not be stored on the server. You can
  define a hook for that. See e.g. the webform documentation to find out which
  (submission) hooks best fit your needs.
- Define the webform. This depends to much on your specific situation.
- Offer payment integration.


Installing
----------
As usual.


Configuring
-----------
After enabling the module:
1) Define the date formats you want to use on admin/config/regional/date-time.
   You can localize these formats in admin/config/regional/date-time/locale.
2) Add a webform to post the form to. See the details below.
3) Add an "Availability Calendar Booking formlet" field to the entity type that
   also has an "Availability Calendar" field. Fill the "URL to continue the
   booking" field with the URL of the webform created in step 2.
4) Set the display settings on the "Manage display" page.
5) Style the form as you like. Note that this module only contains some basic
   styling for the formlet.

Basically this is it. Additionally you may want to define some webform
submission processing. This will be explained further on.


Views integration
-----------------
Suppose you have defined a view that searches on availability. The results page
shows the teasers of the entities that are available during the defined period.
Why not present just the "book now" button and allow the visitor to directly
go to the complete reservation form?

This is possible. You can define default values for the begin and date that can
come from some contextual variable. The results page of the search will probably
have a URL that looks like
search-availability?available[arrival][date]=2012-07-01&available[duration]=2
You can tell the formlet to take the begin date from $_GET, more specifically
the entry available[arrival][date] and that the end date should be based on a
duration (in days) given by the available[duration] entry of $_GET.

You can define this in the display settings for a given display mode. So, the
settings can differ for the full view, where visitors will still have to click
on the calendar before they can proceed to the complete reservation form, and
the teaser, where visitors can directly click on "book now".


How it works
------------
There seems to be quite some confusion about the why and how of this module. So,
following is a bit more in-depth explanation of the ideas behind this module.

Normally the page with the calendar is quite stuffed with all sorts of
information. Thus adding the complete booking form is not desirable. Therefore,
just a small form (aka 'formlet') is rendered that only contains the most basic
fields to allow it to interact with the calendar. Once this information is
completed by the visitor, it can be forwarded to the complete booking form.

Step by step:
- A visitor arrives at your site and, hopefully, eventually at the page with the
  Availability Calendar.
- The Availability Calendar module renders the calendar and defines the
  javascript that allows the visitor to click on available dates.
- This module adds the 'formlet' to that page including some javascript to
  handle the date click events and place the selected dates on the formlet.
- When the user clicks on "Book this resource ..." all information is POSTed to
  another page. This page is defined in the field settings and normally points
  to a webform with the real booking form.
- Together with the arrival and departure dates, a lot of hidden fields are
  submitted. These hidden fields contain information about the calendar field
  and the entity the calendar is attached to. (Note that calendar fields may be
  synchronized between translations and thus the calendar at its own, does not
  necessarily uniquely define the entity that was being shown.)
- The visitor fills in the remaining information and submits the complete
  booking form.
- Note: on webform submission, the values that the formlet originally POSTed
  will not be there anymore. So if you need them during the webform submission
  phase, you need to place them in webform components! (see below)
- The webform module handles the submission by e.g. e-mailing the submission to
  the website or accommodation owner.


Defining the 'complete booking form' webform
--------------------------------------------
This part assumes you are using the webform 3.x module. For webform 4.x please
be sure tho read the notes further on below as well.

- Define the webform components that the visitor should complete like name,
  e-mail, address, number of persons, etc.
- Define webform components for the dates that are already filled in:
  - Begin date (default value: %post[from_display]): the begin date.
  - End date (default value: %post[to_display]): the end date.
  - Duration (default value: %post[duration_display]): the number of days (or
    nights) between begin date and end date.
  or
  - Begin date (default value: %post[from_iso]): the begin date.
  - End date (default value: %post[to_iso]): the end date.
  - Duration (default value: %post[duration]): the number of days (or nights)
    between begin date and end date. For overnight calendars the end date is not
    inclusive in the duration, for full day rental calendars it is.
  These will typically be disabled text fields, the visitor can not change the
  dates anymore as the availability calendar is not visible on the webform.

  The difference between these 2 sets of dates are:
  - The dates in the latter set are in the ISO 8601 (yyyy-mm-dd) format, while
    the dates in the former set are in the format as presented to the visitor.
    You can define and localize this format via the Drupal admin interface at
    admin/config/regional/date-time(/locale).
    Note that this may lead to different date representations for the same date
    depending on the *visitor*, not the *recipient* of the webform!
  - For overnight calendar fields, the to_iso date is the last full day of the
    rental period, while the to_display date is the departure date.
  - The duration in the former set is a string that includes the word(s)
    night(s) and or day(s) and is translated in the language of the *visitor*,
    not the *recipient* of the webform! The duration in the latter is a number.
- As you will typically post to the same webform for all accommodations, you
  need to know from which accommodation the request was coming. To this end, you
  may want to define additional webform components that receive this information
  needed to process the booking:
  - cid (default value: %post[cid]): the calendar id.
  - calendar label (default value: %post[calendar_label]): title of the calendar
    (if set).
  - entity type (default value: %post[entity_type]): the entity type.
  - entity id (default value: %post[entity_id]): the ID of the entity.
  - entity label (default value: %post[entity_label]): the title of the entity.
  With the exception of entity label and/or calendar_label, these will typically
  be hidden fields as the visitor does not have to see this information.
  Note that in a multi-lingual site, a calendar may be synchronized over all
  translations of an entity. So there is not necessarily a 1-to-1 relationship
  between the calendar and the entity.
- Fields from the POST that you don't need, don't have to be defined as a
  webform component.
- You may also have fields from the POST that you only need for internal
  processing, but do not want to send in the e-mail. You should declare those
  fields as webform component and, subsequently, you can exclude them from the
  e-mail in the "Edit e-mail settings" page of the webform module.

Notes for webform 4.x:
- Enable the 'Use GET (instead of POST)' setting on the booking formlet field
  settings form.
- Use a syntax like [current-page:query:from_iso] as default value.


Hooks for custom processing
---------------------------
Depending on your situation, processing the webform may need some additional
programming from your side. Think of extracting the name of the accommodation
from the entity, or getting the e-mail address to send the request to. The
webform module and the mail system offer hooks for this.

1) The webform to complete the booking is used for multiple accommodations and
you want to change the e-mail address to which the webform submission is sent

2) On submission, you want to update the calendar and set the requested dates
to "Optionally booked".

ad 1)
See issue http://drupal.org/node/1470490

ad 2)
See issue http://drupal.org/node/1379384
