CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------
Why?
Because GET is much better for caching. If you're using something like Varnish,
or Boost, these (by default) will not store POST requests (and so they
shouldn't).

Views uses the Drupal Ajax framework. This by default uses POST.
While a lot of Ajax inside of Drupal requires POST (think the Views UI, for
example), a view itself doesn't. If you disable Ajax on a View, it will use
GET anyway.

How?
This module overrides a core Drupal Ajax JavaScript function (this is sometimes
referred to as monkey-patching).

Also...
There are a couple of other reasons the Drupal Ajax framework uses POST. Which
have had to be removed when using GET:

1)
  To ensure valid HTML markup, every HTML id on the current page is sent with
  the POST request. Drupal can then check it is not creating duplicate id's
  (using drupal_html_id()). Sending every id in the request makes it too large
  for GET. So this is removed.
2)
  To ensure an Ajax request can add new JS and CSS files to the current page,
  the filename of every js and css file is sent with the request. These again
  make the request to large for GET, and so is removed.

Note
If you are having any issues with a View because of this, you can set the View
to be exempt from using GET, by going to admin/config/system/views_ajax_get.

Also, if you are using Varnish with the Four Kitchens VCL, or something similar,
you will need to remove the following line
  req.url ~ "^.*/ajax/.*$" ||
Otherwise, Varnish will not cache the Views Ajax responses.

REQUIREMENTS
------------
This module requires the Views module.

INSTALLATION
------------
 * Install as usual, see http://drupal.org/node/70151 for further information.

CONFIGURATION
-------------
 * Once this module is enabled, all Views with Ajax enabled will use GET instead
   of POST.
 * If a View is causing issues, you can have this exempt from using GET.  This
   can be configured at admin/config/system/views_ajax_get

MAINTAINERS
-----------
Current maintainers:
 * Leon Kessler (leon.nk) - http://drupal.org/user/595374
