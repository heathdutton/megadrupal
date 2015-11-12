Statcounter 2.x for Drupal 7.x
==============================
Module: Statcounter - Web analytics


Description
===========
Adds the Statcounter tracking system to your website.

The 2.x branch is a rewrite from scratch, and departs from the method used by
the 1.x branch, by not using the block system to insert the statcounter code
into the drupal pages of your site. It is largely based on code from the
Google Analytics module <http://drupal.org/project/piwik>
and Piwik Web analytics module <http://drupal.org/project/piwik>
by Alexander Hass <http://drupal.org/user/85918>

Requirements
============

* Statcounter account


Installation
============
* Copy the 'statcounter' module directory in to your Drupal
sites/all/modules directory as usual.


Usage
=====
In the settings page enter your statcounter Project ID, and 
Statcounter Security Code, if applicable.

You will also need to define what user roles should be tracked.
Simply tick the roles you would like to monitor.

All pages will now have the required JavaScript added to the
HTML footer can confirm this by viewing the page source from
your browser.


Advanced Settings
=================
To speed up page loading you may also cache the statcounter.js
file locally. You need to make sure the site file system is in public
download mode.


Maintainers
===========
boombatower <http://drupal.org/user/214218>
kdebaas <http://drupal.org/user/75900>
drupalevangelist <http://drupal.org/user/428243>
