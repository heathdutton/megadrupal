Module: Resolve IP
Author: Yannis Karampelas - yannisc - netstudio.gr
Date: 19/8/2012

Description
===========

Resolve IP is a very simple module that adds the hostname of the IP that generated the watchdog entry in the recent log messages report.

This is helpful to understand and easier debug log entries.

For example, knowing that the IP 66.249.66.212 resolves to crawl-66-249-66-212.googlebot.com, makes it easier to understand the log message.

You see the resolved hostname besides the IP without having to copy-paste the IP in a web resolver.

This module is created and sponsored by netstudio.gr, a drupal development company in Athens, Greece.


Installation
============

* Install like any other module throught the Drupal 7 module installer.

* Enable the module from the Modules > Resolve IP.


Setup
=====

* No Setup needed.

* Navigate to admin/reports/dblog and click on any entry. You should see the resolved IP address besides the IP.
