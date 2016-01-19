README.txt
-------------------------------

This module sends data to google analytics and provides an API to integrate with
other modules.

--------------------------------------------------------------------------------
 METHODS:
--------------------------------------------------------------------------------

UNIVERSAL ANALYTICS
-------------------

Requeriments: Google Analytics 7.x-2.x https://drupal.org/project/google_analytics

- UTMP-JS:  It sends the data on client-side (next page load).
- UTMP-PHP: It sends the data on server-side.

CLASSIC ANALYTICS
-----------------

Requeriments: Google Analytics 7.x-1.x https://drupal.org/project/google_analytics

- JS: It sends the data on client-side (next page load).

- PHP: It sends the data on server-side. One of these libraries are required:

  - SSGA: Deprecated library(2009) to PHP 5.2+ versions
    You don't need to download this library, a modified version of this library 
    that implements curl is included on this module.
    Forked from: http://code.google.com/p/serversidegoogleanalytics/

  - PHP-GA: Recomended library only for versión 5.3+
    Requires PHP 5.3 as namespaces and closures are used.
    Download from: https://github.com/thomasbachem/php-ga
    Use the default libraries directory, usually "sites/all/libraries/",
    naming the folder "php-ga" (sites/all/libraries/php-ga).

    Example:
      /sites/all/libraries/php-ga/src/autoload.php
      /sites/all/libraries/php-ga/src/GoogleAnalytics

--------------------------------------------------------------------------------
 FEATURES:
--------------------------------------------------------------------------------

- Rules integration.
- Browser events.
- Form validate error events.

--------------------------------------------------------------------------------
 API USAGE:
--------------------------------------------------------------------------------

To push a custom query to google analytics see ga_push() function documentation.
