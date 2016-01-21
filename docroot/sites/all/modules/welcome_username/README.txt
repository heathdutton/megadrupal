This module renders a block that shows the login form to anonymous users
and a "welcome username" with logout link block to authenticated users.

The string "Welcome" and "logout" are translatable with the t() function.

To change the strings "Welcome" and "logout" save these variables in the
variable table:
welcome_username_welcome_string
welcome_username_logout_string

To change "Welcome" to "Hi":
$ drush vset welcome_username_welcome_string "Hi"

To change "logout" to "Click here to logout"
$ drush vset welcome_username_logout_string "Click here to logout"
