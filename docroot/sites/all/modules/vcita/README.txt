INTRODUCTION
------------
This module implements a way to use the vCita online scheduling and
appointment booking widget from within Drupal. vCita LiveSite(TM) creates a
central, cloud-based facility for customers to connect via messaging or
requests for appointments. It integrates with calendaring applications for
free/busy time negotiations and appointment confirmations. With its contact
management and email campaign facilities, it can act as a simple CRM solution.

REQUIREMENTS
------------
No other modules are required.

Javascript needs to be enabled in the browser for the widget to work.

A valid vCita account is required for the module to work. Free and premium
accounts are available at http://www.vcita.com/?invite=rf-bfb314eb8f5a48d4.
The configuration for the module allows for an account to be created or an
existing account to be activated via login.

RECOMMENDED MODULES
-------------------
The vCita module has been tested with Drupal Commerce and Ubercart. It works
best in an e-commerce setting where customers are looking to answer questions
about the products offered.

INSTALLATION
------------
* Place the module inside your directory that holds contributed
 modules (usually {drupal_root}/sites/all/modules).

* Go to admin/modules and turn on "vCita".

CONFIGURATION
-------------
vCita comes with a set of permissions to allow select roles to administer
the settings. It also needs to be linked to an existing account or to a
new account that can be created from within the configuration page.

* Edit the permissions to allow only privileged accounts to edit the settings:
 go to admin/people/permissions and look for the "vCita" section.

* Configure vCita by linking it to your account or creating a new one. You
 can also filter the display of the vCita widget by URL, node type or role.
 Go to admin/config/services/vcita to link your account or edit the settings.

ISSUES AND SUPPORT
------------------
Support is handled through drupal.org. To open a bug report or support request,
go to https://www.drupal.org/project/issues/2335843?status=All&categories=All

MAINTAINER
-----------
This module is maintained by Klaus Sonnenleiter at PrintedArt. To reach him
through drupal.org (https://www.drupal.org/user/635432) or directly via email:
klaus@printedart.com.

CREDITS
-------
Many thanks to Shelly Bar-Nahor at vCita for helping to figure out the right
way to connect and with setting everything up.
