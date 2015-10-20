Description
===========
Implements Credit card payment services on Hungarian OTP bank
(https://www.otpbank.hu) interface for the Drupal Commerce payment
and checkout system.

Installing otpwebshop Library
=============================
Download otpwebshop Webservice 4.0 package:
https://www.otpbank.hu/static/portal/sw/file_nosync/Webshop_4.0.zip
Downloadable from:
https://www.otpbank.hu/portal/hu/Kartyaelfogadas
- extract Webshop_4.0.zip file and copy Webshop_4.0/kliensek/php/otpwebshop
 folder to your libraries (e.g. sites/all/libraries/otpwebshop)
- WebShopService.php file locations should be:
 sites/all/libraries/otpwebshop/lib/iqsys/otpwebshop/WebShopService.php
- delete demo and sample files, folders (simpleshop, web_demo)
There are many deprecated static function calls for non static functions. You
 can apply patch from: https://github.com/szatom/OTP_Webshop_4.0

Installing & configuring Commerce OTP payment method
====================================================
- Enable the module (Go to admin/modules and search for OTP Payment).
- Go to Administration > Store > Configuration > Payment Methods
- Under "Disabled payment method rules", find the "OTP credit card payment"
 payment method and click the 'enable' link
- Once "OTP credit card payment" appears in the "Enabled payment method rules",
 click on it's name to configure
- In the table "Actions", find "Enable payment method: OTP credit card payment"
 and click the link
- Under "Payment settings", you can configure the module:
  * Display label: you can configure label displayed on checkout page
  * Shop ID: your OTP PosID received from bank
  * OTP interface language
  * Private keyfile path
  * Transaction log directory
  * Transaction log directory - on success
  * Transaction log directory - on fail

Cron job
========
- cron job checks (ask and update status) not processed transactions from bank
- not processed transactions can be caused by closing web browser after
 success payment, before redirection back to webshop page

Logger file configuration
=========================
- edit otwebshop/config/otp_webhsop_client.conf
- set log file: log4php.appender.WebShopClient.File
- set logging level: log4php.logger.WebShopClient.Threshold
- do not change: log4php.rootLogger

Documentation
=============
- You can add more "OTP credit card payment" - useful for test/live together
- Supportable currencies (by OTP bank): HUF, EUR, USD
- Supportable languages (OTP bank interface): hu, en, de
- Currency code is given from order
- For test environment, OTP give Shop ID with '#' prefix
- There is possibility for using drush command for triggering cron jobs
- You can disable OTP cron jobs by setting drupal variable
 'commerce_otp_cron_enabled' to FALSE
- You can list commerce transaction revisions (for OTP payment) at
 admin/commerce/commerce_otp_transactions_list

Testing
=======
- You can use #02299991 shop ID and #02299991.privKey.pem keyfile for testing
 (Webshop_4.0.zip/teszt_kulcsok/#02299991.privKey.pem)
- You can find test cards data in tesztkartyak.txt file
 (Webshop_4.0.zip/doc/tesztkartyak.txt)

Author
======
Tamas Szanyi (https://www.drupal.org/user/389677) of Brainsum
(http://brainsum.com)
