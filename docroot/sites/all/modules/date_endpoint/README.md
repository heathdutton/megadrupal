CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration



INTRODUCTION
------------
This module is a wrapper around the DateEndpointGenerator class, which will
generate date endpoint pairs in intervals, relative to a date of your choosing.
This is useful, for example, if it's Monday, and you want to generate select
options for Sunday, Saturday, and Friday.

Using the class directly, you can generate select options which can be easily be
piped into database queries, to find records within a certain date range.

This module also provides a basic field widget which will store the date
endpoint values in the database, using the Date module as a backend.



LIMITATIONS
------------
Dates are very complex, and as you use this module, you will probably come up
with quite a few configuration options that you need to accomplish your goal.
We can't possibly include every option for every use-case.

Therefore, the intention is to keep the default date endpoint widget as simple
as possible.  Perhaps in the future, we will implement CTools plugins to make it
 easy for you to extend these classes to suit your own needs.

If you need advanced functionality, the best option is to copy the module code
and extend the DateEndpointGenerator class manually.

Other limitations:
* Timezone handling is not implemented yet; all endpoints are stored with no
timezone, which will default to the timezone of the server.  Therefore, the
'timezone handling' field instance setting only affects display.
* The 'default value' field instance settings are not yet implemented.
* PHP 5.3 versions may handle DateTime slightly differently.  If you have
errors, consider updating to the latest PHP patch release.
* Right now, only the 'datetime' database column type is supported.
* This widget is not compatible with Inline Entity Form.



REQUIREMENTS
------------
This module requires the date module, as well as PHP 5.3+, since it makes
extensive use of the DateTime class.



RECOMMENDED MODULES
------------
The Chosen module is shown in the screenshot; it will enhance the select element
with a nicer-looking element, as well as a built-in search feature.



INSTALLATION
------------
Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7 for further
information.



CONFIGURATION
-------------
The widget settings are entered on the normal widget settings page per field
instance.  It contains the following options:
* Reverse Endpoints: Controls which endpoint value we store in the database.
Useful for field instances where we are not collecting an end date.  By default,
 the earliest point in the endpoint pair is used.
* Interval count: How many intervals of each component do you want to present as
select options?  Set to 0 to generate no endpoints for that component.
* (Planned) Origin: Set the origin you want to use to generate endpoints.
* (Planned) Ignore Boundaries: Generate the endpoints purely from the origin
date and time, instead of breaking cleanly on date/month/year boundaries.
