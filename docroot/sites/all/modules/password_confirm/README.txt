***********
* README: *
***********

DESCRIPTION:
------------
This module provides an password_confirm field type.


INSTALLATION:
-------------
1. Place the entire password_confirm directory into your Drupal.
	sites/all/modules/ directory.

2. Enable the password_confirm module by navigating to:
     administer > modules

Features:
---------
  * Fields similar to the password field used in user registration form.
  * Javascript based on line validation.
  * Save Password settings for
      o Drupal based encryption for password value
      o Plain text password value.
  * Show Password on node form
      o Plain Text
      o Dotted Value

Note:
-----
To enable encryption of password, see settings. 
Once you choose to encrypt password you should not use show password, else
it will show the hash value of password.

Author:
-------
Pranit Kumar Jha
dreampranit@gmail.com
