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

See https://docs.acquia.com/cloud/configure/mobile for more information.
