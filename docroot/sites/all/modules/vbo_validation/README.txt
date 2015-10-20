CONTENTS OF THIS FILE
---------------------

* Introduction
* Requirements
* Installation
* Usage
* Packaging
* Troubleshooting
* Maintainers

INTRODUCTION
------------

VBO Validation is a module that provides hooks to add validation between the execution of a VBO and its form or
confirmation step.

Adding validation to prevent a VBO from executing is easy when you have a form step. But without a custom form, adding a
validate function is a delicate exercise of making your module invoke last, form altering, checking for the existence of
certain form elements and actions, and then adding your validation hook.

Here is a shortcut to that process!

This module will provide you with two hooks:

 * hook_vbo_validation_validate - Perform validations before the confirmation step of a VBO is fired.

 * hook_vbo_validation_ACTION_ID_validate - Provide a VBO-specific validation instead of the global hook_vbo_validation_validate().

REQUIREMENTS
------------
This module requires the following modules:

* Views (https://www.drupal.org/project/views)
* Views Bulk Operations (VBO) (https://www.drupal.org/project/views_bulk_operations)

INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


USAGE
-----

To add a validation step to prevent or allow running of VBO actions, simply add one of the two hooks. To set an error
on the actual VBO form field, ask VBO what field it is and set the error there, like so:

  $vbo = _views_bulk_operations_get_field($form_state['build_info']['args'][0]);
  $field_name = $vbo->options['id'];
  form_set_error($field_name, t('No, VBO! No!'));


TROUBLESHOOTING
---------------

This project is still somewhat experimental. Please feel free to poke around the
code. Issues, questions, comments, etc are welcome in the issue queue
https://www.drupal.org/project/issues/vbo_validation.

MAINTAINERS
-----------

 * Jack Franks (franksj) - https://drupal.org/user/2743481
