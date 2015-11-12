OVERVIEW
--------
The Checkmail module checks a POP3 or IMAP e-mail account and prints one or
more of the following: the total number of e-mail messages in your INBOX, the
number of recent (new) e-mail messages (IMAP only), the number of unread e-mail
messages (IMAP only), the total size of the e-mail messages in your mailbox.

NOTE: If you turn off the cache setting for the block, it will query the
e-mail server on every page load where the block is displayed. It is a good
idea to configure the block to display on only certain pages and/or confirm
with the mail server administrator the amount of use you expect. The default
cache expiration before re-checking the server is 1 minute.

This module can encrypt the login password(s), if you are using either the AES
encryption (https://drupal.org/project/aes) or Encryption
(https://drupal.org/project/encrypt) modules. If you choose not to use either of
them, your passwords will not be encrypted, so that if your server is
compromised, the attacker could have access to your plaintext password(s).


REQUIREMENTS
------------
- Permission to use the fsockopen() and other socket functions in PHP.


CONFIGURATION
-------------
There are several configuration options to set:

- POP3 email server
Enter your e-mail server address: mail.example.com

- POP3 email port
Enter the connection port, used to get access to the mail server. If you
don't know what this is, leave the default configuration for port 110.

- POP3 email username
- POP3 email password


AUTHORS
-------
Stefan Nagtegaal: http://drupal.org/user/612 http://www.Sempre-Crescendo.nl/
Kristjan Jansen: http://drupal.org/user/11
David Kent Norman: http://drupal.org/user/972 http://deekayen.net/
Jason Flatt: http://drupal.org/user/4649
