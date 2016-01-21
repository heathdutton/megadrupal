CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------
The Carerix API Client module exposes the Carerix PHP API Client to other Drupal
modules. The module by itself does not provide any integration out-of-the-box.
Carerix is online software for staffing and recruiting. Read more about Carerix
on http://www.carerix.com/.
 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/carerix_api_client
 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/carerix_api_client

REQUIREMENTS
------------
This module requires the following modules:
 * Libraries (https://drupal.org/project/libraries)
This module requires the following 3th party libraries:
 * Carerix PHP API Client (http://development.wiki.carerix.com/cxwiki/doku.php?id=cxrest_api_client#getting_installing)
You should have an approved Carerix account (http://www.carerix.com/) to be able
to use this module.

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * Create sites/all/libraries (if it does not already exist)
 * Put the Carerix PHP API Client (http://pear.carerix.com/)
   in the libraries folder so that Loader.php is in
   sites/all/libraries/carerix_api_client/Carerix_Api_Rest-@version/Carerix/Api/Rest/Loader.php
   where @version is the version of the client e.g. 1.2.9.

CONFIGURATION
-------------
After installation the module should be configured to be able to connect to
Carerix.
 * Go to admin/config/carerix
 * Fill in the information as supplied by Carerix
 * Click the Save button

MAINTAINERS
-----------
Current maintainers:
 * Rico van de Vin (ricovandevin) - https://www.drupal.org/u/ricovandevin
This project has been sponsored by:
 * Finlet
   Finlet offers training and consultancy for Drupal.
   Visit http://finlet.eu for more information.
