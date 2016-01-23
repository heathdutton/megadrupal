
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

Current Maintainer: Bastlynn <bastlynn@gmail.com>

The SCP API provides an api wrapper around scp features. Do not install unless another module requests it of you.

Future plans: Rules integration.

INSTALLATION
------------

1. Copy the scp_api directory to your sites/SITENAME/modules directory. 

2. Enable scp_api on the modules page.

3. Make sure your from and to servers are configured for ssh transfers. This
   is tricker than it sounds! You need to make sure you have SSH keys for
   the account names you will be using for your api - or set up identity files.
   Without an identity file or ssh key setup - you cannot automate the use of
   ssh as ssh will ask for a password. Password access is not a feature 
   currently implemented by this module.

FEATURES FOR DEVELOPERS
-----------------------

A new hook: hook_scp_api_alter($file_set) has been added on 6.x-1.1. This hook
allows other modules to alter the contents of a file_set before the set is
executed by scp.