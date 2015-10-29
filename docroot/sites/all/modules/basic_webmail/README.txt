ABOUT
-----
The Basic webmail module allows one to read and write e-mail through an IMAP
mail server. There are not a lot of fancy features (thus the name), just a
usable webmail client.


INSTALLATION
------------
Install the module in the usual way, which is documented on this page:
http://drupal.org/documentation/install/modules-themes


CONFIGURATION
-------------
The configuration settings are on the admin/config/system/basic_webmail page.

For each user who will be allowed to use this module, their individual settings
must be configured, as well. That is done in their "My account" page, which is
located at user/$uid/edit, where $uid is the user ID # they received when
registering for the site. Look for the fieldset labeled "Basic webmail" and fill
in the fields.

If you are trying to get this to work with Gmail, here is the configuration that
worked for me:
* In Administration -> Configuration -> Basic webmail
  (admin/config/system/basic_webmail), set the following:
  * In the "Server settings" field set:
    * Set the "Server address" field to "imap.gmail.com" (w/o the quotes).
    * Set the "Connection port" field to "993" (w/o the quotes).
    * Check the "Encrypt session using SSL" check box.
    * Uncheck all the others.
  * The other settings are personal preference and can be set to what ever you
    wish.
  * Save the settings.
* In My account -> Edit, set the following:
  * In the "Basic webmail account settings" field set, set the following:
    * Put your GMail e-mail address in the "E-mail account login" field,
      including the @gmail.com part.
    * Put your GMail password in the "Password" and "Confirm password" fields.
  * Save the settings.

If you are using the PHPMailer (https://drupal.org/project/phpmailer) module,
configure it thus:
* In Administration -> Configuration -> PHPMailer
  (admin/config/system/phpmailer), set the following:
  * Check the "Use PHPMailer to send e-mails" check box.
  * In the "Primary SMTP server" field, put "smtp.gmail.com" (w/o the quotes).
  * You can leave SMTP Backup Server blank, or put in the name of some other
    SMTP server, but it will have to use the same settings for the following two
    options.
  * In the "SMTP port" field, put "465" (w/o the quotes).
  * Set "Use secure protocol" to "Use TLS".
  * In the "SMTP Authentication" field set, set the following:
    * In the "Username" field, put your GMail e-mail address, including the
      @gmail.com part.
    * In the "Password" field, put your GMail password.
  * The rest of the settings are optional.

If you are using the SMTP Authentication Support
(https://drupal.org/project/smtp) module, configure it thus:
* In Administration -> Configuration -> SMTP Authentication Support
  (admin/config/system/smtp), set the following:
  * In the "Install options" field set, set "Turn this module on or off" to
    "On".
  * In the "SMTP server settings" field set, set the following:
    * In "SMTP Server", put "smtp.gmail.com" (w/o the quotes).
    * You can leave SMTP Backup Server blank, or put in the name of some other
      SMTP server, but it will have to use the same settings for the following
      two options.
    * In the "SMTP port" field, put "465" (w/o the quotes).
    * Set "Use encrypted protocol" to "Use TLS".
  * In the "SMTP Authentication" field set, set the following:
    * In the "Username" field, put your GMail e-mail address, including the
      @gmail.com part.
    * In the "Password" field, put your GMail password.
  * The rest of the settings are optional.


PERMISSIONS
-----------
There are three permission settings available for this module:
  administer basic_webmail:
    This permission allows a priviledged user to access the various
    configuration settings of this module.
  access basic_webmail:
    This is the basic access permission to the normal use areas for viewing and
    sending email.
  access users' email addresses:
    This permission gives users sending emails with Basic webmail access to the
    email addresses of the users of your site. It is used for automatically
    populating the To, CC, and BCC fields of the send mail form. If you don't
    want everyone having access to all the users' email address on your site,
    then do not give anonymous or authenticated users this permission.


SECURITY CONCERNS
-----------------
This module stores users' email account password in plain text in the data
column of the users table. If you choose the "Use the user's account e-mail
address and password for checking e-mail." option in the configuration settings,
this means that their website password is the same as their email password and
is stored as plain text in the database. If you are using this module in a
closed or controlled environment (like for inter-office email), this may not be
too much of an issue. If you would prefer that the users' passwords were not so
accessible, this module will utilize either the Encryption
(http://drupal.org/project/encrypt) or AES encryption
(http://drupal.org/project/aes) modules to encrypt and decrypt the users' email
passwords, as necessary. Please note that if you enable encryption after using
this module, users will have to re-save their passwords for the encryption to
start.

Also, if you do not want people to have access to the email addresses of the
users on your site, then be careful which roles you give the "access users'
email addresses" permission. That permission gives users sending emails with
Basic webmail access to the email addresses of the users of your site through
the automatic populating the To, CC, and BCC fields of the send mail form.


AUTHOR
------
Jason Flatt
drupal@oadaeh.net
http://drupal.org/user/4649
