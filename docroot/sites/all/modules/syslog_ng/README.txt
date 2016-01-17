Syslog-ng logging module for Drupal 7
=====================================
http://drupal.org/project/syslog_ng


This module is pretty simple by nature and is very similar to the
core syslog module (in fact the vast majority of its code is copied
directly from the core syslog module). The only difference is that it
was written to use syslog-ng instead of syslogd or rsyslog.

Why log to Syslog?
------------------
The most common reason is because it's much more efficient to log to
a text file than the MySQL database. Other reasons include ease of
exporting or centralization or archiving of your log files.

Why use Syslog-ng over regular Syslog?
--------------------------------------

Syslog-ng allows for far more customization than regular syslog.
It gives you the ability to log to different files for each Drupal
site on a server (handy for servers with lots of sites, or a server
with dev/stage/prod sites). You can also easily create log files
that belong to a certain user (very useful on a shared server with
web users who should not see one another's log contents). Syslog-ng
is extremely customizable and there are many possibilities.  Or you
may already be running syslog-ng on your server.

Installation and Configuration
------------------------------

Note: Before you start using this module you need to have syslog-ng
installed on your server.

Install and enable the module with the standard procedure: (see 
http://drupal.org/documentation/install/modules-themes/modules-7).

Then visit its configuration page under Configuration -> Development. 

The module defaults to disabled at first, so that you can get your
syslog-ng setup properly.

On the configuration page, set an Ident string (something unique on
your server to identify this particular site), and set an other
options you like. Then save the page, and the module will generate
some configuration for you to put in Syslog-ng's syslog-ng.conf file.

Once you've done that and re-started syslog-ng, you can check the
enable logging checkbox in the settings page, save again, and your
site will start logging.

Finally, once everything is working properly, you can disable the
Database Logging module in core, so that you are only logging to
syslog-ng.

