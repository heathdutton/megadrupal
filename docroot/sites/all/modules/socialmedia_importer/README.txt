CONTENTS OF THIS FILE
---------------------

  * Summary
  * Requirements
  * Installation
  * App Authentication
  * Developers


SUMMARY
-------

Social Media Importer is a developer only module that integrate social media´s 
API (e.g. Twitter, Facebook) in Drupal. This module allows 
you to authenticate with this API´s and use this authentication to Import Data
in JSON.


REQUIREMENTS
------------
1. OAuth Module - This module depends on OAuth Module.
   You can download this module from http://drupal.org/project/oauth


INSTALLATION
------------

1. Copy this module directory to your sites/all/modules or
   sites/SITENAME/modules directory.

2. Enable the module

3. To manage Social Media Applications you need to enable the UI Module.

CONFIGURATION
-------------
 1. On the People Permissions administration page ("Administer >> People
    >> Permissions") you need to assign:

    - The "administer Social Media Applications" permission to the roles 
      that are allowed to access the social media importer admin pages 
      to manage the Applications.
      
APP AUTHENTICATION
------------------
1. Use the app ID and app secret from your Social Media Application's page 
  (i.e. https://code.google.com/apis/console/) to fill
   out the Application form at
   http://example.com/admin/config/services/socialmedia_importer/application/add
   Select the applicaion provider eg googleplus, facebook etc.
   
3. On save of the form it will ask for access,
   click allow access so that the Application gets authenticated.
   
4. This Application can be use to import Data.

IMPORT DATA TO DRUPAL
---------------------
 Create a module and call the function 
 socialmedia_importer_get_application_data($id).$id is the unique Application
 id in the database. This is not the app id from the Social Media Application
 page. The function will return the Application data in JSON.

DEVELOPERS
----------
Full API documentation is available in socialmedia_importer.api.php.
