tngintegrate.module readme.txt
by Arturo Ramos
arturo.ramos2@gmail.com
March 25, 2008

updated by Alex B
alexxikon@gmail.com
March 2, 2012


This is Version 1.1 of the 7.x branch of a module that integrates Darrin Lythgoe's The Next Generation
(TNG) genealogy software with Drupal. Both TNG8 and TNG9 versions are supported, with automatic detection
of the installed version at runtime.

The module creates a new account for any Drupal user who does not exist in the TNG user tables.

For users that do exist, the module creates an autosubmit form that logs the user automatically into TNG.

This module will only work if TNG and Drupal share the same database.

To install, save the TNG software in the /sites/all/libraries directory and install it as prescribed in the
TNG installation instructions. Then enable the tngintegrate module through the admin/build/modules menu.

You will then need to configure the module by setting several key variables in the /admin/settings menu under
TNG Genealogy.  These must be set.  Even if all of the values appear correct, click on "Save Configuration" to
set the variables.

1. Path to TNG Directory.  This should point to the folder on the server where the TNG software resides.
   The folder should be in the same directory as the Drupal installation.  The default is "sites/all/libraries/tng",
   which indicates that TNG would be installed in a folder called "tng" in the Drupal installation directory.
   You can install TNG in a different folder, as long as the path is not simply "tng".
   
2. Name of the TNG user information table.  By default, upon installation of TNG, this table is called "users."
   However, if you use a prefix before your TNG table names when installing TNG (which is recommended if you are
   putting these tables in the same database as the Drupal tables as is necessary for this module to work), you
   will need to add the prefix here as well.  The default value here is "tng_users" assuming you have added a 
   "tng_" prefix to your TNG table names.
   
3. Profile Fields for Realname.  You can specify up to two custom profile fields which you have created using
   Drupal's built-in profile module for this module to create the "Real Name" and "Description" field values in
   the TNG user database.  The value of this field is displayed when a user logs on.  If you specify two fields,
   they will be concatenated with a space in between.  
   
4. Error logging.  This determines whether the errors get logged into Drupal's watchdog module or get displayed
   on the screen.  The default is to use the Watchdog module.
  
5. Admin e-mail address for TNG.  This is the "from:" address for welcome messages when new users are created.

6. Subject Line for TNG Welcome e-mail message - self explanatory

7. Text of TNG welcome e-mail message - self explanatory.  This could include certain instructions on how to gain
   more than access privileges, for example, having a tree created for which the user would have edit privileges.
   
Permissions must also be set to allow access to the TNG Integrate Module.  This can be done through 
admin/user/access.  To access TNG through Drupal, a role's "access TNG" permission must be set.  The "administer
TNG" permission allows members of a role to change TNG permissions for any user and to change the TNG Integrate
Module's settings.
   
Once these settings are set, users will be added when they click on the TNG Genealogy menu item or when an 
administrator edits a user's TNG permissions.  TNG permissions can be set by anyone with the "administer TNG"
permission at /user/x/edit/tngintegrate where x is the user id.

When this module attempts to add a Drupal user to the TNG user table, it first checks whether there is a user in
the TNG user table with a username that matches the Drupal user name.  If there is, the two users will be matched
as a single user.  This is necessary because the TNG user table does not allow duplicate usernames.  This feature
also allows for easy upgrade from earlier versions of this module which used usernames, rather than user ids to
integrate the two user tables.

There are certain shortcomings with the current module which will be addressed in future versions, namely:

1. You cannot assign branch or tree-specific rights to a user from within Drupal.  This must be done through the
   TNG software.
   
2. Once a user has logged on to TNG through Drupal, she will no longer be able to log on to TNG directly (without
   logging on through Drupal) unless she has her password reset.  This is because the TNG Integrate module assigns
   a new randomly generated password to the TNG user account each time a new session is started through Drupal for
   security reasons.

3. The install file and a number of database calls in the code are MySQL specific, so the module may not work with
   PostgreSQL or other databse types.  I encourage other users more familiar with those database types to correct
   this bug.
   