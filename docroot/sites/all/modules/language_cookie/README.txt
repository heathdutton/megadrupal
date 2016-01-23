CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Usage
 * Support
 * Compatibility
 * Authors
 * Sponsors


INTRODUCTION
------------

Adds an extra "Cookie" field to the Language Negotiation settings, allowing the
language to be set according to a cookie.

The cookie name & expiration date are configurable in the settings page.


USAGE
------

- Enable the module and go to: Administration » Configuration » Regional and
  language » Languages » Detection and selection (available at the following
  URL: 'admin/config/regional/language/configure').

- Enable the "Cookie" detection method and re-arrange the user interface language
  detection as you see fit. The language cookie will be set to whichever
  language the user interface turns out to be in. The recommended setup is (from
  top to bottom):
    URL -> Cookie -> Language Selection Page -> Default.

  Just to illustrate, another possible setup is:
    User -> Session -> Cookie -> Default

- For the cookie to be set properly on cached pages, the variable
  "page_cache_invoke_hooks" has to be set to TRUE.
  This can be done by adding the following line to your settings.php file:
  $conf['page_cache_invoke_hooks'] = TRUE;

- If you use URL-based caching such as Varnish and want the cookie to be set on
  all pages, there is a setting for this in the administration interface: Go to
  'admin/config/regional/language/configure/cookie' and check the relevant
  checkbox. If you want cookie-based automatic language selection without
  invoking Drupal, you will need to implement a Javascript based solution which
  reads the cookie and redirects the user to the right page.


SUPPORT
-------

If you find a bug or have a feature request please file an issue:

http://drupal.org/node/add/project-issue/language_cookie


COMPATIBILITY
-------------

A lot of work has gone into ensuring maximum compatibility with other contrib
modules. If you find a bug please use the issue tracker for support. Thanks!


AUTHORS
--------

* Alex Weber (alexweber)
  http://drupal.org/user/850856 & http://www.alexweber.com.br


SPONSORS
---------

This project is made possible by Webdrop (http://webdrop.net.br)
