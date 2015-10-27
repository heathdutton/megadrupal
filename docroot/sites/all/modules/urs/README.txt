Description
===========
User Role Summary module 

The User Role Summary module provides a block which gives a summary
of each of the user roles which exist, and which when clicked go directly to the user admin page filtered for users of that role type.  
 
For example, the block might contain something like the following:
- Administrator (1)
- Basic member (328)
- Full member (7251)
- On-hold user (114)
- Suspended user (504)

You will see that in brackets is the number of users who have that role.
 
If, for example, you click on the role "Full member" you will go to the
page http://example.com/admin/user/user with the user filter set to "Full member".

The block will be enabled on the http://example.com/admin/build/block page and has the title "User Role Summary".


Requirements
============
Drupal 7


Installation
============
1.  Copy the user_role_summary folder to the appropriate Drupal directory (e.g. sites/all/modules or sites/all/default).

2.  Enable the User Role Summary module in the "Site building -> Modules" administration screen (e.g. admin/build/modules).

3.  Enable the User Role Summary block (e.g. on the http://example.com/admin/build/block page)
