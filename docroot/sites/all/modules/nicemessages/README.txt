Version: nicemessages-7.x-1.x-dev

Description
============

Nicemessages module provides displaying drupal messages in nice popups using
the jQuery jGrowl plugin (http://stanlemon.net/projects/jgrowl.html).

Module written by blazey (http://drupal.org/user/353861) for the monthly Meant4
Drupal Contrib Contest (http://meant4.com). 
Since 2011 module maintained by Digidog (http://drupal.org/user/1001934)

Installation
============

1) Download nicemessages from project page and copy it to sites/all/modules or sites/$sitename/modules.
2) Download jGrowl from http://stanlemon.net/projects/jgrowl.html, unpack and rename it to 'jgrowl' and move it into the directory sites/all/libraries/.
3) Enable module at admin/modules and (de)activate it individually at each user profile.
4) Detailed settings can be changed at admin/config (position, color, fading time, etc.)

Settings
========

The settings form is at admin/config/system/nicemessages. It is divided into
global (default state, position) and message-type-specyfic settings.

Permissions
===========

administer nicemessages
  Gives access to configuration form.

toggle nicemessages
  Users with this permission can turn on or off displaying nicemessages for
  them at their account edit page (user/$uid/edit).

Dependencies
============

jQuery jGrowl plugin (http://stanlemon.net/projects/jgrowl.html)
Should be renamed to jgrowl and placed into sites/all/libraries

Support
=======

http://drupal.org/project/issues/nicemessages
