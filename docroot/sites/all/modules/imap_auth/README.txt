
Overview
---------
This module provides Drupal user authentication using an IMAP, POP, or NNTP server as the backend. The site administrator can choose which servers can be used for authentication, or enable all servers.


Requirements
------------
  * IMAP Auth requires the PHP IMAP extension to be enabled.
    (http://www.php.net/manual/en/book.imap.php)


Install
-------
Install the module by following the instructions at http://drupal.org/node/70151


Configuration
-------------
Configuration settings are located at Administer > Site configuration > IMAP auth

Enable IMAP Auth:
  * Disabled : Turn off IMAP authentication.
  * Enabled: Users will be able to logged in using IMAP/POP3/NNTP accounts.
 
Allow all servers:
  * Checking this box will allow any server to be used for authentication.


Adding a server
---------------
Server type:
  * The type of server you want to use for authentication.

Server name:
  * Server name is the part of the email address that comes after the @. For
    example, if your email address is you@example.com, the server name is
    example.com

Server address:
  * Server address is the location of the server that corresponds to the server 
    name above. For example, imap.example.com.

Port:
  * Optional. The port that the server runs on. Leaving this blank will have
    a negative impact on the speed of the connection to the authenticating
    server. You can try setting one of the following default ports.
    * 110 - POP3
    * 119 - NNTP
    * 143 - IMAP
    * 563 - NNTP with SSL
    * 993 - IMAP with SSL
    * 995 - POP3 with SSL

Mailbox name:
  * The name of the mailbox or newsgroup to connect to. If you're unsure
    what this should be, try INBOX. For some newsgroups, this can be left
    empty.

Use SSL?:
  * Check if your server requires a secure connection, or if you want the
    added security, if your server supports it.

Enabled:
  * Check to enable authentication with this server.
