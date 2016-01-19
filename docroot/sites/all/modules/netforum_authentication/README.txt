README

Thanks to everyone at Avectra for helping out with my questions on this, 
including but not limited to Kerim Guc, Darryl Hopkins, Devin Dasbach, and Ford Parsons

The authentication module provides the following key functions:

- allows netForum customers with a web username and password to log in
- fetches user information from netforum
- updates information in netforum when user changes information
- assigns users roles based on information in netforum
- signs users on to eWeb
- Cross domain SSO

Most of those require setup before use.

Mapping netForum users to drupal roles depends on two steps, 
the first is getting the criteria from the database, and the second
is matching the user to those criteria.  By default it will search based on
membership information.  See netforum_auth_user_categories() and 
netforum_auth_categories()

Cross domain SSO allows two drupal sites running this module to sign a user 
onto eweb using one of the domains.  So if there were www.example.com, 
eweb.example.com, and www.different.com, both drupal sites can use sso
to make a user on eweb.example.com appear logged in.

Read more about how to set up and configure netforum authentication at
admin/help/netforum_authentication