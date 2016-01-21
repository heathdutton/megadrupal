# TFA Rules

TFA Rules is a Drupal module that provides integration with the Rules module to
add a condition to check to see if TFA has been enabled for a user.

This module also includes a default rule that redirects users on login to the
TFA security page with a message recommending that they configure it.

## Requirements

* https://www.drupal.org/project/tfa
* https://www.drupal.org/project/rules

## Installation

* Enable the `tfa_rules` module - see https://www.drupal.org/documentation/install/modules-themes/modules-7 for details.
* Visit /admin/config/workflow/rules to configure.
