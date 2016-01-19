CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Configuration
* Permissions
* Maintainers
* Special Thanks


Introduction
------------
The Watchdog Notifier module allows for the configuration of automatic email
notifications when certain log messages of the specified severities appear in
the watchdog log.  The scanning of the log and subsequent notifications can be
configured to send immediately upon occurance, it can be managed with a
Unix-style cron configuration, or left to be done during regular Drupal cron
activity.

Upon installation, add email addresses and the desired watchdog types and
severities to be monitored to the watchlist.  Then, select the preferred
scheduling option: immediately send messages or manage with the cron option
(via Drupal or using the Job Scheduler module).  If the cron option is selected,
the next time cron is run, the watchdog log will be scanned for any new matching
type/severity messages, and that message will then be emailed to the respective
email addresses.


Requirements
------------
The Job Scheduler module is required.
https://www.drupal.org/project/job_scheduler

If a cron option is enabled, then the Dblog module needs to be enabled.

Installation
------------
Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


Configuration
-------------
There are two main configuration options accessible under
'Configuration > System > Watchdog Notifier' or by going to
/admin/config/system/watchdog_notifier:

* The recipient email address, watchdog type, and watchdog severity for each
    log message that needs to be monitored.
* Managing the scheduling of the watchdog notifications: either immediately
    sending the notification or scheduling notifications via Drupal's cron
    utility/the Unix-style crontab available from the Job Scheduler.


Permissions
-----------
If someone other than user 1 needs to configure the watchlist or the cron
settings, set the "Administer the watchlists and frequency of scans for the
watchdog notifier" permission for that user's role.


MAINTAINERS
-----------
Current maintainers:
* Adam Fuller (dasfuller) - https://drupal.org/user/2731951

SPECIAL THANKS
--------------
DamienMcKenna for providing bugfixes and patches that enabled the immediate-send
feature for notifications.
