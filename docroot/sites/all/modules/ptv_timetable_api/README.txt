PTV TIMETABLE API
-----------------

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers


INTRODUCTION
------------

The PTV Timetable API has been created to give the public direct access to
Public Transport Victoria's public transport timetable data. The API allows you
to query locations for scheduled timetable, line and stop data for all
metropolitan and regional train, tram and bus services in Victoria
(including NightRider). It also includes real-time data for metropolitan tram
and bus services (where this data is made available to PTV), as well as
disruption information, and access to myki ticket outlet data.

The PTV Timetable API module is a third-party integration module for
Australia Public Transport Victoria Timetable API. It provides a clean wrapper
class to allow user call PTV timetable API in object oriented way.

For obtaining an official API documentation, please visit following page:

https://www.data.vic.gov.au/data/dataset/ptv-timetable-api


 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/jiqiang/2494467


 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2494467


REQUIREMENTS
------------

To use PTV Timetable API you need to register for a security key and developer
ID with PTV. Send an email to APIKeyRequest@ptv.vic.gov.au with the following
information in the subject line of the email:

  "PTV Timetable API – request for key"

Once PTV has got your email request, they send you a key and a developer ID by
return email.


RECOMMENDED MODULES
-------------------

None


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7 for further
information.


CONFIGURATION
-------------

 * Configure user permissions in Administration >> People >> Permissions:

   - Administer PTV timetable API

    User in roles with "Administer PTV timetable API" permission will have
    access to module configuration page .

 * Save your PTV Timetable API security key and developer ID in
   Administration >> Configuration >> System >> PTV Timetable API


TROUBLESHOOTING
---------------

If it shows "access denied" when you access configuration page, please check if
"Adminster PTV timetable API" permission is enabled for the appropriate roles.


FAQ
---

None


MAINTAINERS
-----------

Current maintainers:
 * Qiang Ji (jiqiang) - https://www.drupal.org/user/2882815
