LiveReload
http://drupal.org/project/livereload
====================================


DESCRIPTION
-----------
LiveReload is a Desktop app + Safari/Chrome/Firefox extension that:
1. Applies CSS and JavaScript file changes without reloading a page.
2. Automatically reloads a page when any other file changes
  (html, image, server-side script, etc).

This module adds a LiveReload javascript to your page, aiming the functuallity
of the Browserplugin. This means that no plugin is needed to use this.
Additionally it gives you the possibilty to load styles with tags instead of
@import, this might be an issue in firefox and orher oler browsers.


REQUIREMENTS
------------
Drupal 7.x
livereload - http://livereload.com/


INSTALLING
----------
1. To install the module copy the 'livereload' folder to your sites/all/modules
   directory.
2. Read more about installing modules at http://drupal.org/node/70151


CONFIGURING AND USING
---------------------
1. Go to admin/people/permissions to give the "Use LiveReload" permission to
   selcted roles.
1. Go to admin/config/development/performance to disable livereload.js manually
   and to force the css to be rendered with <style> instead of @import.


REPORTING ISSUE. REQUESTING SUPPORT. REQUESTING NEW FEATURE
-----------------------------------------------------------
1. Go to the module issue queue at http://drupal.org/project/issues/livereload?text=&status=All
2. Click on CREATE A NEW ISSUE link.
3. Fill the form.
4. To get a status report on your request go to http://drupal.org/project/issues/user
