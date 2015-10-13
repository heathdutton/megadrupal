
Pubcookie module
================

DESCRIPTION
-----------
This module integrates pubcookie-based authentication with Drupal.

For more information about pubcookie, see http://pubcookie.org.

PREREQUISITES
-------------
Before using this module, make sure that your pubcookie environment
is set up and running correctly.

The best way to do this is to create a subdirectory on your webserver
with an HTML file and an .htaccess file in it. The .htaccess file
would look something like this:

Authname "Testing"
PubcookieAppID foo
Authtype NetID  <-- or whatever Authtype your organization uses
Require valid-user

Upon trying to access the HTML file, Apache should redirect you
to your pubcookie server, and upon authentication the pubcookie
server will redirect back to your HTML file. When all that is
working, you are ready to try pubcookie.module.

INSTALLATION
------------
Place the entire pubcookie module folder into your third-party modules
directory, typically at sites/all/modules.

Enable the module in Drupal by going to Administer -> Site building ->
Modules.

Set up pubcookie.module by going to Administer -> Site configuration -> 
Pubcookie.

Ensure that Clean URLs are enabled at Administer -> Site configuration ->
Clean URLs.

If desired, enable the pubcookie login block by going to Administer -> 
Site building -> Blocks.

ID/E-MAIL EQUIVALENCY
---------------------
Checking the ID/E-mail equivalency checkbox says that the distributed
login ID, such as jsmith@example.edu, is also a valid email address.
In some cases the distributed IDs are not valid email addresses
(for example, your drupal.org ID is not a valid email address).
If this box is checked, during the registration process the pubcookie
module will insert the user's ID into the mail column of the user table.

PUBCOOKIE SITE ACCESS (optional)
--------------------------------
Pubcookie Site Access is a module that is bundled with the pubcookie
module. It allows you to restrict login to a whitelist of known usernames.
For example, if you are creating a site for example.edu, all example.edu
users may log in. By enabling the Pubcookie Site Access module, you
can restrict login to just users tom, dick, and harry by going to
Administration / Configuration/ People / Change who may access this site.

The permission to add/delete users from the whitelist is called "administer
pubcookie site access".

Perhaps a better name for the module would have been Pubcookie Access
Control List.

Roles can also be assigned to future users (i.e., users who are on the
whitelist but have not logged in yet) so that when they do log in they
are assigned a role or roles.

The permission to assign future roles to users on the whitelist is called
"administer pubcookie future roles".

Role assignment to users who have already logged in is possible if the person
assigning has 'administer users' and 'administer permissions' access; in other
words, the same permissions required to assign roles in Drupal core.

LDAP INTEGRATION
----------------
LDAP is integrated into pubcookie module. If you don't want to use LDAP,
leave the LDAP server setting blank.

To have user fields automatically populated by LDAP when users
register, name the Machine Name of the field to be the same
as the LDAP directory field name. For example, if your LDAP directory
returns a field called "displayname" containing the user's full name,
go to Administer -> Configuration -> People -> Account Settings and click
on the Manage Fields tab, then add a new field of type Text with the Machine
Name "field_displayname".

AUTO-USERNAME ASSIGNMENT
------------------------
By default Drupal gives new users registering via distributed authentication
a username that is the same as their distributed authentication ID.
You may not want this. For one thing, if a user makes a post and the
user's username is joe@example.edu and that is a real email address
(see ID/E-MAIL EQUIVALENCY, above), that real email address will now
appear in "submitted by" links and be picked up by hungry web crawlers.

By assigning an LDAP field to be used as username, this problem is avoided.

HOOKING INTO PUBCOOKIE USER ACCOUNT CREATION
--------------------------------------------
The Drupal hooks that fire during the login process are not helpful for
determining a first-time pubcookie-based authentication. So pubcookie provides
hook_pubcookie_post_register() in case you want your own module to do something
after the pubcookie account creation is complete. For example, we often use
that hook to match existing content that should be assigned to the user and
assign ownership programmatically, or create a new node that is the user's
profile when we don't want to use Drupal's user-based profiles.


GOTCHAS
-------
If you check the ID/E-mail equivalency checkbox so the mail column of the
user table is populated, and if you have local users with the same email
address, you cannot update the accounts through Administer -> User management
-> edit because Drupal says "The email address joe@example.edu is already
registered." That is because email addresses must be unique in the user table.

TROUBLESHOOTING
---------------
If you see the message "Pubcookie request received over non-secure protocol"
while trying to login, you probably haven't created the directory and
.htaccess file that pubcookie's "Login directory" setting points to.
Or you may not have clean URLs enabled; e.g., your login link is pointing
to q=/pubcookie/pc when you want it to point to /pubcookie/pc.

If you see "Forbidden: You don't have permission to access /login/ on this
server" you should change the name of the login directory that contains your
.htaccess file. You cannot use "login" as the name of your login directory
and also use the login redirect that pubcookie provides at
http://example.edu/login. Naming the login directory something else, e.g., 
pubcookie, will alleviate this problem.

WHY IT WORKS
------------
When you click on the Log In link provided by the pubcookie block, it takes
you to the directory you specified for "Login directory" under Administration /
Configuration / People / Pubcookie (by default, 'pubcookie'). The pubcookie 
module takes this path, adds "pc" (an arbitrary string) to the end of it and 
-- and here's the key -- registers it as a menu item in the menu hook. So now
http://yourdomain.com/pubcookie/pc is not a nonexistent file but a registered
Drupal path that is "located" inside a directory that's protected by a
.htaccess file restricting the contents to pubcookie-server-authenticated
users. So when you reach that path, the pubcookie module receives a call
to pubcookie_page() and goes from there.
