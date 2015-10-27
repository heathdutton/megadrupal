--------------------------------------------------------------------------------
  ajax_screen_lock module Readme
  https://www.drupal.org/project/ajax_screen_lock
--------------------------------------------------------------------------------

Contents:
=========
1. ABOUT
2. INSTALLATION
3. REQUIREMENTS
4. CREDITS

1. ABOUT
========

This module simply locks screen when AJAX is running on page.
You can prevent unwanted user actions while the AJAX request is performed.
Popup appears automatically when AJAX query is sent.

2. INSTALLATION
===============

1. Install as usual, see http://drupal.org/node/70151 for further information.
2. Go to the configure page admin/config/user-interface/ajax-screen-lock and configure module.
3. Install library jquery.blockUI.js into directory sites/all/libraries/jQuery.blockUI/jquery.blockUI.js

  Now it's available here http://malsup.com/jquery/block/#download
  Or here https://github.com/malsup/blockui/

4. Got to the status page (admin/reports/status) and check if all is ok ;)

Contact me with any questions.

3. REQUIREMENTS
===============

You should install Libraries API (https://www.drupal.org/project/libraries).

4. CREDITS
==========

Project page: http://drupal.org/project/ajax_screen_lock

Maintainers:
Eugene Ilyin - https://www.drupal.org/user/1767626
