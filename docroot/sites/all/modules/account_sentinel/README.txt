CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Drush commands


INTRODUCTION
------------

Account Sentinel perceives changes made to a configurable set of monitored
roles' accounts, even those that are results of direct database modification
(e.g. SQL injection). A set of e-mail addresses can be configured to be notified
when such a change is detected, also a log of changes can be viewed in Drupal
and via Drush for manual review.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/banviktor/2570357

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2570357


REQUIREMENTS
------------

No special requirements.


RECOMMENDED MODULES
-------------------

 * Password Strength (https://www.drupal.org/project/password_strength):
   When enabled, password change events will also store the strength of the
   new password.


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

 * Configure user permissions in Administration » People » Permissions:

   - Access Account Sentinel logs
     Users with this permission are able to access the logs of Account
     Sentinel on Administration » Reports » Account Sentinel log.

   - Change Account Sentinel's configuration
     Users with this permission are able to change the configuration of Account
     Sentinel on Administration » Configuration » System » Account Sentinel
     settings.

 * Configure the module in Administration » Configuration » System » Account
   Sentinel settings:

   - Monitored roles
     Choose which roles' users should be monitored. Typically any administrative
     roles should be checked.

   - Recipient(s) of notification e-mails
     You may provide e-mail addresses to send notifications to. The format
     should be (if not empty):
      * user@example.com
      * user@example.com, anotheruser@example.com
      * User <user@example.com>
      * User <user@example.com>, Another User <anotheruser@example.com>

   - Automatic DB check method
     Account Sentinel periodically compares Drupal's data about users with its
     own snapshots of monitored users. You can configure with this option what
     the trigger for these audits should be.

 * [Optional] Set last audit warning threshold
     Account Sentinel will show a warning if it's been a while since the last
     database audit. By default this warning is shown after 3 days. You can
     change this value using Drush:

     drush vset --yes account_sentinel_audit_warn_after NEW_VALUE

     Where NEW_VALUE is the threshold in seconds.


DRUSH COMMANDS
--------------

 * account-sentinel-audit (as-audit)
   Runs the Account Sentinel database audit.

 * account-sentinel-show (as-show)
   Shows an event's details or a list of the most recent events.
   Please consult "drush help as-show" for more information.
