In the Acquia Cloud platform, you can redirect site visitors in the Varnish
layer, based on the devices they are using, as determined by the user-agent
HTTP header in the request. You can do this by using your web site's .htaccess
file to set HTTP headers in the response.

However, when you configure Acquia Cloud to use PHP-FPM, the .htaccess file is
no longer able to set HTTP headers and these redirects stop working. To get the
redirect headers to output again, you'll need to generate them in PHP code.

This module uses Drupals hook_boot() to check the current hostname against
configured mobile, tablet and desktop URLs and sets the correct varnish
headers on the page to provide device-specific redirects through Varnish.

By using this module, you don't need to add code to your settings.php file
and the configure device URLs automatically become exportable in a feature
if you have enabled the the strongarm module.

The module provides a block with links to each of the device specific site
URLs.


INSTALLATION
------------

1. Download the module and extract it to sites/all/modules or install it
   using `drush dl acquia_mobile_redirect'.


CONFIGURATION
-------------

1. Go to admin/config/system/acquia-mobile-redirect and enter device-specific
   URLs for the desktop, tablet and mobile versions of the site. Leave any of
   the fields blank to not output a URL for that device type. Device switching
   now works!

2. Optionally, configure the "Acquia Mobile Redirect Links" block and place it
   somewhere on your site. This block allows users to override the device
   selection.


THEMING
-------

You can target div#block-acquia-mobile-redirect-links to style the mobile
redirect block. Each of the device-specific list items has the device name as
a class you can use to target it. The links themselves have an ID that is used
by the redirection JavaScript, so only change the IDs if you change the JS too.


MORE
----

See https://docs.acquia.com/cloud/configure/mobile for more information about
device detection and redirection on Acquia.
