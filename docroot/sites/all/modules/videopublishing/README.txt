-- SUMMARY --

VideoPublishing.com is a leading provider of high quality, online video publishing 
solutions for businesses, publishers, video professionals and website owners.
This module provides integration with the VideoPublishing API bringing specific 
functionality to your Drupal installation.


-- REQUIREMENTS --

PHP 5.1.2
php_curl extension

-- INSTALLATION --

1. Create a 'VideoPublishing' subdirectory in the videopublishing module directory
2. Download the VideoPublishing API (PHP Library) from http://videopublishing.com
 - see: http://videopublishing.com/web-developers.html
3. Copy the 'library' directory in the 'VideoPublishing' subdirectory 
 - result: videopublishing/VideoPublishing/library
4. Install as usual
 - see: http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Permissions >>
  videopublishing module:

  - Administer videopublishing
    Users in roles with the "administer videopublishing" permission will be able
	to access the sitewide configuration page.

  - View videopublishing errors
	Users in roles with the "administer videopublishing" permission will be able
	to see VideoPublishing API erros messages. This may come in handy for module
	maintainers, developers or site administrators.

* See admin/config/media/videopublishing for the site-wide configuration page.