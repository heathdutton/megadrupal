Tivoli Access Manager Authentication Module for Drupal 7

This module allows site users to authenicate using Tivoli Access Manager (TAM),
when a user visits user/tamauth and they have the appropriate headers in their 
request.

When a user visits the TAM url they will be authenticated and have their roles
synced from the headers to Drupal.

All configuration variables must be set in the $conf array in your
settings.php. An admin user interface _may_ be added in the future. A patch
that implements this will be welcome.

** Configuration Variables **
tamauth_map_roles
An array that maps TAM roles to drupal roles.

tamauth_redirect - default: admin
The URL users are redirected to after successfully authenticating.

** Important Security Information **
This module assumes that all TAM headers are legitimate. It is not recommended
to use this module on a public website - it is designed for internal user 
access. If you must use this module with a mixed authentication scheme, you
should use Apache's mod_headers or equivelant functionality to strip headers
or LocationMatch to limit access to the TAM URL.
