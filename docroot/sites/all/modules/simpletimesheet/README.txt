
--------------------------------------------------------------------------------
Simple TimeSheet
--------------------------------------------------------------------------------

Maintainer:  xlyz, xlyz@tiscali.it

Provides very basic functionalities to handle timesheets in Drupal 7.

This module has been developed as none of the many timesheet / timerecord 
6.x modules has been ported to Drupal 7.x.

Poject homepage: http://drupal.org/project/simpletimesheet

Issues: http://drupal.org/project/issues/simpletimesheet


Installation
------------

The following dependencies shall be latest dev release (or any non dev 
version released after july 31st 2011 if available):

views-7.x-3.x
fullcalendar-7.x-2.x-dev 
date-7.x-2.x-dev 

 * Copy the whole simpletimesheet directory to your modules directory
   (e.g. DRUPAL_ROOT/sites/all/modules) and activate it in the modules page

 * Assign "Create activity" and "Create time records" permission to relevant
   roles

 * Assign "Timesheet access" permission to roles that can access time records
   reporting
 
 * ...

 * Profit!  :)
   

Documentation
-------------

This module use fields and views as much as possible to allow easy
customization.

Time records is where you register how you spend your time.

Each time records is linked to one "activity". Users are allowed to link only
activities that they are member of.

At least one content type "activity" has to be created. 

"Timesheet access" permission is required to view timesheets reports.

Please refer to fullcalendar documentation for learn how to use it.

WARNING: Multiday time records can already be created, but reporting does not
yet handle them correctly. For the time being is strongly recommended to use
only single day time records.


TODO
----

- Handle multiday time records
- Autocomplete for nodereference Views exposed filter (when ready upstream)
- Create time records dragging on fullcalendar (when ready upstream)
