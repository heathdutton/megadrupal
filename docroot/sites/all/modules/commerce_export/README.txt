
-- SUMMARY --

This module enables the exporting of Commerce entities via the Rules module.
Leveraging the Rules module allows the triggering of export actions by events.
One or more entities can be exported to one or more files. Export files are
saved to the temporary directory and available in Rules as file objects to be
moved to another location, sent via email, sent via ftp, etc. Formatting is
handled via Drupal's template system, so virtually any text format is
achievable. Example rules and template files are provided.

* Current supported entities:
  - Commerce Order

For a full description of the module, visit the project page:
  http://drupal.org/project/commerce_export

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/commerce_export


-- REQUIREMENTS --

* Drupal Commerce
  https://drupal.org/project/commerce

* Rules
  https://drupal.org/project/rules


-- INSTALLATION --

* Install as usual, see
  http://drupal.org/documentation/install/modules-themes/modules-7


-- CONFIGURATION --

Provided actions can be configured via the Rules module (see example rules).


-- CONTACT --

Current maintainers:
* Jon Antoine (jantoine) - https://drupal.org/user/192192

This project has been sponsored by:
* Showers Pass
  Technically engineered cycling gear for racers, commuters, messengers and
  everyday cycling enthusiasts. Visit http://showerspass.com for more
  information.
