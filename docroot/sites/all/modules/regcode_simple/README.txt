INTRODUCTION
------------
Registration code (simple) is a lightweight module which allows site administrator
to set a (global) code to allow only users with this code to register user
account on the site. This is useful if site owner invites certain users but does
not know their email addresses to manually create user accounts or use
invitation modules to take care of inviting users.

If you need a more comprehensive solution to use personalized codes, need to
limit usage of a code or want to map codes to user roles, you may find solution
with Registration Codes https://www.drupal.org/project/regcode


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
* Set the code in Administration » People » Permissions » Account settings
or http://example.com/admin/config/people/accounts#edit-registration-cancellation.

After the code is set User registration form will require users to use code to
allow registrations to the site.

MAINTAINERS
-----------
Current maintainer:
 * Perttu Ehn (rpsu) - https://drupal.org/user/102121

