 CONTENTS OF THIS FILE
 =====================

 * Introduction
 * Features
 * Requirements
 * Installation

 INTRODUCTION
 ============

 This module provides an integration to Icinga (http://www.icinga.org/)
 reporting tools. It can also be used also as connector Nagios as it uses same
 communication protocol.

 REQUIREMENTS
 ============

 * PHP min version 5.3
 * Drupal xautoload module to utilise namespaces
 * Running Icinga

 INSTALLATION
 ============

 * Note that some of the instructions below are Debian based.

 * To retrieve sensor results Drush commands are being invoked. In case drush
   command is called by munin or nagios users it may result in problems due to
   permission issues. The solution is to alow th munin and nagios users to sudo
   as the www user. This can be done using visudo tool:

   %nagios ALL =(ALL) NOPASSWD: /path/to/drush

   Then remember to prepend to each drush command you define during the
   configuration with "sudo -u your_www_user".

 Active checks via NRPE
 ------

 In order to connect to a remote site and collect data you need to install the
 NRPE plugin that mediates the communication. For more info visit:
 http://docs.icinga.org/latest/en/nrpe.html

 Simple steps for Debian (prerequisite - Icinga system installed):

 -- Client (where the site to be monitored is running) --

 * Install needed packages:
   # apt-get install nagios-nrpe-server nagios-plugins

 * Install monitoring* modules. First install the monitoring module so that
   during monitoring_icinga installation the PHP classes provided can are found.

 * There is a local config file provided at /etc/nagios/nrpe_local.cfg.
   Visit the admin/config/system/monitoring/icinga page, section "NRPE
   configuration" which provides an example of the needed configuration. Note
   that this configuration contains options and a command. In case you are
   setting up monitoring of multiple Drupal sites running on the same system you
   just need to append a separate command for every other site in the same
   nrpe_local.cfg file.

 * Make sure nagios user has the sufficient permissions to invoke the drush cmd.
   The cleaner solution though is to execute the cmd with sudo as the www user.

 * Restart the nrpe daemon (Debian: # service nagios-nrpe-server restart).

 -- Server (where the Icinga instance is running) --

 * (first time only) Install the nagios nrpe plugin which will care about the
   communication with remote nrpe servers (the "server" here is little
   misleading and it refers to a utility running at the monitored system which
   will talk to the central Icinga server):
   # apt-get --no-install-recommends install nagios-nrpe-plugin

 * Check connection to your client (this will make sure Icinga can connect to
   the remote monitored system):
   # /usr/lib/nagios/plugins/check_nrpe -H 192.168.2.11 (replace with your IP)
   Above command should display NRPE v...
   # /usr/lib/nagios/plugins/check_nrpe -H 192.168.2.11 -c check_users
   Above command should display user info.

 * (first time only) In the /etc/icinga/commands.cfg file create the Icinga
   command to issue the remote NRPE call. See the "Command definition" section
   at the admin/config/system/monitoring/icinga page and copy-paste provided
   command.

 * Create the Icinga objects. At the admin/config/system/monitoring/icinga page
   see the "Host and services configuration" section. The configuration provided
   there defines host, service groups and services objects. Creating a cfg file
   in the /etc/icinga/objects and copying the provided configuration should be
   enough to monitor enabled sensors.

 * Restart icinga service. After next NRPE pull the host with services should
   appear in the Icinga web tool. (Debian: # service icinga restart).

 -- Troubleshooting --

 In case of "NRPE: Unable to read output" you most likely have permission issue.

 Passive checks using "nsca"
 ------

 From the performance point of view this is recommended. The problem with active
 checks is that each sensor is called separately. That involves invoking drush
 command in full bootstrap mode. That can have severe affect on performance.

 The idea of passive checks is that results are pushed from the monitored system
 to the central Icinga server. This has one functional implication, when if the
 monitored system is down, the status change will not get reflected in the
 central Icinga server. For such case the "freshness" checking should be
 involved. For more info see
 http://docs.icinga.org/latest/en/distributed.html#problemspassive

 On both, the monitored system and Icinga server install nsca:
 # apt-get install nsca

 -- Server (where the Icinga instance is running) --

 * (first time only) Edit the nsca config nsca.cfg (Debian location
   /etc/nsca.cfg) and update following settings:

   - Update command file path as it differs from the Nagios location.
     command_file=/var/lib/icinga/rw/icinga.cmd

   - Set decryption password and decryption method
     password=somesecretdescryptionpassowrd
     decryption_method=1

 * (first time only) Edit /etc/icinta/icinga.cfg and set
   check_external_commands=1

 * On the monitored system enable the monitoring_icinga module and see the
   "Host and services configuration" section at
   /admin/config/system/monitoring/icinga/passive. This is the configuration of
   the Icinga objects: server, service groups and services. Copy this
   configuration into a separate cfg file in /etc/icnga/objects

 * Restart icinga service (Debian: # service icinga restart).

 -- Client (where the site to be monitored is running) --

 * (first time only) Edit the /etc/send_nsca.cfg file and set the encryption
   password and method to comply with settings at Icinga server:
   password=somesecretdescryptionpassowrd
   encryption_method=1

 * Create a shell script which will using the nsca submit the sensors results
   check to the Icinga server. See the section "Setting up send_nsca command" at
   the /admin/config/system/monitoring/icinga/passive page. This is a working
   example how this script may look like. Copy the script and paste it into an
   executable file at your monitored system. Then execute the script manually.
   You should see output saying that packages are being successfully submitted.

 * Create a crontab entry which will trigger this script automatically:
   */5 * * * * /path/to/script/submit_check_result > /dev/null 2>&1

 -- Troubleshooting --

 You might have troubles connecting/submitting. Again permissions problem, but
 regular chmod/chown did not work for me. This is what did:

 # service icinga stop
 # dpkg-statoverride --update --add nagios www-data 2710 /var/lib/icinga/rw/
 # dpkg-statoverride --update --add nagios nagios 751 /var/lib/icinga/
 # service icinga start

 Eventually, in the icinga.cfg set the "log_passive_checks" to 1. Then you can
 see the attempts to submit passive checks in the /var/log/icinga/icinga.log.

 Passive check with freshness checking
 .....................................

 This option is a combination passive and active approaches. The results are
 expected to be submitted by the monitored system via nsca. However the Icinga
 server conducts freshness checks on individual services (sensors) and in case
 the check does not pass an active check is performed.

 In order to have this setup you need to have functional both - the active and
 the passive checks.

 There is only one difference - that is the services configuration. For this
 purpose use the configuration provided at the
 admin/config/system/monitoring/icinga/passive-with-freshness page. Notice that
 the only difference from the passive is the check_freshness flag turned to 1.
