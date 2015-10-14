INTRODUCTION
------------
Role Expose -module introduces a tab on user profile page which lists user's
roles. Site administrators may choose which roles to list on the page.
Users may be granted an option to view own roles or all users roles.

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
* Configure user permissions in Administration » People » Permissions.
  Suggested permissions are:
  - For authenticated users: View own roles
  - For user admins: View roles of all users

* Customize exposed roles below Role management area in
  Administration » People » Permissions » Roles
* Exposed roles are visible in user profile page for users with permissions with
View own roles or View roles of all users (for own or all users, respectively).

TODO
----
Add support for Apply for role https://www.drupal.org/project/apply_for_role
to allow easy requests for roles user does not yet have.

MAINTAINERS
-----------
Current maintainer:
 * Perttu Helle (rpsu) - https://drupal.org/user/102121

This project was originally empirical part of a bachelor’s thesis:
* Subject Automatic testing in Drupal 7 module development"
* @see http://urn.fi/URN:NBN:fi:amk-2011112916150
  (in Finnish, abstract in English)
