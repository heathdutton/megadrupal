Status Watchdog Logger
======================

Status Watchdog logs system status messages (admin/reports/status) to the
watchdog. This is especially useful when combined with syslog and external
monitoring solutions such as Splunk.

Dependencies
------------

* [xautoload](https://www.drupal.org/project/xautoload)

Installation
------------

Simply turn on the module and watchdog messages will be logged during cron
runs. By default, messages are logged once per hour and after a cache clear.
This ensures that any changes in status after a deployment are immediately
logged.

Configuration
-------------

Set ```$conf['status_watchdog_interval']``` in settings.php to the number of
seconds between logging runs. By default, the interval is set to 3600 seconds.
