
Description
===========
Module for working with variables.
Analog built-in functions Drupal varible_set() / variable_get(), but with differences:
1. variables are stored in a separate table and are loaded only when necessary
   (which does not load the memory);
2. variables may belong to specific users;
3. variables may be dependent on the session;
4. variables can be temporary (delete by Cron);
5. variables can be global.


Requirements
============
Drupal 7.x


Installation
============
1. Copy the entire user_variable directory the Drupal sites/all/modules directory.
2. Login as an administrator. Enable the module in the "Administer" -> "Site
   Building" -> "Modules"
3. Use the module through the API functions.


API functions
=============
user_variable_set($name, $value, $common = FALSE, $uid = 0, $expired = 0, $session = FALSE)
Set a variable if it does not exist, then create.

    $name - Variable name.
    $value - Variable value.
    $common - If TRUE then the variable does not depend on user ID.
    $uid - User ID.
    $expired - The lifetime of a variable (in seconds) if equal 0 then the
        variable is not deleted automatically.
    $session - If TRUE then the variable is tied to the session when set,
        the variable is tied to the value (eg, session ID).


user_variable_get($name, $common = FALSE, $uid = 0, $session = '')
This function return variable value.

    $name - Variable name.
    $common - If TRUE then the variable does not depend on user ID.
    $uid - User ID.
    $session - Session ID.
