DESCRIPTION
-----------

Simplenews publishes and sends newsletters to lists of subscribers.

This module adds some new permission to permit more users (admin) to manage
their newsletter.

The admin (uid = 1) or who has the permission can also change the owner of 
the newsletter and the old nodes of that newsletter. 

A new menu "My newsletter"  adds the links to go directly to the administration
pages: these pages show Simplenews data for users.

REQUIREMENTS
------------

 * Simplenews

INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "simplenews_owner_newsl" in the sites/all/modules
    directory and place the entire contents of this simplenews_owner_newsl
    folder in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page.

 3. ACCESS PERMISSION

    Grant the access at the Access control page:
      People > Permissions.

 4. CONFIGURE SIMPLENEWS OWNER NEWSL

    No configuration has needed from this module.

CONFIGURATION
-------------

The module doesn't need configuration.

Inside the user's permissions there are the new 'Administer own newsletters' 
and 'Administer own simplenews subscriptions', to select the users to which 
you grant administrator role, and 'Administer own newsletters by role' and 
'Administer own simplenews subscriptions by role' to grant the administrator 
permission to a role, instead a single user. 

The role permissions grant only to administer simplenews/newsletter but 
don't permit to edit the newsletter/node. To do this it needs to select 
the "The nodes owner".

If are set simplenews template for the newsletters, the user must have set 
also "View the administration theme" to load the *.tpl.
