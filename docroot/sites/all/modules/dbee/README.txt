******************************************************************** 
D R U P A L M O D U L E 
******************************************************************** 
Name: DataBase Email Encryption (dbee) 
Author: Julien Duteil <julienledut at gmail dot com>
******************************************************************** 

DESCRIPTION: 

This module protects users email address, encrypting them into the 
database. This module doesn't alter user experience. In case of database 
hacking, this sensitive data would be useless for the hacker. 

It uses the AES module API, providing the encryption method. 
Easy to use : 
- install the DataBase Email Encryption (dbee) module : the encryption 
will be enabled for all users email address. 
- Uninstall it or disable it : it will disable the users email 
address encryption for all users email address. 

To access to the administration options page, navigate to "configuration > 
System > DataBase Email Encryption settings".
This module provide an extra features : Allow to loggin using either the 
username or the email address. It is disabled by default. 
This module add a new field in the user view page, expliciting if the 
email address is effectively crypted or empty. Only users with 
"administer database email encryption" permission are able to see it. 

For security reason, it is highly recommanded : 
- to store encryption key into a file outside the webroot, instead of into
the database (read "installation" section and "Note" section) 
- and to KEEP A COPY OF THE ENCRYPTION KEY into a secure location : the 
encryption key is not saved on batabase backup, AND LOOSING THIS KEY 
WOULD DEFINITELY PREVENT ACCESS TO ALL YOUR USER EMAIL ADDRESSES !! 

******************************************************************** 
WHY THIS MODULE WAS CREATED ? 

Several big companies were victims of database hacking, each 
successfully attacks exposing millions emails and personnal datas. 

******************************************************************** 
WHAT IS THE DIFFERENCE BETWEEN THIS MODULE AND OTHERS LIKE : 

- OpenPGP (http://drupal.org/project/openpgp) 
- SpamSpan filter (http://drupal.org/project/spamspan) 
- Invisimail (http://drupal.org/project/invisimail) 
Those modules provide methods to hide email addresses from spam-bots. 

- mollom (http://drupal.org/project/mollom) 
- CAPTCHA (http://drupal.org/project/captcha) 
Those modules provide methods to determine if the author is either a 
human people or a spam-bot. 

The DataBase Email Encryption provide protection for the users email 
address from an other kind of attack : the database stealing. 

******************************************************************** 
MAINTENANCE: 

When you procced to regular database backup of your website, be careful 
to save both database AND the file that contains the encryption key. 

******************************************************************** 
ABOUT CUSTOM MODULES COMPATIBILITY: 

The dataBase Email Encryption module is compatible with drupal core. 
Thanks to various hooks, most custom modules should be compatible with 
the DataBase Email Encryption (dbee). Some won't be comptatible or may 
need, usually, fast adjustments by a drupal developper (thoses 
adjustements may be written directly into the dbee module). As an 
example, thanks to provided fixes included into the dbee module, the 
dbee module is natively compatible with the custom modules : 
- "Email Registration" (http://drupal.org/project/email_registration)
- "logintoboggan" (http://drupal.org/project/logintoboggan)
If you experiment compatibility issues with some custom modules, please 
report thoses issues on http://drupal.org/project/issues/dbee, as I may 
fix them.

******************************************************************** 
NOTE ABOUT SECURITY: 

Obfuscation versus Security (extract from 
http://drupal.org/project/encrypt) 

It should be noted the difference between obfuscation [1] and security. 
It is important to understand how security is at work and where the 
points of failure are. 

By default, this module uses a key that is stored in your database, 
while the main use of this module is to store encrypted data in the 
database. This is actually just an example of obfuscation because if the 
database itself is compromised, all the appropriate parts are available 
to retrieve that data, though much more complicated once the data has 
been encrypted. 

When you put your key outside the webroot, the encrypted text and key 
are now in two distinct parts of the system which will have a lot less 
likelihood of being compromised at the same time. It is still important 
to know that this module does not make your data completely secure, it 
just allows a level of security that Drupal does not inherently provide, 
and in fact there are many levels that need to be thought about to have 
fully secure data. 

[1] : http://en.wikipedia.org/wiki/Obfuscation 

******************************************************************** 
NOTE ABOUT THIS MODULE DEVELOPMENT : 

BECAUSE THIS PROGRAM HANDLES SENSITIVE DATA, I RECOMMAND TO PERFORM 
DATABASE BACKUP BEFORE USING IT.
This module has been developed respecting Drupal security guide 
("Writing secure code") and drupal best practices. Features have been 
heavily tested using simpleTest development tool. 

The program is provided "as is" without warranty of any kind, either 
expressed or implied, including, but not limited to, the implied 
warranties of merchantability and fitness for a particular purpose. The 
entire risk as to the quality and performance of the program is with 
you. Should the program prove defective, you assume the cost of all 
necessary servicing, repair or correction. 
