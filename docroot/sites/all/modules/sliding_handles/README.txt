  -- SUMMARY --

  This is a simple module that adds an empty <span> tag to a selected menu.
  The span can then be styled separately from the li and a elements.
  It is named after the Sliding doors technique posted by Douglas Bowman
  on http://alistapart.com/article/slidingdoors

  Normally this would be done with javascript or a template.php override,
  but this is for the special case where you either don't do that or
  aren't allowed to do that.

  For a full description of the module, visit the project page:
  http://drupal.org/project/sliding_handles

  To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/sliding_handles


  -- REQUIREMENTS --

  * You have the core menu installed.

  -- INSTALLATION --

  * Install as usual, see http://drupal.org/node/895232 for further information.



  -- CONFIGURATION --

  * Configure user permissions in Administration > People > Permissions:

  - Administer Sliding handles

  Required to change the target menu.

  Select the target menu at admin/config/user-interface/sliding_handles
  to specify the menu where you want the span to appear.
  e.g. main-menu  This must be a valid existing menu machine name.

  Click Save configuration and reload your page with the menu enabled.

  -- CONTACT --

  Current maintainers:
  * Arne Olafson - http://www.drupal.org/user/661268
