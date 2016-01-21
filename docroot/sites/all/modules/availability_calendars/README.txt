/**
 * README for Availability Calendar module.
 *
 * @author Erwin Derksen (fietserwin: http://drupal.org/user/750928)
 * @link: http://drupal.org/project/availability_calendars (project page)
 *
 * Note that the module name is availability_calendar, thus without an s, though
 * the project page and the tar.gz do have an s. This is for historical reasons.
 */

The Availability Calendar module allows you to add an availability calendar to
entities. Example use cases are tourist accommodation, e.g. bed and breakfast,
holiday homes or self catered apartments, and car or motor bike rental.

An availability calendar shows your customers at what dates your accommodation
is still available and at what dates it is already booked.

This module focuses on displaying availability. It does not try to support the
booking or payment process. Having said this, the "booking formlet" sub-module
does give you a start by offering a booking request form where visitors can
easily select their dates. But there is no further integration. So, the booking
formlet will not automatically change the availability state of a calendar. You
will have to edit the availability state manually.

If you are looking for a fully integrated booking and payment process, try the
Rooms module (http://drupal.org/project/rooms).

For a general overview of features see the project page:
  http://drupal.org/project/availability_calendars

Below you will find more detailed informatin that is useful for site builders
and developers.

Dependencies
------------
- The 'booking formlet display settings form' uses OR #states which are
  available as of Drupal 7.14.
- The Views support part uses the format_string function that is available as of
  Drupal 7.9.
- The Views support part uses the date_popup if available, but this is no hard
  dependency.
- Date parsing based on (localizable) date types uses either the date_api module
  or PHP 5.3 functionality. Thus either you are on PHP 5.3 or higher, or you
  must install and enable the date_api module.
- Date argument parsing (Views contextual filter) is either done internally
  (only yyyy-mm-dd accepted) or by the date_api module that accepts far more
  notations including e.g. @ for the current date. See the date_api module
  documentation.


Experimental: Entity integration
--------------------------------
The 7.x-5.x branch introduces entity API integration. It will continue to
support Availability Calendar as a field though and we will try to continue to
support this hybrid approach. Fields to keep it easy, Entities if you need more
power.
Done: see the change list in CHANGELOG.txt
Todo: see the todo list in availability_calendar.entity.inc.
Styling
-------
The modules contains a style sheet with basic styling that should give you a
reasonable look & feel out of the box (availability_calendar.base.css).
Additionally, you can specify some styling via the admin user interface on
admin/config/content/availability-calendar/styling. This mainly concerns styling
related to the calendar and its states, so that site builders do not need to
know in detail how the calendar and especially the (split day) states are
rendered. This will generate a file
sites/default/files/availability_calendar/availability_calendar.css. Remaining
styling is to be defined in your (sub-)theme. See availability_calendar.base.css
for how the calendar and key are rendered.

Issues have been reported for various themes, a.o. the Shiny theme. This module
does not try to work out of the box with all themes available on D.O. It is up
to the site designer to add necessary CSS to undo/overrule clashing style
settings.


Views integration
-----------------
Availability Calendar provides the following features to you when working with
the Views module:

- An exposable filter, named "<field name> available", to search on
availability. You can choose to do a search on a start date and a duration or on
a start date and an end date, where the end date can be inclusive, typical for
full day rental, or exclusive, better suited to overnight rental.

- A contextual filter, also named "<field name> available", to search on
availability. You can do a search on a start date and an end date, where you can
choose whether the end date is inclusive, typical for full day rental, or
exclusive which is better suited to overnight rental. You can provide the 2
dates by separating them with 2 dashes (--) otherwise the single date provided
is used for both the start date and the end date (inclusive, regardless your
setting).

If the Date API module is enabled you can enter your arguments in many formats
including e.g. @ for the current date. See the date_api module documentation.
Otherwise, you can only provide the arguments in the yyyy-mm-dd format.

Together with this contextual filter you can enable validation on this argument,
but the date api module is very relaxed in what it accepts. If the date api
module is not enabled and you thus stick to the yyyy-mm-dd format, checking is
more tight.

- Fields to show or filter on calender enabled, calendar name, calendar created
date and calendar updated date. Note that although there is a separate filter on
the calendar option "enabled" in the views UI, if you define a view that
accesses information from one of the availability_calendar_* tables, an extra
join condition on enabled will be added automatically. So normally there is no
need to add this filter, except perhaps in some administrative edge use cases.


Search API integration (experimental)
-------------------------------------
Availability integrates with Search API to allow to filter on availability
within searches.

Steps to perform:
- Visit the admin settings form at admin/config/content/availability-calendar to
set "Number of months to index" to the value you want. The index will "copy"
availability data to the index but only for a number of given months from now.
This means that searches further in the future than this setting always will
succeed.
- Define a search server as usually. However, for now only search_api_db servers
are supported (search_api_db is s separate module on drupal.org).
- Define an index as usual. The index should be on the entity type you want to
search for, not on the Availability CaLendar entity type.
- On the fields tab of the index (page
admin/config/search/search_api/index/{index_name}/fields), click on "Add related
fields", select the calendar field you want to filter on, and click on "Add
fields".
- Click again "Add related fields", select the "{Calendar field name} »
Availability Calendar" and click on "Add fields".
- Select the field "{Calendar field name} » Availability Calendar » Filtered
availability" and set its type to "Availability".
- Select the other fields you may want to be able to search on.
- Click on "Save changes".

Availability can now be indexed. Note that as only a given number of months into
the future are indexed, the index will get outdated when time passes, even if
the entities and its availability are never changed. So once a month or so, you
should reindex unconditionally (or set all entities that did not change last
month to "needs to be reindexed" (not sure if this is easy to do).

To use the indexed availability, you must create a view based on the search
index. This means that the sub module search_api_views must be enabled. For now,
no other ways of accessing the search index is available.

Create the view:
- On admin/structure/views, click on "Add new view".
- Select the created index in the "Show" drop-down.
- On the edit form, add a (contextual) filter for the "{Calendar field name} »
Availability Calendar: Filtered availability (indexed)" field.
- Complete the view as needed to meet your other requirements.

WARNING:
The above described method only works when the calendar field you are indexing
has only 1 value. Otherwise you need to perform the following steps:
- Perform the steps as above, but instead of selecting the "{Calendar field
name} » Availability Calendar » Filtered availability" field, select the
"{Calendar field name} » Availability Calendar » Calendar ID" field. You still
have to set its type to "Availability" though!
- Create another index for the Availability Calendar item type. This index must
be on the same server, as tables will be joined.
- Select the field "Filtered availability" and leave its type at "Availability".
- In the View, you create, add a (contextual) filter on the field "{Calendar
field name} » Availability Calendar: Calendar ID (indexed)". It will be turned
into a filter on availability by automatically joining to the 2nd index as
created above. The user interface will also be the same, thus allowing you to
define the start and end date, not a number.


