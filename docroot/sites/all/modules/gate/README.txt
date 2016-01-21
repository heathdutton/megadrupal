Gate for login- and mail-process.

This module provide functionality to improve security and mail handling.

# THE MODULE IS IN EARLY STATE OF DEVELOPMENT
#
# Bigger changes in Development:
# There are still API changes. With commit on 2013-10-04 the variable logic was
# changed to drupal standard (0 = blocked, 1 = unblocked). There is no update
# routine for this. Please check your settings with "drush gate-status"
# after update and correct this setting manually. For the introduced database
# scheme there is an update routine which should be triggered with /update.php
# or "drush updatedb".
# The drush commands are now following the "gate logic" with "open and closed".
# The gates "onetimelink-adminrole" and "pwlogin-adminrole" can't block user 1.
# For this the gates "onetimelink-user1" and "pwlogin-user1" have to be closed
# separately.

The main module allows a simple way to block password-login for user 1 and
admin role. This is useful if administrators have access to drupal via drush
and the admin processes should be organized only with drush.
If the "direct-login" or the "direct-mail" gate are closed, there there are
"keys" needed to move on.
For an advanced security and mail controlling strategy there is a submodule
for rules integration. There are rules actions provided to open the gates in
multifactor-login processes and mail sending based on your own rules.

Drush is always important to take control of this module. For some settings
there is no graphical user interface to help avoiding an accidental complete
lock out when drush isn't available.
The status of the main open/close variables can be checked by
"drush gate-status" and can be changed with the commands "drush gate-open" and
"drush gate-close". For opening the direct-mail gate with drush  there is the
command "drush gate-uli" to add generate gate login data and add a gate-token
to the onetime-login link.

This module is operating on hook_user_login, hook_user_mail to operate on mail
and login process. Also the forms 'user_login', 'user_login_block' and
'user_pass' are altered. There are rules events for all these hooks and rules
actions to operate there.

WARNING:
This module allows several way for a complete lock-out of your system. Using
drush is strongly recommended. If drush is not available you need a direct
access to your database and how to modify variables.
There are some simple security options like blocking the password or request
forms for passwort reset/onetime-login links. If there are other ways in your
system to login via password or send a onetime login link the gate can be
be bypassed. Please check and maybe use the direct-login gate to control on a
deeper level.
The direct-mail gate is only operating on the internal mail system of drupal.
Contrib or custom modules can use their on mail system or do a direct call
of the PHP mail function. Against the last possibility to bypass the gate you
can control the mail sending via php.ini and route the mails through a
separate script where gate mails can get the possibility to pass. For this
there will be an optional mail header setting for checking in this external
script.

TIP:
If you want to close the gate for onetime logins and you are thinking about how
to send password recovery mails on another way to your users you can use rules
with default rule action "Send account email" with type "Password recovery".
A link on user profiles created by the module rules_link and only accessible by
administrators can help with that.
