INTRODUCTION
------------
Lithium SSO is a single sign-on project that allows two-way authentication
between Drupal and Lithium community user systems.

The login process, account registration and credentials recovery are all
performed by Drupal. After login, Drupal will send the user back to the page
they came from on the Lithium forum.

When the user is logged into Drupal they are automatically logged into the
Lithium site that is set in the configuration page at:
admin/config/services/lithium-sso.


REQUIREMENTS
------------
This module requires the following modules:
 * Libraries (https://www.drupal.org/project/libraries).


INSTALLATION
------------
 * Lithium SSO requires the Libraries module, both versions 1.0 and 2.0 will
   work. Before installing this module you must obtain the LithiumSSO libraries
   from your Lithium provider, unzip it and place the file lithium_sso.php in a
   sub-directory named lithium_sso in the libraries directory at:
   /sites/all/libraries.

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


   CONFIGURATION
   -------------
 * Customize the Lithium SSO settings in Administration »
   Configuration » Content authoring » Lithium SSO.


- Enter the client ID, domain and SSO key.


TROUBLESHOOTING
---------------
 * If you ever have problems, make sure to go through these steps:


   - Go to admin/reports/status (i.e. the Status Report). Ensure that the status
     of the Hierarchical Select by Role module is ok.


- Ensure that the page isn't being served from your browser's cache.
     Use CTRL+R in Windows/Linux browsers, CMD+R in Mac OS X browsers to enforce
     the browser to reload everything, preventing it from using its cache.


- Go to admin/config/development/performance and clear all caches.
