CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers

INTRODUCTION
------------
Fancy select is a jQuery plug-in developed by
Octopus Creative (http://octopuscreative.com/).

This module loads fancySelect (http://code.octopuscreative.com/fancyselect/)
jQuery plug-in, which converts simple HTML select DOM elements with a specified
CSS class into a stylish select box. You may configure DOM CSS from settings
form.

REQUIREMENTS
------------
This module requires the following modules:
 * Libraries API (https://www.drupal.org/project/libraries)
 * jQuery Update (https://www.drupal.org/project/jquery_update)
   This module work with jquery >= 1.7. After installing jQuery Update don't
   forget to upgrade default jquery version. 

INSTALLATION
-------------
* Download the latest fancySelect javascript library from 
  https://github.com/octopuscreative/FancySelect, extract the content and place
  the entire directory inside any of the following directories:
 
  -  sites/all/libraries
       Recommended. This is Drupal's standard third party library
       install location.
  -  profiles/{profilename}/libraries
       Recommended if this library is to be profile specific.
  -  Site-file-directory-path
       usually sites/<site_domain>/files. This location is not recommended.
       Only use this if you for some reason need to run different versions of
       the fancyselect library for different sites in your multi-site install.

  After install, you should have something like this:
    sites/all/libraries/fancyselect/...
   
  WARNING!
  --------
  Library location is determined by file scan. This scan is performed only once
  and the location is cached. If the library location is changed for any reason
  (such as after upgrading the js lib), visit admin/reports/status to force a
  re-scan and make sure the "fancySelect library" status
  entry shows "Installed".
        
* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

* Download the latest jQuery Update module
  from https://www.drupal.org/project/jquery_update and enable it.
 
CONFIGURATION
-------------
Go to admin/config/user-interface/fancyselect to configure.

  * Insert value for 'fancySelect DOM selector' same as jQuery selector
    you want to convert into fancy select and save the form. 

TROUBLESHOOTING
---------------
  * If the module not getting installed even after resolving all dependencies
    
    - Check sites/all/libraries/fancyselect is accessible and 
      have read access

    - You have installed Jquery Updates and set default jquery version higher
      than 1.7 and still it is showing jquery version dependency 
      then follow below steps:
      
        * admin/config/development/jquery_update and check jquery version
          for admin theme. If it is lower than 1.7 please set it to 
          'site default' or higher than 1.7
 
MAINTAINERS
-----------
Current maintainers:
  * Dileep K. Mishra (mishradileep) - https://www.drupal.org/u/mishradileep
