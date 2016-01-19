DESCRIPTION
===========
This module extends the Flag module with Organic Groups support so that the
administrators of individual groups can override some of the settings of the
system wide flags.


REQUIREMENTS
============
Requires that the following modules be enabled:
* Flag (2.x branch)
* Organic Groups (2.x branch)
* Organic Groups Context (2.x branch)


INSTALLATION
============
1. Copy the module to your sites/all/modules folder.
2. Enable the Organic Group Flags (in the Flags group)


USE
===
When logged in as the administrator of a group, click on the "Administer Group"
tab.  There you'll see an option "Administer Flags" (group/node/%nid/admin/og-flag)
where %nid is the node id of the group.

The Administer Flags page displays a list of all flags that can be overridden.
Click the "Edit" link to override the settings for the flag.

NOTE: The OG Flag module uses the OG Permissions.  So when you override a flag
you must make sure that you have explicitly granted permission to use the flag
within your group.
 * Click Administer Group
 * Click on Permissions
 * Check the checkboxes next to the flag(s) users should be able to use.
    * If the Permissions page is read-only:
       * Click the Edit Tab
       * Change the field "Group roles and permissions" to be "Override default roles and permissions".

