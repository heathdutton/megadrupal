CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

The BotScout module will check new users by there ip, name, email, or any 
combination of the three to see if they are a known bot. This module checks the 
information against a database of known bots. If the user is found in the 
database they are not allowed to create an account. If the contact form is also 
being checked they will not be allowed to submit the contact form if their data 
matches information found in the database.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/projects/botscout

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/botscout


REQUIREMENTS
------------
This module requires the following modules:

 No special requirements.


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

 * Module configuration in Administration » Config » People » Botscout:


   - Enable IP filtering


     Enables/disables checking new users by their ip address.


   - Enable username filtering


     Enables/disables checking new users by username.


   - Enable email filtering


     Enables/disables checking new users by email address.


   - Alert by email when bot is blocked


     Sends an email to the email address you provide when a bot is blocked.


   - Email address


     This is the email address to alert you at when alert by email is on.


   - API key


     Api keys are used to access the botscout database without one you are 
     limited to 20 bot checks/day. get a free api key at www.botscout.com


MAINTAINERS
-----------

Current maintainers:
 * Jonathan Langlois (acetolyne) - https://www.drupal.org/user/1036768
