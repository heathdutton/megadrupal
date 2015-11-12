-------------------------------------------------------------------------------
XING CONNECT MODULE for Drupal 7.x
  by Ajay Nimbolkar - ajaynimbolkar (at) gmail (dot) com
-------------------------------------------------------------------------------

DESCRIPTION:
------------
What does this module do?
* Allow users to register with Xing, their usename, email, profile pic can be
synced to their Drupal account.
* Allow users to login with Xing.

How it works
User can click on the "login with XING" link on the user login page/ User login
block

When the user click the "login with XING" link, it automatically takes
user to Xing and asks for his permission. Once granted the module checks
the users email. If the email address is found on the Drupal Site, he is logged
in automatically. Otherwise a new user account is created with the email address
and the user is logged in.
-------------------------------------------------------------------------------

INSTALLATION:
-------------
* Put the module in your Drupal modules directory and enable it in
  admin/modules.
* Using Drush :: drush pm-enable xing_connect
-------------------------------------------------------------------------------

CONFIGURATION:
-------------
1) Create a new app on https://developers.xing.com/apps
This will give you an App ID/API Key and an App Secret
2)Open Configuration form such as admin/config/people/xing-connect
3) Enter your app id and secret key obtained from Xing app into the system
setting form, "post url " textbox for redirection after user login/register
and save your settings.

MAINTAINERS:
------------
Current maintainers:
 * Ajay Nimbolkar (ajayNimbolkar) - https://www.drupal.org/u/ajaynimbolkar
