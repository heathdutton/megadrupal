$Id: README.txt,v 2.0 2011/05/06 12:00:00 francoud Exp $

          JOIN_ROLE_WITH_PASSWORD - README

--------------------------------------------------------------------
NAME:       JOIN_ROLE_WITH_PASSWORD  for Drupal 7
AUTHORS:    francesco brunetta <brunetta@uniud.it>
--------------------------------------------------------------------


---------------
DESCRIPTION
---------------

This module allows users to "join" a role just using a given password. 

Site administrator can decide WHAT ROLE users should be able to join using
a password, and WHAT USERS should be able to use this feature.

Users can join selected roles, if they know the correct password; they can
then leave that role, also if they dont know the password.


---------------
REQUIREMENTS
---------------

Drupal 7.

---------------
INSTALLATION 
---------------

Extract all files into your "modules" directory. 

Enable the module: 
  Go to "administer >> build >> modules" and put a checkmark in the 'status' column next
  to 'join_role_with_password'.

---------------
USAGE
---------------


Enable permissions appropriate to your site. This module provides two permissions:
	- 'administer join_role_with_password'
	- 'join new role'

The Site administrator, or users with the 'administer join_role_with_password' permission,
can then manage roles' passwords accessing:   admin/config/people/role_password




Here you can:
 - add a password to a role: when u add a password to a role, users with the "join a new role"
   access permission will be able to join that role, just using the password you defined. No
   other authorization required. Better that you add password only to roles that are not very
   privileged. 
 - change password to a role: if you previously defined a password for a role, you can also
   change it  (admin/config/people/role_password/changepwd)
 - remove password from a role: if you previously defined a password for a role, you can then
   remove the password; if you do it, users will not be able anymore to join that role, or
   to leave it! admin/config/people/role_password/remove
 - define some options for the module. For version 2.0, you can:
     - enable or disable (default: enabled) the "leave role" section: if disabled, users will
       not be able to leave a role after they joined it (they can only join roles).
     - define a minimum length for password (from 0 to 10, where 0 means: no minimum length for
       passwords).

Site administrator should give "join a new role" permission to selected user's role, but also to
all authenticated users. 

Users with the 'join a new role' permission can Join or Leave a role. They will find a
new "Join or leave a role" tab in their user's profile (user/x/join_role):
    My account >>  Join or leave a role

They will be able to join a role, giving the proper password; they will be able to leave a role
that they previously joined, also without knowing the password.

---------------
TECHNICAL HINTS
--------------- 

This module creates a new table, "roles_passwords", in which it will save role id and passwords.

WHen admin add a password to a role, this role id, and its password, are written in the table.

When users want to join or leave a a role, they receive a list of roles from that table, if they
also exists in the "role" table.

When admin remove a password to a role, the entry is removed from roles_passwords table. No
modification is ever done to the "role" table.

When admin completely remove a role, it is possible that some old role id remains in the
roles_passwords table; this is not a real problem: since they are no more included in the role
table, they will not be shown to the user. Anyway, each time administrator clicks in the
admin/user/role_password tabs, a garbage collection starts, and each inconsistent information
will be removed. 

Note that, if an user join a role, and then the administrator remove the password from that
role, user will STILL have that role, and will NO MORE be able to leave it (users must be
modified manually).


It's possible that admin or users receive this error:

     An illegal choice has been detected. Please contact the site administrator

It's only possible when: 
1) user open the "Join or leave role" form
2) admin, at the same time, remove a password from a role, or completely removes a role
3) user then try to join or leave a role that is no more present, or has no more password.

You shouldn't worry too much about this error. Should be rare. The only way to avoid it,
is disabling the drupal form's api built-in security check; it's possible, but at the moment
I preferred not doing it. We'll see in the future (it could be a configuration option).

See also: http://drupal.org/node/153774 and http://groups.drupal.org/node/1839





