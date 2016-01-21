README.txt
==========
JSMin-PHP is a Drupal 7 module that uses the JSMin-PHP script to optimize your cached JavaScript files.

INSTALLATION & CONFIGURATION
============================
1) Install the module.
2) Go to /admin/settings/performance, enable the service. Enable js aggregation if it's not already enabled.
3) If you would like to compile locally using PHP-based JSMin-PHP script:
  * Download the JSMin-PHP script (https://raw.github.com/eriknyk/jsmin-php/master/jsmin.php) and place it
    in your libraries directory (e.g. sites/all/libraries/jsmin-php/jsmin.php).
  * Go to /admin/settings/performance and check local compiling status. The module will let you select the local compiling option only 
    if your environment passes the checks. Select "Compile locally" option.

LIMITATIONS & TROUBLESHOOTING
============================= 
JSMin-PHP module can operate in 1 mode as displayed under Preferred Processing Method in settings:
  1) Compile locally
  First mode requires jsmin.php file present in the correct libraries directory.

CHANGELOG
==========
7.x-2.1
==================
* Created the sub-module - it is not recommended for use, except by those who are unable to use java locally,
  or unwilling to use external services
