Module description
------------------

The Postmark module allows the administrator to switch the standard SMTP 
library over to use the third party Postmark library. An account with 
Postmark is required to use this module:

https://postmarkapp.com/

The recommended third party library for this module is the latest version
of the Postmark PHP class v0.4.5 by Markus Hedlund et al.
http://github.com/Znarkus/postmark-php

Note: The module uses the REST API to connect to Postmark.

Installation
------------

Installing the Drupal 7 version of the Postmark module requires several steps:

1) Install required modules as follows:

   Download the Mail System module into sites/all/modules, then enable it.
   http://drupal.org/project/mailsystem

   Download the Libraries module into sites/all/modules, then enable it.
   http://drupal.org/project/libraries

2) Download the external Postmark PHP library into sites/all/libraries.
   The library is available at https://github.com/Znarkus/postmark-php
   If you are using Git, this can be done using these steps:

   $ cd /var/www/html/sites/all (or an appropriate path on your system)
   $ mkdir libraries (if the folder doesn't exist yet)
   $ cd libraries
   $ git clone git://github.com/Znarkus/postmark-php.git

   The Postmark PHP library should now reside here:

   sites/all/libraries/postmark-php

3) Download the Postmark module to the modules folder in your installation.
   This is usually sites/all/modules.

4) Enable the Postmark module using Administer > Modules (/admin/modules).

5) Go to Configuration > Postmark (admin/config/system/postmark).

6) Enable Postmark functionality and add your API key from your Postmark account.
   You can get an API key here:  https://postmarkapp.com/

7) The test functionality enables you to test the integration is working, this 
   will use a credit by default each time you submit an email address.

8) The email address that emails are sent from on your site must have a valid 
   Sender Signature set up in your Postmark account. Different modules use 
   different settings for the "From" address. One important place to check is 
   the address on Configuration > Site information.
	 
9) Emails sent by the core Contact module use the sender email as the Reply To
   address, and will append the sender's email address to the bottom of the email
	 so the recipient can see who submitted the Contact form.  This is necessary
	 when using Postmark, as all emails must be sent from a sender defined by a
	 Sender Signature.  The "From" email address is the address defined by the
	 setting on Configuration > Site information.

10) Configure the Mail System module (setup in step 1) so that all modules use
    Postmark to send email.  Alternatively, you can configure it so some modules
		use other mail modules or the default Drupal mail handler if you'd like. For
		example, if you don't like the way the Contact module is handled, you can
		set things up so Contact module emails are sent by the regular Drupal mail
		handler.

Support and bugs
----------------

If you have any problems using the module, please submit an issue in the 
Postmark queue (http://drupal.org/project/issues/postmark).

That's also a good place to check for known problems and "todos".

Credit
------

The Postmark module was developed by
 * Luke Simmons (luketsimmons)
 * Allister Price (alli.price)
from Deeson Online (http://www.deeson.co.uk/online).

The Drupal 7 version of Postmark was developed by:
 * John Oltman (john.oltman)
from SiteBasin.com
 
Credit also goes to the phpmailer (http://drupal.org/project/phpmailer) module on 
which this module is heavily based.
