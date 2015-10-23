
-- OVERLAY THEME DOCUMENTATION --


-- SUMMARY --

The Overlay theme module provides an option to set the theme of modal frames
provided by the core Overlay module, different from the default which is the
Administration theme. This is useful for example if you want to exploit the
overlay to provide inline editing of content to end users, without exposing them
to the bare-bones styling of a typical administration theme.

Project page:
  http://drupal.org/project/overlay_theme

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/overlay_theme


-- REQUIREMENTS --

* Drupal 7.
* Core module Overlay must be enabled.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Customize the Overlay theme settings in Administration » Appearance.
  You can choose an overlay theme and limit its diplay to a list of paths in the
  same way you limit the display of Blocks.


-- TROUBLESHOOTING --

* Only active themes can be used as overlay themes, so if you don't see your
  theme listed in the Overlay theme settings, check that it is active.

* Overlay theme tries its best to determine if a page will be displayed using an
  overlay, but the detection is not bulletproof since Drupal initializes the
  current theme before the overlay. In some edge cases, this module might fail
  to select the right theme. If you encounter one such case, please file an
  issue.


-- CONTACT --

Current maintainers:
* Gregorio Magini (peterpoe) - http://drupal.org/user/55674


This project has been sponsored by:
* Work Hard Pilosophy - Web and interactive media.
  Visit http://workhard.ph for more information.

