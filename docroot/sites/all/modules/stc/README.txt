CONTENTS OF THIS FILE
---------------------
* Introduction
* Installation
* Configuration


INTRODUCTION
------------
This modules enable state transition control on select list fields. If you
enable state transition control on a field, you can set each (current) value of
this field which values can be set in next time (if the field current value is a
particular one). If you didn't set any state transition for value than all
values can be selected.

State state transition Control module can be very useful in case tracker systems
or in any other systems with a special form where it's necessary to control the
value state transitions of a field (ex.: an administration field).

INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-------------
* Create a integer, text or float type list field in any content type.
* Set allowed values of the field. (Hint: This can be changed after all.)
* On the field settings page check "Enable state transition control on this
  field."
* Set each (current) value of field which values can be set in next time.
