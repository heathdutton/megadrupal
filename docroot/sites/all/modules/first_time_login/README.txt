-------------------------------------------------------------------------------
First time login for Drupal 7.x
  by Neeraj Singh - neerajprasadcse (at) gmail (dot) com
-------------------------------------------------------------------------------

DESCRIPTION:

First time login module prompts user to reset their profile,
when they login to their account for the first time (once updated it will not
prompt from next login onwards).

After creating a user account with basic details,
user can be intimated to log on to their profile and ask them to update it.
Where in the user can update their account passwords also.

You can configure the threshold number of days after,
which user will be again prompted to update their profile.

Timestamp for the existing users will be updated to their last access time,
at the time of module installation.

NOTE:
Super user (user with UID = 1), will not be prompted for profile update.
Default threshold number of days after which user will be again prompted to
update their profile is set to 120 days.
-------------------------------------------------------------------------------

INSTALLATION:
* Put the module in your Drupal modules directory and enable it in
  admin/modules.
* Using Drush :: drush pm-enable first_time_login
-------------------------------------------------------------------------------

MAINTAINERS
-----------
Current maintainers:
 * Neeraj Singh (neerajsingh) - https://www.drupal.org/user/3202261
