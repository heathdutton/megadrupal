The Basics
==========
Social Connect module allows users to login on a Drupal website 
through the Facebook and Hyves – using their Facebook or Hyves login and password.

The module also brings other extra features:

Admin can allow user profile fields update with Hyves and Facebook data
Admin can configure field mapping and customize login buttons
NOTE! HyvesConnect JS supports only IE8 or greater

Differences from other modules:

Standalone (not need any library)
Flexible
Small and simple
Basic support Domain Access module


Requirements
============
Core module: field module.


Installation
============
Install as usual, see http://drupal.org/node/70151 for further information.


Settings
========
* admin/config/people/social_connect
Module settings page where you can enable Hyves/Facebook Connect and configure it.
You need to create new Hyves/Facebook application or edit existing.
Hyves application consumer key and secret can be found here: http://www.hyves.nl/developer/applications/
Facebook applocation ID and secret can be found here: http://www.facebook.com/developers/apps.php


* admin/config/people/social_connect/mapping
Module settings page where you can configure field mapping. Fields that used for login (Facebook UserID and Hyves Username) is recommended to be mapped.
