403 to 404

This is a simple module that emits a 404 error when a user tries to access
a page that they don't have permission to view.

Install this module like any other module and set the 403 page path to m4032404.
You configure the module via one of the following methods:

* Run drush vset --exact -y site_403 m4032404

* Set $conf['site_403'] = 'm4032404'; in your settings.php

* Default 403 in admin/config/system/site-information to m4032404

This module is designed to do one thing, and one thing only.  Don't expect 
anything fancy.

403 to 404 was developed by [Dave Hall Consulting](http://davehall.com.au).
