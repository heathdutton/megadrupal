INTRODUCTION
------------
Cache Bypass Path is a lightweight module which simply provides a non cached
path accessible only by a hash. This is intended for Drupal installations that 
have a Varnish Caching Layer. Without the correct hash the path is simply a 404. 
With the hash, a request can be made directly to Drupal, bypassing the VCL. This 
is useful for site health checks whe varnish caching is on.

REQUIREMENTS
------------
none

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

1. Navigate to admin/config/development/cache-bypass-path to see the current
cache bypass path. You can also regenerate it here.

 MAINTAINERS
-----------
Current maintainers:
 * Michael DeWolf (mrmikedewolf) - https://drupal.org/user/2679073