Caching
-------
Caching pages with availability calendars is possible but keep in mind that the
calendars change just because the date changes, thus without anyone changing the
data that belongs to the calendar. This means that ideally you should set your
page caches to expire next midnight. However, most caching mechanisms, including
the standard one provided by Drupal, only allows you to set an offset to the
current time. So an offset up to half a day should not give you many problems.
Note that in a multilingual set-up with field syncing (i18n_sync module) field
syncing goes through node_save and thus invalidates the cache.


I18n
----
Availability calendar is (or strives to be) fully multilingual aware. Using the
standard translation model - several entities composing 1 translation set - the
calendars can be shared between translations by enabling field syncing for them.

The names of the states are considered hard coded texts and thus translated
using t() not i18n_string, even though they may be overridden via user entered
input. They should thus be entered in English.

The names of the calendars are field values and thus not translated. On syncing
they won't overwrite already existing names, but if no name exists in the target
language the name is copied.

Form labels are passed through t() and thus can be translated. If you want to
change the labels in english because, e.g., the terms arrival and departure do
not fit your use case, you can use the String Overrides module
(http://drupal.org/project/stringoverrides). This might get changed in the
future by placing these texts in variables (that can be made multilingual aware
with i18n_variable).


API
---
All database access, querying as well as writing, is placed in separate
functions, thus never directly in form handling functions. So this functionality
is easily available to other modules. To make use of the API you have to include
the .inc file:
  module_load_include('inc', 'availability_calendar');
This to prevent the API being loaded on every request.


Installing
----------
As usual. After enabling the module:
- Define the states you want to use on
  admin/config/content/availability-calendar/settings
- Define the date formats you want to use on admin/config/regional/date-time.
  You can localize these formats in admin/config/regional/date-time/locale.
- Define the basic styling, including the colors for the states, on
  admin/config/content/availability-calendar/styling
- Add availability calendar fields to the requested content types.


Upgrading from Availability Calendars 7.x-3.x
---------------------------------------------
Read the compatibility notes in CHANGELOG.txt to see what you have to check and
test.


Upgrading from Availability Calendars 7.x-2.x or earlier
--------------------------------------------------------
To Drupal this is a different module from the already existing Availability
Calendars module. This makes upgrading via update.php a bit tricky. Therefore, a
separate update module has been created. This module can be found in the latest
7.x-2.x package. So install that version as well. The Availability Calendars
update module contains an UPGRADE.txt with more detailed information about
upgrading.


Issues in core and other modules you may run into
-------------------------------------------------
- [#838096]: Problem with the "active" class of tablesort
- [#1342874]: Allow date popup in exposed Views form to 'remember' value
- [#1580700]: Hidden "secure value" component loosing it's token (%get, %post)
  value on webform submission
- [#1592688]: #states are applied twice on same element
