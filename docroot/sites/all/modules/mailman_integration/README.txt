Mailman Integration - Introduction
-----------------------------------

This module allows Drupal Administrator to create, subscribe and unsubscribe
a Mailing List from Drupal.


Requirements
------------
 * Access to Mailman
 * You need to have the Mailman URL and Mailman site password


Installation
------------
 * Copy the mailman_integration folder to your module directory.
 * At Administer -> Modules (admin/modules) enable the module.


Configuration
--------------
 * Configure the module settings page link
   - Administer -> Configuration -> User Interface -> Mailman Integration. 
   - (admin/config/user-interface/mailman-integration).
 * Mailman Integration module page link
   - Administer -> Configuration -> System -> Mailman Mailing Lists.
   - (admin/config/system/mailman_integration/list).


Features
--------
The primary features include:

 * Administrator to manually create a new Mailing List from Drupal.
 * Administrator can subscribe/unsubscribe a Mailing List for users 
from Mailman Integration module admin page.
 * Administrator can create a role based Mailing List from Drupal.
 * When a Member is assigned this new Role, the Member should automatically
become part of the corresponding Mailing List.
 * When a Member is removed from a particular Role, the Member
should be removed from the corresponding Mailing List as well.
 * When a Role is removed from Drupal, the corresponding Mailing List,
if any, should be removed from Mailman.
 * User can subscribe/unsubscribe a mailing list from my profile 
if admin check the User can Subscribe option in the mailing lists.
 * When the Cron runs all the mailman lists migrated into Drupal.


Maintainers
-----------
Selva Gajendran.S
