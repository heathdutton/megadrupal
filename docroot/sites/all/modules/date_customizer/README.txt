CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

This module allows user to customize the date field by adding hyphen or comma
as a separator in between two dates or multiple dates.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/date_customizer

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2505087

REQUIREMENTS
------------

This module requires the following modules:

 * Date (https://drupal.org/project/date)

INSTALLATION
------------

 * Download the module and copy it into your contributed modules folder:
   [for example, your_drupal_path/sites/all/modules] and enable it
   from the modules administration/management page (this module requires
   date API module to be enabled).See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

 * To configure the module use below point:

   - To add date type
     Go to Administration » Configuration » Regional and language,
     click on add date type, enter the date type and select the date format
     you want, if you want to add different date format follow the nextstep.

   - To add date format
     Go to Administration » Configuration » Regional and language » Date & time
     click on add date format and enter the format string, to customise
     the string format please see:
     http://www.php.net/manual/en/function.date.php

   - To add the custom date format to date field in content type
     Go to Administration » Structure » Content types
     select any content type you want and add the date field to it,
     then click on the manage display, select the format as date customizer
     click on the configure icon and do the configuration.

MAINTAINERS
-----------

Current maintainers:
 * Ravindra Fegade (rrfegade) - https://www.drupal.org/u/rrfegade
