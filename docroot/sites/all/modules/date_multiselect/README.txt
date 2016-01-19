INTRODUCTION
------------
Javascript multiselect calendar widget using the MultiDatesPicker library on
top of the date_popup module. This will provide a new date_multiselect widget
to use on the date, datetime, and timestamp field types.

This code uses the MultiDatesPicker library that is included with this module.
Localization of the interface is handled by core.

The widget will use the site default for the first day of the week.


REQUIREMENTS
------------
This module requires the following modules:
 * Date (https://www.drupal.org/project/date)
 * Libraries (https://www.drupal.org/project/libraries)

And the following javascript libraries:
 * MultiDatesPicker plugin (http://multidatespickr.sourceforge.net)


INSTALLATION
------------
 1. Download the MultiDatesPicker plugin (http://multidatespickr.sourceforge.net
    version 1.6.3 is recommended) and extract the file under
    sites/all/libraries/multidatespicker. You should now have a
    sites/all/libraries/multidatespicker/jquery-ui.multidatespicker.js file.
 2. Download and enable the module.
 3. Add a date/datestamp/timestamp field with a date_multiselect widget to any
    entity.


LIMITATIONS
-----------
When using this widget no time will be selected for the date.


MAINTAINERS
-----------
Current maintainers:
 * Joaquin Bravo (jackbravo) - https://www.drupal.org/u/jackbravo
