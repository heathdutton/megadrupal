CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Contact

INTRODUCTION
------------

PathInfo shows the function that the current page url is served by.

Information is shown in the footer of HTML pages, after any other content.
It is added to the page by registering a shutdown function (just like
devel's querylog), so will appear outside of any content rendered by theme('page').

INSTALLATION
------------

Enable the module as normal. By default, the footer information is shown.
If you want users other than user 1 to see path info, give them the 'view
pathinfo' permission.

You can also disable output without disabling the module at admin/config/pathinfo/config.

CONTACT
-------

Current maintainers:
* Cameron Tod (cam8001) - http://drupal.org/user/129588

