CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration


INTRODUCTION
------------
This module implements an interface for processing Webform-created forms with
the the vetting service E-Hawk. E-Hawk is vaguely like an anti-spam service, but
more sophisticated; rather than being geared toward spam, it's designed to
evaluate provided personal information to assess the likelihood of fraud, rather
than evaluating the content of a message.

 * For a full description of the module, visit the project page:
   https://drupal.org/project/ehawk [TK]

 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/ehawk [TK]


REQUIREMENTS
------------
This module requires the following modules:
 * Webform (https://drupal.org/project/webform)

In addition, it requires your version of PHP have been compiled with libcurl
support


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * Configure user permissions in Administration » People » Permissions:

   - Configure E-Hawk Settings

     Users in roles with the "Configure E-Hawk Settings" permission will have
     the ability to link webforms to the e-hawk service as well as view and
     change the E-Hawk API key.

 * Once installed and activated, navigate to Admin > Configuration > Web
 Services > E-Hawk. From there you can input the API key provided to you by
 E-Hawk to initiate the connection.

 * After you've setup your API key, you can designate any and all webforms
 you've created to have submissions processed by E-Hawk (in addition to whatever
 other processing you're doing with Drupal). Values returned by E-Hawk are then
 stored as a part of each webform submission.
