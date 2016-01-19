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

This is a light weight module that helps to restrict the term names with
blacklist characters/words, character length, word count and unique name.
Also restricts new terms creation through term reference, based on settings.

This also handles the validation of term names that are attached to the fields,
lets say, the term names that are created through the taxonomy reference,
autocomplete widget.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/term_name_validation

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/term_name_validation


REQUIREMENTS
------------

"No special requirements".


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

 * Configure user permissions in Administration » People » Permissions:
   Term Name validation admin control

 * Configure Term Name Validation in Modules » Configure
   admin/config/taxonomy/term-name-validation

 * Configure Term Name Validation for a Taxonomy reference field in the
   field settings (autocomplete - this is the case where new terms will be
   created if it is not already existing.).


MAINTAINERS
-----------

Current maintainers:
 * Krishna Kanth (krknth) - https://drupal.org/u/krknth
 * Harika Gujjula (harika gujjula) - https://drupal.org/u/harika-gujjula

