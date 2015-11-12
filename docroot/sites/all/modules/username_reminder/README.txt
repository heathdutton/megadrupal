INTRODUCTION
------------
Users sometimes forget their usernames. Username Reminder provides a form for
requesting a username reminder, which is linked from the user login block and
the user page. The user who has forgotten their username can enter their e-mail
address into the form, and if it matches an active account, the account
username is e-mailed to the e-mail address.


CONFIGURATION
-------------
The subject and body of the reminder e-mail are customizable via Administration
» Configuration » People » Username Reminder. Token replacements are
supported.


OPTIONAL MODULES
----------------
* Token (https://www.drupal.org/project/token):
  When enabled, the administrator can browse for tokens to use in a customized
  reminder e-mail.


CREDITS
-------
Thanks to Steve Krzysiak for the Forgot Username module [0], which was a useful
reference for designing and implementing this module.

[0] https://github.com/stkrzysiak/forgot-username-drupal-module
