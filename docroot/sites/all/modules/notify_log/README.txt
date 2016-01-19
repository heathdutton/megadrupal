--------------------------------------------------------------------------------
  notify_log module Readme
  http://drupal.org/project/notify_log
--------------------------------------------------------------------------------

Contents:
=========
1. ABOUT
2. INSTALLATION
3. REQUIREMENTS
4. CREDITS

1. ABOUT
========

The module is designed to help developers,
informing them of the new entries in the log.

The module provides two types of notifications.
One of them works only on Linux operating system
and requires installation of package "libnotify-bin", the second way works via
jQuery library Growl.

2. INSTALLATION
===============

1. Install as usual, see http://drupal.org/node/70151 for further information.
2. Go to the configure page and select comfortable for you way of notifications.
3. You can configure the plugin(Growl) at your convenience.

  If you prefer the system notification, you should use Linux, install
  notify-send (libnotify-bin) package and give to your web serves's user
  access to use it, so add "www-data ALL=(ALL) NOPASSWD: /usr/bin/notify-send"
  string to end of /etc/sudoers file.
  Instead "www-data" you should use username
  of you web-server user ('nginx' for nginx, 'www-data' for apache, etc).

  Other way is use jQuery Growl library, so got to the library site
  and download it: http://ksylvest.github.io/jquery-growl and extract
  to libraries path (sites/all/libraries) by default.

4. Got to the status page (admin/reports/status) and check if all is ok ;)

Contact me with any questions.

3. REQUIREMENTS
===============

You should install Libraries API (https://www.drupal.org/project/libraries).
Also you should to update jQuery version via jQuery update module
(https://www.drupal.org/project/jquery_update) and install
the jQuery Growl library (http://ksylvest.github.io/jquery-growl) OR install
the libnotify-bin packadge.

4. CREDITS
==========

Project page: http://drupal.org/project/notify_log

- Drupal 7 -

Authors:
* Dmitry Kiselev (kala4ek) - https://www.drupal.org/user/1945174
