CiviCRM Reroute Email
http://drupal.org/project/civicrm_reroute_email
====================================================

====================================================
CiviCRM Reroute Email is developed and maintained by
Ratan <ratan.kuet@gmail.com>
====================================================

Description
-----------
This module will reroute all email sent from CiviCRM to the specified
email address.

Firstly, using this we can test email functionality on a single email address
without changing contacts email address. Secondly, when an web application is
under development, we don't want to send accidental email to real users.
Using this module we can prevent all the accidental email from CiviCRM.
This will work for CiviCRM bulk mailing as well.

Installation
------------
1. Enable the module.
2. Go to Configuration > Development > CiviCRM Reroute Email.
3. Enable CiviCRM rerouting and enter an email address to reroute all the
emails to this address.

Testing
-------
1. Once installation and setup is done, you should verify whether rerouting is
working or not.
2. Go to a CiviCRM contact profile.
3. Try to send a mail to the user from "Actions > Send an Email" link.
4. You will see that the email will go to the reroute email address that
you set.
5. The email will not go to the actual email of the contact.

Permission
----------
User with the "Administer site configuration" permission can configure
the settings.
