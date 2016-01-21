Adaptive Image Styles
=====================

Description
-----------

This module allows any image to be adaptive simply by setting it to be
rendered with the 'adaptive' style.  This module also integrates with
the Media module, and adds an adaptive option selectable from the media
displays.

Which image styles are used to generate the adapted images is also
configurable.  The adaptive image styles module offers an easy way to
make any images adaptive.

Simply choosing to display the image with the "adaptive" image style with
make the image adapt to the clients window width.  The Adaptive Image
Styles administration page provides an interface for choosing which image
styles are using to generate the adapted images and set the threshold
for when they are chosen.  The threshold refers to browser window width.

Example:
  If there are three image style selected, with thresholds
    of 480, 768, and 992.
  If the window width is from 0 - 480 pixels, the adaptive image will be
    displayed using the image style with a threshold of 480.
  From 481 - 768, the 768 image style will be used, and any wider than 769,
    the image style with the threshold of 992 will be used.


Caveats
-------

AIS works using an htaccess rewrite rule when using Apache or web server configuration when using NginX.

If a client has JavaScript disabled, the original image will be displayed.


Non-Apache / NGINX server configuration references
--------------------------------------------------

Web tool to convert htaccess rules to NGINX rules:
	http://winginx.com/htaccess

Related, solved issues:

Validate NginX rules:
#1669182: Running this module on a non-apache server
	https://drupal.org/node/1669182

Octopus / NGINX:
#1452732: adaptive image styles for my responsive design.
	https://drupal.org/node/1452732


Installation
------------

  1. Enable the module
  2. Patch Drupal's .htaccess file
  3. Correctly configure the RewriteBase in Drupal's .htaccess file.
  4. Display your images and media with the 'adaptive' image style
  5. Configure which images styles are used by AIS


To patch Drupal's htaccess file with patch, run the following from you
Drupal root:

$ patch < sites/all/modules/ais/ais.htaccess.patch

To patch Drupal's htaccess file manually:

Add following:

# AIS: Adaptive Image Style
  RewriteBase /
  RewriteCond %{REQUEST_URI} ^(.+)/files/styles/adaptive/(.+)$
  RewriteCond %{REQUEST_URI} !/modules/image/sample.png
  RewriteCond %{HTTP_COOKIE} ais=([a-z0-9_-]+)
  RewriteRule ^(.+)/files/styles/adaptive/(.+)$ $1/files/styles/%1/$2 [R=302,L]

Into the .htaccess file before the following lines:

 # Pass all requests not referring directly to files in the filesystem to
 # index.php. Clean URLs are handled in drupal_environment_initialize().
 RewriteCond %{REQUEST_FILENAME} !-f
 RewriteCond %{REQUEST_FILENAME} !-d
 RewriteCond %{REQUEST_URI} !=/favicon.ico
 RewriteRule ^ index.php [L]

NOTE:
You must set the RewriteBase correctly.
If Drupal is at the root of your domain, it should be set to "RewriteBase /".
If Drupal is in a directory (like http://www.example.com/mydirectory),
it must be set to "RewriteBase /mydirectory".

If you've modified your .htaccess file correctly (and you have no other
modification), your .htaccess file should look like the "ais.htaccess.example"
file included with this module.


Running AIS on NginX
--------------------

The following NginX configuration should work with AIS:

location ~* /(?:.+)/files/styles/adaptive/(?:.+)$ {
  if ( $http_cookie ~* "ais=(?<ais_cookie>[a-z0-9-_]+)" ) {
    rewrite ^/(.+)/files/styles/adaptive/(.+)$ /$1/files/styles/$ais_cookie/$2 last;
  }
  access_log off;
  add_header X-Header "AIS Generator 1.0";
  set $nocache_details "Skip";
  try_files  $uri @drupal;
}


Author
------

David Pascoe-Deslauriers, Sponsored by Coldfront Labs Inc.

