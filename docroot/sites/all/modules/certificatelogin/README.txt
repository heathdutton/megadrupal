CONTENTS OF THIS FILE
---------------------

 * Description
 * Installation
 * Settings / options

DESCRIPTION
-----------

Allow users to login by certificate.

INSTALLATION
------------

1. Place whole certificatelogin folder into your Drupal modules/ directory.

2. Enable certificatelogin module by navigating to
     Administer > Site Building > Modules (admin/build/modules)
     
3. Bring up certificatelogin configuration screen by navigating to
     Administer > Site Configuration > Certificate Login Settings 
    (admin/settings/certificatelogin)
     
4. Configure all settings 

SETTINGS / OPTIONS
------------------

Most options should be pretty much self-explanatory. But some may not! 

 * "PHP code to retrieve user name"
   This code should evaluate to the username that will be taken from the
   environmental variable. For example '$GLOBALS['user']->mail'. The value will
   be used as a username so it must be unique. An email address is a good choice.
   Do not include PHP tags.
 * "Current Login Override"
   If the user is currently logged in and this option is enabled they will be
   logged out and logged in as the user corresponding to their certificate. If it
   is disabled they will only be logged in if they are not currently logged in.
