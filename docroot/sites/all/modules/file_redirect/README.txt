File Redirect, v6.x-1.x
================================================================================
Author: Chris Albrecht (KeyboardCowboy)
  - chris@162unlimited.com
  - @ChrisAlbrecht
  - http://drupal.org/user/176328

Description
--------------------------------------------------------------------------------
When given the host URL of your production site, this module will add a snippet
into the .htaccess file at your files directory root and redirect requests to
your production site if:

1. The hostname of your production site does not match the hostname entered in
   the UI. (prevents recursion)

2. The request is looking for a file inside the files directory you specified in
   the File System settings.

3. The requested file does not exist.

This prevents files from being duplicated and gives the illusion that files
exist on each server you have implemented this module.

It is not a 100% solution, but it is close. I haven't tested all use cases, so
please report any cases that don't work for you in the issue queue.


Installation
--------------------------------------------------------------------------------
1. Assign the permissions for the file_redirect module
2. Go to:
  - D6: Site Configuration > File System > File Redirect
  - D7: Configuration > Media > File System > File Redirect
3. Enter the full URL to the root of your production site

That's it! This will not work on your actual production site to prevent
recursion, but will work on any dev/test/staging/etc sites.
