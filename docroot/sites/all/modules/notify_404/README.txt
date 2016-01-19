/**
 * @file
 * README file for Notify 404.
 */

Notify 404

CONTENTS
--------

1.  Introduction
1.1 Concepts
2.  Installation
2.1  Requirements
3.  Configuration
3.1 Checking Permissions
4.  Using the module
5.  Troubleshooting
6  Database schema

----
1.  Introduction

Notify 404

----
1.1  Concepts

Notify 404 provides the ability for notification emails to be sent to a user
when a specific volume / frequency or 404's (page not found) errors have
occurred on the website. This module allows for configurable email settings
and volume / frequency settings.

----
2.  Installation

Install the module and enable it according to Drupal standards.

The module's configuration pages reside at:
- admin/config/system/notify-404

The module depends on token module.

----
2.1  Requirements

Notify 404 requires:
- Token

----
3.  Configuration

Notify 404's configuration section is located at:

- Admin > Configuration > System

This section allows the admin to configure email, volume and frequency settings.

----
3.1  Checking permissions

In order to administer the module you must have the appropriate permissions.
Navigate to admin/people/permissions and select Administer 404 Notifications
for the appropriate role.

----
4.  Using the module

Once the module is installed and the correct permission are given, a user
can give set the "To" address, email subject / message and the frequency /
volume. The frequency is when the module will check for 404 errors, and the
volume is how many of the same 404 error must be logged before the email is
sent. The module runs via hook_cron() and is dependant on a cron job setup.
If the cron only runs every hour but, your frequency is set to every 10
minutes, the check will only occur every hour.

----
5.  Troubleshooting

- If no email is set, the system will use the default site email. Check to
make sure one of these is set.
- If no emails are being received, ensure that a cron job is set at the
server level.

----
6.0  Database schema

Notify 404 stores all its settings in the core variable table.
