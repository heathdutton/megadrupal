
Lazy registration for Drupal

Description
-----------
Lazy registration module implements lazy registration for Drupal.


Features
--------
It can create accounts automatically, purge them after a period of time, allow users to make them permanent by either email confirmation or a simple edit to the account.

The account is created the first time the user tries to access an area which requires authentication. There are two ways of handling a user trying to access a members only area:
- Automatically create a temporary account
- Display a login form with the option of continuing with a temporary account.

Users can confirm the account in two ways:
- By editing their account information
- By email confirmation to make sure people don't enter bogus email addresses


Installation
------------
Unpack the module in the modules directory and activate it in the "admin/modules" page. 

Configuration
-------------
Configure the module at "admin/settings/lazyreg".
If you want the users created by the module to have another set of permissions until they confirm the account, create another user role at "admin/user/roles". 
A brief description of the options:
- Enable lazy registration: this sets the "access denied" handler to the "lazyreg" page. Please note that this means that your previous handler (like logintoboggan) will be overwritten.
- Skip login screen: a temporary account will be created immediately without displaying the login page.
- Ask for email at the beginning: the user is asked his email address and an email is sent detailing how to confirm and keep the account.
- Simple confirmation: the account is confirmed when the user edits his profile.
- Require email confirmation: the user is sent a confirmation url. Please note that this option is still buggy.
- Purge accounts after how many days of inactivity: the number of days of inactivity after the temporary accounts are deleted. 
- Temporary user role: the user created will be given this role.
- Email template with password and Email template: templates for the emails sent to the user.


Author
------
Alexandru Badiu
http://www.voidberg.org