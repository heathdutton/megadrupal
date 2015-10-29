Overview
---------

cPanel is a next generation web hosting control panel system. cPanel is 
extremely feature rich as well as include an easy to use web based interface 
(GUI).

cPanel is designed for the end users of your system and allows them to control 
everything from adding / removing email accounts to administering MySQL 
databases.

This module provides the factility to automatically create email and FTP 
accounts for every Drupal user using cPanel administrative interface.

Install
-------

1. Upload cpanel.module to the Drupal modules/ directory.

2. Activate the module via the Drupal module configuration menu
   (admin/modules)

3. Configuration: (admin/config/cpanel)

cPanel server settings:
 * Name: cPanel server name
   Example: cpanel.mycompany.com
 * Port: cPanel server port. 2082 by defaut
 * Username: cPanel administratvive username provided by your ISP
 * Password: cPanel administratvive password provided by your ISP

eMail accounts settings:
 * Email Accounts Creation Status:
   Enabled: Allow creation of email accounts 
   Disabled: Creation of email accounts will be turned off
 * Mail quota: Amount of disk space the account can use (in Mb)
 * Mail domain: a valid email domain (or subdomain) on wich email
   accounts will be created (the part after "@")
   Example: users.example.com
   Note:
   You should create this domain (or subdomain) in your cPanel
   administrative interface.
   
FTP accounts settings:
 * FTP Accounts Creation Status:
   Enabled: Allow creation of ftp accounts 
   Disabled: Creation of ftp accounts will be turned off
 * FTP quota: Amount of disk space each account can use (in Mb)
 * FTP directory root:
   Directory which the account will have access under 
   "/home/youaccount/public_html" folder.
   Example: /users/
   Be aware that this value MUST start and end with a "/".
   If you enter a single "/", the new FTP user will have 
   access to "public_html/username" directory and all directories under it.
   If you enter a value "/directory/", the new FTP user will have 
   access to "public_html/directory/username" directory and all directories 
   under it.
 * FTP global domain: Main domain of your cPanel account
   Example: example.com

3. Administrative Actions: (admin/config/cpanel)
 * Massive quota change:
   You can change the quota of All email or FTP accounts
 * Massive aging change:
   You can change the number of days to keep e-mail of All email accounts.
 * Massive accounts deletion:
   You can delete ALL email or FTP accounts.
   WARNING: This action can't be undone. Deletion of email or FTP accounts
            can delete all messages and files of users. Use it at your own
            risk!.
