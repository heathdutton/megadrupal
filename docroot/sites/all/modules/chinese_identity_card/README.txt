INTRODUCTION
------------
The Chinese Identity Card module is a small module. But it's useful for entering
ID Card in china. No need to install field validation module or using drupal
form alter api to add validation handler. This module using text form element,
And adding module defined widget and validation. Now this module only accepts
valid ID Numbers in china (For example: 511702197701193532).

This module also supports third party service to validate the ID. just implement
hook_chinese_identity_card_validate().


REQUIREMENTS
------------
No special requirements.

INSTALLATION
------------
Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

CONFIGURATION
-------------
There is no configuration. When enabled, The module would generate a new field
type named Chinese identity card in the field list.
