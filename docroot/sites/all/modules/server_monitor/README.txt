Server Monitor

Introduction
==========================
The Server Monitor module will monitor and display the status of a
number of servers which will be polled a ping or by retrieving a
URL (drupal_http_request or cURL). The status checks can be configured to
run during cron.

Description
==========================

When cron runs, Server Monitor will iterate through the list of configured
servers and check their status (up or down) using the method
specified (ping, curl or drupal_http_request). The current status is recorded
and is displayed in a block on your website as well as in a JSON feed for use
with other services you may have.

Requirements
==========================
 - cURL, if you wish to use it as a monitor method.

Additional Notes
==========================
If you have the Varnish module installed, Server Monitor will automatically
ban/purge the JSON feed when cron runs.  For this reason, if you are running
Varnish server(s), it is also best to check the 'Use JSON generated block?'
option in the administration menu, as it will ensure that your block displays
as up-to-date information as available.

Drush support. You can also use drush to run the server monitor via either
drush server-monitor, or the alias drush smon.

Future Plans (Drupal 7 version only)
==========================
- Prior to first stable release, I would like to add a logging functionality to
allow for recording of server statuses.
- Views integration, if there is enough interest/use of the module to warrant
the work.