
SUMMARY
------------

The Skype Status module allows you to display users' Skype online presence
information in their profile via Skype's public presence service and provides
also an individual block for site wide usage.


KNOWN PROBLEMS
--------------
Skype's presence service returns an "Unknown" status unless you've enabled
"Allow my status to be shown on the web" in your Skype client's privacy
options.


INSTALLATION
 ------------

  1. Copy all the module files into a subdirectory called
      sites/all/modules/skype_status/ under your Drupal installation
      directory.

  2. Go to Administration >> Modules and enable the Skype Status module.

  3. Go to Administration >> Configuration >> People >> Account settings
      >> Manage fields and create a new single-line textfield that will hold your
      users' Skype IDs. You can name and title this field anything you like, but
      make sure that the field is displayed in the user profile.

  4. Go to Administration >> Configuration >> People >> Skype status settings
      to review and change the configuration options to your liking. Specifically,
      select the profile field that you created in the previous step. You may also
      hide the title of the field if you so wish.

  5. Go to My account, click Edit, input your Skype ID into the profile
      field you created in step 3, and click Save; when your profile page is
      displayed, you should see your Skype status and your Skype ID should
      have become converted to a skype:// link.

  6. (See README.txt for information on submitting bug reports.)


FURTHER USAGE
-------------

The module does currently support views integration. You will see display
options for both "Skype Button" and "Skype ID (plain text)" when using
the Skype ID field within views displays.


BUG REPORTS
-----------
Post bug reports and feature requests to the issue tracking system at:

  <http://drupal.org/node/add/project_issue/skype_status>


CREDITS
-------
Developed and maintained by:
  Nicholas Alipaz <http://nicholas.alipaz.net/>
  Abdelatif Sebbane <http://drupal.org/user/499616>
Sponsored by:
  Stitch Technologies <http://www.stitch-technologies.com/>


HISTORICAL CREDITS
------------------
Original developers:
  Arto Bendiken <http://bendiken.net/>
  Miglius Alaburda <http://drupal.org/user/18741>
Original sponsors:
  MakaluMedia Group <http://www.makalumedia.com/>
  M.C. Dean, Inc. <http://www.mcdean.com/>
  SPAWAR <http://www.spawar.navy.mil/>