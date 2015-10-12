helpful.module
--------------
Helpful.module is a helpful module that lets you display advanced_help 
topics to users without giving them full 'access advanced help' permissions.

It also provides additional functionality to make advanced help more user 
friendly for end users.


REQUIREMENTS
------------
Helpful module requires:

- advanced_help.module: http://drupal.org/project/advanced_help
- The patch to advanced help located at http://drupal.org/node/1471092  
  (Direct download: http://drupal.org/files/advanced_help-ini_data_to_alter.patch)

To patch advanced help, simply download the patch file to advanced_help's folder, and 
run the terminal command:

$ cd /path/to/sites/all/modules/advanced_help
$ wget http://drupal.org/files/advanced_help-ini_data_to_alter.patch
$ patch -p0 < advanced_help-ini_data_to_alter.patch

