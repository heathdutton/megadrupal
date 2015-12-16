ILS Authentication
==================

PURPOSE
-------

ILS Authentication is a framework for authenticating users of a Drupal site
against Integrated Library Systems. Each ILS requires a 'driver' to work 
with this module. A driver is an .inc file that connects to the ILS, queries 
its user database/API, and returns to the ILS Authentication module:

  1) a TRUE or FALSE value (depending on whether the current user credentials 
     authenticate against the ILS); and
	
  2) optionally, an email address from the ILS account.

Since ILS Authentication uses standard hooks to integrate with Drupal's account 
registration and authentication subsystems, existing and new local Drupal
accounts (i.e., those created by the core user module) and accounts created by
other contributed modules are unaffected by this module.

Site admins who install ILS Authentication must select which driver to use. 
The 'sample' driver is installed by default. Only one driver can be active.
The module only supports authentication against a single external source.

Please note that this module will create accounts for users if they
authenticate against the configured external source, regardless of whether
the site administrator has chosen "Visitors can create accounts and no
administrator approval is required" under "Public registrations" at
admin/user/settings. 
Local Drupal accounts are governed by the this setting as usual.

FEATURES
--------

This module:

-supports new authentication sources by using 'drivers', which are PHP .inc
 files that contain only a few functions (documented below). This modular
 approach allow new drivers to be added without needing to modify the .module
 file.

-allows drivers to
   -add their own admin settings
   -modify Drupal forms for their own purposes, such as the login form or 
    the module settings form
   -perform their own validation on the login form

-allows site admins to
   -tell users (when they log in) that they must add an email address to their 
    profile, since not all external authentication sources can provide an email 
    address for the user.
   -define driver-specific messages to be displayed when an external user 
    requests a new password, and also to be displayed in external users' 
    account pages. Since external users may not have email addresses 
    (something core Drupal does not allow), external users do not receive
    an email when they request a new password. Instead, they see a 
    driver-specific message telling them where to change their password
    (i.e., at the remote ILS). Drupal's default behavior of sending an email 
    to users requesting new passwords still applies to local accounts.
   -define default roles for user accounts created with this module.
   -log usernames/passwords routed to external sources. This feature is intended
    to assist driver developers and should be turned off at all times other 
    then during testing and debugging.

PREREQUISITES
-------------

The module itself has no particular PHP, database, or Drupal requirements, 
other than it only works with Drupal 7.x.

Certain PHP configuration options must be enabled to allow drivers to
connect to their authentication targets. For example, if a driver connects
using a URL, file_get_contents or cURL must be enabled, depending on how the
driver is written. 

INSTALLATION
------------

Just like any other module: unpack the distribution in your site's module
directory, go to Administration > Modules, and enable. For configuration
go to Administration > Configuration > People > ILS Authentication.

ILS Authentication does not create any database tables but does insert 
some entries into the variable table. If you uninstall the module, 
these entries are removed.

By default, a 'sample' driver is installed. Site admins will need to select
another driver in the 'ILS Authentication' admin form, since 'sample' doesn't
connect to an external authentication source.

In order to illustrate how drivers can modify Drupal forms, the sample
driver changes the labels on the fields in your site's user login form 
to 'Your custom username field title' and '... and a custom password field
title.' To disable this, comment out the first 'if' block in sample.inc's
ilsauthen_form_alter_driver() function.

UNINSTALLATION
--------------

This module cleans up all {variable} table entries it inserts. It does not
create any database tables on installation.

On uninstallation, all accounts created using this module are converted 
to regular Drupal local accounts (that is, the association between the 
ILS Authentication module and the accounts is deleted). Users will be
able to log into the site with the password that they used the last time
they logged in before the module was disabled. This is possible because 
each time an external user logs into the site with ILS Authentication 
enabled, ILS Authentication stores an encrypted version of the remote 
password in the user's record. Should the user change her password on the
remote ILS after the module is disabled but before attempting to log into
Drupal again, her attempt will fail because the remote and local passwords
are no longer the same, and she will be presented with the standard Drupal
response to unrecognized passwords.

DRIVERS
-------

ILS Authentication comes bundled with several drivers. You are encouraged 
to write your own and if possible, contribute it to be distributed with the
module so others can use it. See drivers/sample.inc for inline comments
in addition to the following brief guide.

To create a driver, put the following five functions in a file in the
module's drivers directory. Your file must end in .inc and should be named
to describe what external authentication source it connects to. However,
since the file's name (without the .inc) appears in the Drivers select list
in the module's settings form, keep the name short.

The following functions must appear in every driver: 

ilsauthen_driver_meta() defines basic information about the driver. Currently,
only the name is defined.

ilsauthen_form_alter_driver() allows you to modify Drupal forms using
hook_form_alter(). $form[$reset_password_message_element] is required, as it
stores the message that users will see when they request new passwords. This 
message is required because Drupal doesn't manage passwords for accounts
created by external authentication services.

ilsauthen_driver_login_validation() allows you to validate form elements that
you add to the user login form with ilsauthen_form_alter_driver().

ilsauthen_get_email_address() simply returns the account's email address back to
the ilsauthen module. In most cases you can use an exact copy of this
function as it appears in the sample.inc driver.

ilsauthen_driver_connect() is the function that actually connects to the
external authentication service. This function can be quite complex, depending
on how you need to connect to the external service and what type of data you
need to manipulate to determine if the user's credentials authenticate
against the service. At a minimum, this function must return TRUE if the 
credentials authenticate and FALSE if they don't. Optionally, you can populate
the $_SESSION['ilsauthen_driver_mail_address'] variable inside this function
(if your external authentication service stores user email addresses and you
can access them) so that user accounts created with ILS Authentication will
contain an email address. Drupal allows programmatic creation of user accounts
with no email address but without one, the user will not be able to receive
messages from the site and they won't be able to update their account profile
without adding an email address, since the account edit form makes the email
address field required.

SECURITY
--------

ILS Authentication does not require, or add, any specific security measures 
beyond those provided by Drupal itself. 

If you write a driver for an external authentication service, you need to 
secure the connection between Drupal and the service yourself. Since most 
drivers will use some sort of web service to connect to authentication
services, you should consider using the standard technologies that other web
clients and servers use, such as HTTPS, restricting access to your external
service to localhost (where possible) or to a specific IP address, etc.

Passwords for accounts managed by external authentication sources are stored in 
the local Drupal database, but they are encrypted using md5 in exactly the same 
way as local Drupal passwords are. As explained under "Uninstallation" above, 
passwords are stored so users can log into the local Drupal even if the ILS 
Authentication module is disabled.

Since the sample.inc driver contains credentials in ilsauthen_driver_connect()
that anyone can use to log into your site (provided they know those credentials,
you have this module enabled, and you have enabled left the sample driver
enabled), you may want to change the sample password. If you enable another
driver, this is not an issue. DO NOT DELETE sample.inc.

As stated above, this module provides an option to log usernames/passwords 
routed to external sources. This feature is intended to assist driver
developers and should be turned off at all times other then during testing and
debugging.

KNOWN ISSUES/LIMITATIONS
------------------------

-This module only populates the username (name), password (pass), and, 
 optionally for each driver, the email address (mail) fields in the 
 local Drupal user table. Currently it does not support populating 
 fields managed by Drupal's profile module or by contrib modules that
 replace the core profile module.


-When an external user requests a new password, the core message "Further
 instructions have been sent to your e-mail address" is displayed along with 
 the active driver's password request message.

----------------
End of README.txt
