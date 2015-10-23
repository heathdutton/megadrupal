 CONTENTS OF THIS FILE
 =====================

 * Introduction
 * Features
 * Requirements
 * Installation

 INTRODUCTION
 ============

 This module provides an integration to Munin (http://munin-monitoring.org/),
 allowing to expose sensor data as munin graphs.

 REQUIREMENTS
 ============

 * PHP min version 5.3
 * Drupal xautoload module to utilise namespaces
 * Running Munin server

 INSTALLATION
 ============

 * Note that some of the instructions below are Debian based.

 * To retrieve sensor results Drush commands are being invoked. In case drush
   command is called by munin or nagios users it may result in problems due to
   permission issues. The solution is to alow th munin and nagios users to sudo
   as the www user. This can be done using visudo tool:

   %munin ALL =(ALL) NOPASSWD: /path/to/drush

   Then remember to prepend to each drush command you define during the
   configuration with "sudo -u your_www_user".

 On the monitored system the munin-node service needs to be installed and
 configured. It is responsible for collecting and passing the monitoring data
 to the Munin reporting tool.

 Simple steps for Debian (prerequisite - Munin system installed)

 * Install needed packages:
   # apt-get install munin-node

 * Update /etc/munin/munin-node.conf to allow your Munin server IP address:
   allow ^212\.25\.2\.101$

 * Install monitoring_munin module.

 * The Monitoring Munin Drupal module provides an example script to trigger
   the sensors at: admin/config/system/monitoring/munin/config. Copy the script
   and create an executable file at /etc/munin/plugins. Note that it is
   recommended to invoke drush command as the www user.

 * Restart munin-node service (Debian: # service munin-node restart).

 * Point your Munin server to the client system by a new host tree in the munin
   /etc/munin/munin.conf file.
