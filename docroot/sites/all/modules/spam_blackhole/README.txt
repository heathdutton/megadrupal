Description
-----------
This is a simple module that replaces form actions for all anonymous users with a dummy URL which is then replaced using javascript before form submission. All spambot submissions (from bots that don't execute javascript) will go to a blackhole. Will require javascript to work for normal users. 

Configuration
-------------
1. Visit 'administer >> site configuration >> spam blackhole'
2. You can specify the specific forms which should be handled by the module. 

Installation
------------
1. Extract the tar.gz into your 'modules' or directory.
2. Enable the module at 'administer >> site building >> modules'.
3. Configure options in admin/settings/spam-blackhole

Uninstallation
--------------
1. Disable the module.
2. Uninstall the module

Credits
-------
Written by Zyxware, http://www.zyxware.com/
