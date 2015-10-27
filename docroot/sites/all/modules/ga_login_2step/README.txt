CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers

INTRODUCTION
------------
The GA Login 2step module shows Code input in a second step of the form
submission and works well with the permission "login without code" to bypass the
ga login 2 step for particular users.

REQUIREMENTS
------------
This module requires the following modules:
 * GA login (https://drupal.org/project/ga_login)

RECOMMENDED MODULES
-------------------
 * Markdown filter (https://www.drupal.org/project/markdown):
   When enabled, display of the project's README.txt help will be rendered
   with markdown.

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 * You may want to disable other authentication module, since its output clashes
   with other authentication mechanism.

CONFIGURATION
-------------
 * Configure GA Login 2 step in Administration » People » GA Login

 * Customize the GA Login 2 step settings in menu settings in Administration »
   People » GA Login

TROUBLESHOOTING
---------------
 * If the ga login 2 step is not working , check the following:

   - Have you enabled the "Two Step login form" from Administration »
     People » GA Login?

FAQ
---
Q: I enabled "Two Step login form", but ga login 2 step is behaving differently.
   Is this normal?

A: Yes, there might be some other authenticated module which is effecting its
   intended behavior.

MAINTAINERS
-----------
Current maintainers:
 * Naveen Valecha (naveenvalecha) - https://drupal.org/u/naveenvalecha
