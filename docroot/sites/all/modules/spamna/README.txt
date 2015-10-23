INTRODUCTION
============
The Spamna (=Spammer Node Add) module checks for users who have attempted to
do a disallowed "node/add" and thus judges that they are spammers. Optionally
it deletes the users found.

REQUIREMENTS
============
The core 'statistics' module should be enabled for this to work. Only when the
core statistics module is enabled are the page requests such as "node/add" 
logged.

INSTALLATION
============
Install as you would normally install a contributed Drupal module. For example,
place the files in the sites/all/modules/spamna directory and enable the module
at the admin/modules page. 

See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information. 

On the admin/people/permissions page, grant the 'delete spammers' authority to
user roles who you want to be able to run these checks.

USAGE
=====
Go to the path spamna/check (e.g. http://www.mysite.com/spamna/check) and you 
will then be presented with a form where you can run the spammer check. It is
best to run the check first without selecting the option to delete the spammer
users, so that you can check that the users which would be deleted are actually
spammers.