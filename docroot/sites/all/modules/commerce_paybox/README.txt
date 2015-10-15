DESCRIPTION
===========
Paybox Service (http://www1.paybox.com) integration for the Drupal Commerce
payment and checkout system.

INSTALLATION INSTRUCTIONS FOR COMMERCE PAYBOX
=============================================

First you should make sure you have an Paybox Merchant account, and are ready
to configure it:

Installing & configuring the Paybox payment method in Drupal Commerce
---------------------------------------------------------------------
 - Install the Paxbox module.
 - Paybox payment functions thanks to a CGI module. Download it in the download
   section (http://www1.paybox.com/telechargement_focus.aspx?cat=3) of the
   paybox site.
 - Navigate to the admin section of the Paybox module (Store > Configuration >
   Paybox System) and set the path to the CGI module.
 - Enable the Paybox payment rule (Store > Configuration > Payment methods).
 - Edit the Paybox payment rule "Action settings" with your paybox informations
   (be sure to uncheck "Use Paybox test plateform" on production).

Notes
-----
IMPORTANT : Paybox module require the Paybox servers to be able to contact your
Drupal site for orders validation. During the test phase, the Paybox sandbox
servers WILL NOT confirm the validation of the order process.

To achieve more tests you'll have to allow your IP (usally 127.0.1.1 or
127.0.0.1) to validate orders (Store > Configuration > Paybox System),
and the call the URL yourself.

AUTHOR
======
Samuel Hurel (http://drupal.org/user/789980) of IDEIA (http://ideia.fr).
The author can be contacted for paid customizations of this module as well as
Drupal consulting.
