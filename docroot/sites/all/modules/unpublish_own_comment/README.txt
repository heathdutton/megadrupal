CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers
 
INTRODUCTION
------------
This module adds a "unpublish" button on comments based on permissions.
You'll se the button if:
 * You are the site administrator
 * You can admin comments
 * You've enabled the permission (defined by this module) unpublish own comment.

This module is very useful if you need some role must be able to unpublish own 
comments but not edit or delete then.

REQUIREMENTS
------------
This module requires the following modules:
 * Comment (core module)

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
   
CONFIGURATION
-------------
The link appear only if user has the permission:
 * Site super-admin role
 * Admin comments
 * Use unpublish own comment (defined by this module)
 
MAINTAINERS
-----------
Current maintainers:
 * Alvaro J. Hurtado (alvar0hurtad0) - https://drupal.org/u/alvar0hurtad0

Special thanks to:
 * Ruben Egiguren (keopx) - https://drupal.org/u/keopx
