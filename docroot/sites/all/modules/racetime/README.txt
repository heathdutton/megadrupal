Readme file for the racetime module for Drupal
----------------------------------------------

OVERVIEW

  Racetime creates a field that allows the user to store race
  times in the format [mm:]ss.0xx, where m is an optional minutes value,
  s is the number of seconds.  Race times can also track the tenths,
  hundredths or thousandths of a second.

  Values are stored in the database as an integer value.  A time like 1:15.20
  is stored in the database as 7520.  The module converts the time to the
  number of seconds and then multiplies it by the number of decimal places
  configured for the field. The value is stored in a database field called
  time.

USAGE

  Times can have 1, 2 or 3 decimal places representing tenths, hundredths or
  thousandths of a second.  The default is 2 (hundredths).

  When adding this field to a list, you need only supply the number of decimals
  the times will use.  When entering or changing a time in the field, a
  javascript function is called as soon as editing is done that will format the
  time correctly. If you want to enter 1:25.75, you have the option of entering
  the time as 12575 and the javascript function will format it properly.  If
  you enter an improper character, such as a letter or a semi-colon, the script
    will display a warning.

  If you plan to program with this field, two very useful functions that come
  with this module are
    * _racetime_string_to_int
    * _racetime_integer_to_formatted

  Both functions can be used throughout your site to convert numbers from an int
  to a string or string to an int.

  Both functions require a time and an optional number of decimals.  The number
  of decimals defaults to 2, but if your times use only tenths or thousandths of
  a second, you will want to specify 1 or 3 for this value.

  When creating a node programmatically, be sure to call the
  racetime_string_to_int function on the entered time to ensure that the time
  is stored correctly.

  $time = _racetime_string_to_int($form_state['values']['field_racetime'],2);
  $node->field_racetime[$node->language][]['time'] = $time;


INSTALLATION

  Installation is like with all normal drupal modules:
  extract the 'racetime' folder from the tar ball to the
  modules directory from your website (typically sites/all/modules).

DEPENDENCIES

  Because this module creates a field, the module depends on the existence
  of field_ui
