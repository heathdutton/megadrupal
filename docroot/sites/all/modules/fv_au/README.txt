

Description
-------------

The field validation: Australia module provides field validation for various
Australian data.

Validators currently included are:

 * Australian Business Number (ABN)
 * Australian Company Number (ACN)
 * Tax File Number (TFN)
 * Medicare card number

If there is any other Australian data that can be validated that you would like
to see added please open a feature request in the issue queue.



Requirements
--------------

 * Field validation 2.x (https://drupal.org/project/field_validation)



Installation
--------------

Install the field validation: Australia module as usual.
See http://drupal.org/node/70151 for further information.



Configuration
---------------

This module is configured in the same manner as other validators of the
field validation module.

Anywhere that fields are used (nodes, taxonomy terms, field collections,
profile2 profiles, etc.), each field has a "validate" operation link.
This is where you configure validation for each field.

Once you have clicked the "validate" link you are presented with a list of
different validation types.
To configure a validation rule:
 1. Click the link for the desired validation for your field. For example: ABN.
 2. Give the validation rule a title. For example: ABN
 3. Select the column, which is usually "value".
 4. Add a "Custom error message". For example: The ABN you have entered is
invalid. Please enter a valid ABN.
 5. Configure other settings as required.

You may configure multiple validation rules per field.



Authors/maintainers
---------------------

Author/maintainer:
- Reuben Turk (rooby) - http://drupal.org/user/350381



Support / Bugs / Feature requests
-----------------------------------

Issues should be posted to the issue queue on drupal.org:
http://drupal.org/project/issues/fv_au
