
-- SUMMARY --

This module provides a credit cart payment gateway for the uc_credit module
through Quickbooks Merchant Services (QBMS) for Web Store.  This is NOT full
Quickbook integration.  This module simply processes credit cards through QMBS
for a merchant with a QBMS account.

This module uses and is dependent on the Intuit PHP Toolkit by Keith Palmer 
(http://www.ConsoliBYTE.com).  The Framework can be download from Intuit:
  https://code.intuit.com/sf/frs/do/viewSummary/projects.php_devkit/frs

For a full description of the module, visit the project page:
  http://drupal.org/project/uc_qbms

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/uc_qbms


-- REQUIREMENTS --

Module Dependancies: uc_payments & uc_credit

This module requires a Quickbooks Merchant Services for Web Store account.
If you already have QB Merchant Services then you need to login and make sure
you have turned on web store payments as follows:
  Step 1: Login (https://merchantcenter.intuit.com/secureweb)
  Step 2: Under "Account" menu select "Edit Processing Information"
  Step 3: Select "ON" next to "Process with Web Store" and Submit Changes
        (please leave the other settings as you found them)
  Step 4: Agree to all the Intuit stuff, extra charges, yadda yadda..
  
Now you have the proper merchant services to use this module.


-- INSTALLATION --

Download the Intuit Quickbooks PHP DevKit:
  https://code.intuit.com/sf/frs/do/viewSummary/projects.php_devkit/frs
Unzip and rename the top directory to 'quickbooks-php-devkit'
FTP the entire package to sites/all/libraries
The main file in the DevKit should now have the following path:
  sites/all/libraries/quickbooks-php-devkit/QuickBooks.php
with the rest of the package located as:
  sites/all/libraries/quickbooks-php-devkit/dev
  sites/all/libraries/quickbooks-php-devkit/docs
  sites/all/libraries/quickbooks-php-devkit/QuickBooks
  sites/all/libraries/quickbooks-php-devkit/src
  sites/all/libraries/quickbooks-php-devkit/tests

Now install module in the usual Drupal manner.


-- CONFIGURATION --

To process Credit Cards with this module in a production environment you must 
first meet the -- Requirements -- (see above) and completed the -- Installation --
(including the Quickbooks PHP DevKit)

Next you need to get a Connection Ticket from Intuit by connecting your 
Quickbooks Merchant Services (QBMS) account to the Intuit AppID for this module.

Step 1: Paste this url into your browser:
  https://merchantaccount.quickbooks.com/j/sdkconnection?appid=179399124&appdata=mydata
Step 2: Login using your QBMS account name and password.
Step 3: Approve the connection with "QBMS CC Gateway for Drupal/Ubercart"
Step 4: DO NOT turn on any additional passwords or security if it asks.
Step 5: Copy the Connection Ticket Intuit and paste it on the module configuration page:
  Administration > Store > Configuration > Payment methods > settings 
  i.e. (DRUPAL_ROOT)/admin/store/settings/payment/method/credit
Step 6: Confirm that your Quickbooks PHP DevKit path is set correctly and that
  Test Evironment is DISABLED.


-- CONFIGURE TEST ENVIRONMENT --

This process is slightly more complicated because you need to first create a
test merchant account:
  https://merchantaccount.ptc.quickbooks.com/j/mas/signup?nonQBmerchant=true
It looks Official, but it's just a dummy merchant account for testing.

There is also a special AppID for testing so you will now need to connect your
dummy merchant account with this separate AppID by going here:
 https://merchantaccount.ptc.quickbooks.com/j/sdkconnection?appid=180074823&appdata=mydata

Continue through the --Configuration -- steps above starting with Step2.
If you've already received your production Connection Ticket be sure and save it
for when you switch back to production.

Once you have both Connection Tickets...
  One with your real merchanat account connected to the live AppID link.
  Other with your dummy merchant account connected to the test AppID link.
You can now just switch back and forth by "Enabling" or "Disabling" the Test
Environment and entering the corresponding Connection Ticket.

-- How To Retrieve a Lost Connection Ticket or Generate a New One --
Go to:

  Live Connection Ticket:
    https://merchantaccount.quickbooks.com/j/sdkconnection?appid=179399124&appdata=mydata
  Test Connection Ticket:
    https://merchantaccount.ptc.quickbooks.com/j/sdkconnection?appid=180074823&appdata=mydata

Login with the corresponding merchant account (live or test)
Then select the "QBMS CC Gateway for Drupal/Ubercart"
  To show your existing Connection ticket: Click "Use this Connection"
  To re-issue a new ticket (in case it has been compromised): Click "Create New Connection" 


-- CONTACT --

Current maintainers:
  Art Williams (artis) - http://drupal.org/user/77599
  Jon Antoine (AntoineSolutions) - http://drupal.org/user/192192

Based on uc_authorizenet.
