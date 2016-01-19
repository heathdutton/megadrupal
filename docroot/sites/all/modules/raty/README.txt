JQUERY RATY WIDGETS
-------------------
Output any numeric field as a 'stars widget'.
Use a 'stars widget' as the input for any numeric field or Views exposed filter.
These are the field types handled: number_integer, number_decimal, number_float, list_number.


INSTALLATION
------------
This module comes with the jQuery Raty library ready to go!  No double downloads for you.
If you really feel like it, there is libraries integration and you can download your own.


SET UP
------
After enabling raty:
1) All numeric-storage fields will have a new formatter option for Raty.

After enabling raty_input:
1) All numeric views filters can be output as Raty stars (exposed filter).
2) All numeric-storage fields will have a new input widget option for Raty.


LIMITATIONS
-----------
Using Raty as an input method (Field or Views Exposed Filter) requires jQuery 1.7+
due to the use of .on() calls in the Raty library.
Because of this, the raty_input module requires the jquery_update module.
You can get jQuery Update here: http://drupal.org/project/jquery_update

You can use Raty as a field formatter with Drupal core's version of jQuery.


AUTHOR INFO
-----------
jQuery Raty library by Washington Botelho http://wbotelhos.com/raty/
Drupal Raty module by drastik http://drupal.org/user/433663
Read more at http://drastikbydesign.com


LICENSE INFO
------------
The jQuery Raty library is licensed under a Drupal-compatible MIT license, and
written permission has been given to include it with this module.
