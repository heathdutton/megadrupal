
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Credits


INTRODUCTION
------------

Encrypt Password module adds the ability to use the Encrypt module to encrypt
user passwords.

Currently the installation method is more convoluted than it should be, requiring
a patch to the Encrypt module, however this will be resolved very soon with a new
Encrypt module release.

Refer to Encrypt issue: https://www.drupal.org/node/2231779


INSTALLATION
------------

1. Download and install the latest dev version of the Encrypt module:
   https://www.drupal.org/node/1166672 (currently version 7.x-2.x-dev)

2. Download the latest clean patch from the above referenced issue:
   https://www.drupal.org/node/2231779

3. Apply the patch to the Encrypt module directory.

4. Immediately run the site update.php script to update tables in your database.

5. Download the latest version of this module, Encrypt Password, and place it
   in the correct module directory for your site, usually:
   sites/all/modules/contrib/encrypt_password. Make note of the location used.

6. Edit your sites/default/settings.php file (or your site's settings.php file).
   Add the following line to the end of the settings file altering the path to suit:
   $conf['password_inc'] = 'sites/all/modules/contrib/encrypt_password/password.inc';

7. Enable the module at Administer >> Site building >> Modules.


CREDITS
-------
* Rick Hawkins (rlhawk)
* Patrick Teglia (CrashTest_ or pteglia)
* Sponsored and made possible by Townsend Security - http://townsendsecurity.com