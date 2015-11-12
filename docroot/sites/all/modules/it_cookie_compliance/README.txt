INTRODUCTION
------------

This module extends EU Cookie Compliance module to be compliant with the Italian
law, which requires the preemptive blocking of cookies when the user has not
yet accepted the popup conditions.

It adopts a lightweight fully server-side approach to exclude Javascript files
blocks, and node fields, from any rendered page until the cookie policy has been accepted.

INSTALLATION
------------

1. Install and configure EU Cookie Compliance so that the popup is enabled
and viewable.
See https://www.drupal.org/project/eu_cookie_compliance.

2. Install IT Cookie Compliance.

3. Go to the admin/config/system/it_cookie_compliance page to set the
javascript, blocks and fields to be excluded before the acceptance. 

CACHED PAGES
------------

There's some tweak to be done when a page cache is set to get everything work 
correctly. It does not seem possible to get it work by code only with the 
standard Drupal cache working  without patching the core. 
See also https://www.drupal.org/node/322104.

A simple solution is to fool the caching system with some redirect when the 
acceptance cookie is not found.
Add those rewrite rules in your .htaccess, before the all-redirect to index.php:

RewriteCond %{REQUEST_FILENAME} !-f
RewriteCond %{REQUEST_URI} !=/favicon.ico
RewriteCond %{HTTP_COOKIE} !cookie-agreed=2 [NC]
RewriteCond %{QUERY_STRING} !cookie-not-accepted=1 [NC]
RewriteRule ^ %{REQUEST_URI}?cookie-not-accepted=1&%{QUERY_STRING} [L,R]
