# Site policy module

This module provides some optional best practice configuration for your Drupal
site to ensure all the correct policies are applied.

## User 1

On Cron run, the user 1 account is checked
The user 1 account can be disabled if its not being used
The user 1 password can be scrambled

The user 1 account has all privileges. Losing control of the user 1 account
can be a major security breach. It is better to have crafted user accounts
on the site for audit purposes and security control.

# Configuration

By strict design and principle, this module doesn't have any UI exposed settings
or configuration forms. The reason behind this philosophy is that - as a pure -
utility module only site administrators should be able to change anything and if
they do, things should be traceable in settings.php.

The following configuration options are available:

|      $conf setting                | Default |               Description                 |
|:----------------------------------|:--------|:------------------------------------------|
| site_policy_user_1_active_time  | 0       | Number of seconds the user 1 account can  |
|                                 |         | be left enabled. If the last access time  |
|                                 |         | is greater than this it is disabled       |
|                                 |         | Set to 0 to disable this feature          |
|                                 |         | Set to 86400 for 1 day                    |
|                                 |         |                                           |
| site_policy_user_1_pw_scramble  | 0       | Number of seconds before the user 1       |
|                                 |         | password gets scrambled                   |
|                                 |         | If using drush, you can always login      |
|                                 |         | with drush uli                            |
|                                 |         | Set to 0 to disable this feature          |
|                                 |         | Set to 86400 for 1 day                    |
|                                 |         |                                           |
