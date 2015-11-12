This module enables a Drupal website to send statistics to a remote Zabbix server. For more information on Zabbix, see http://www.zabbix.org

INSTALLATION
1. Enable module

2. Setup Zabbix server Zabbix Trapper entries
The following keys are available for Zabbix server configuration

drupal.lastcronrun
  Numeric (unsigned)
  Sends to zabbix the time that cron was last run as a unix timestamp.

drupal.moduleupdatestatus
  Text
  Sends to zabbix comma separated list of modules that are not up to date with the current Drupal.org version.

drupal.lastadminlogin
  Text
  Sends to zabbix the time that the admin user last logged in as a unix timestamp.

drupal.watchdogemergencies
  Numeric (unsigned)
  Sends to zabbix the number of watchdog emergencies since the last check.

drupal.watchdogalerts
  Numeric (unsigned)
  Sends to zabbix the number of watchdog alerts since the last check.

drupal.watchdogcritical
  Numeric (unsigned)
  Sends to zabbix the number of watchdog critical entries since the last check.

drupal.watchdogerrors
  Numeric (unsigned)
  Sends to zabbix the number of watchdog errors since the last check.

drupal.watchdogwarnings
  Numeric (unsigned)
  Sends to zabbix the number of watchdog warnings since the last check.

drupal.watchdognotices
  Numeric (unsigned)
  Sends to zabbix the number of watchdog notices since the last check.

drupal.watchdoginfo
  Numeric (unsigned)
  Sends to zabbix the number of watchdog info entries since the last check.

drupal.watchdogdebug
  Numeric (unsigned)
  Sends to zabbix the number of watchdog debug entries since the last check.

drupal.watchdogphp
  Numeric (unsigned)
  Sends to zabbix the number of php watchdog entries since the last check.

drupal.allusers
  Numeric (unsigned)
  Sends to zabbix the total number of user accounts for the site.

drupal.activeusers
  Numeric (unsigned)
  Sends to zabbix the number of active user accounts for the site.

drupal.enabledmodules
  Text
  Sends to zabbix comma separated list of modules that are enabled on the site.

drupal.enabledthemes
  Text
  Sends to zabbix a comma separated list of themes that are enabled on the site.

drupal.allnodes
  Numeric (unsigned)
  Sends to zabbix the total number of nodes there are on the site.

drupal.publishednodes
  Numeric (unsigned)
  Sends to zabbix the number of published nodes are on the site.

drupal.sessionsanon
  Numeric (unsigned)
  Sends to zabbix the number of current sessions for anonymous users.  Checks for sessions started in the last 15 minutes.

4. Setup your own triggers and alerts on your Zabbix server based on your own requirements.

5. Refer to the zabbix_items.xml in the example_zabbix_server_configuration directory. This is an export of example_items from Zabbix Server 1.8.2.