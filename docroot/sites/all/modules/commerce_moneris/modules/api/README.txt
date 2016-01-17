

INSTALLATION
============

1. Enable the Commerce Moneris API module.

2. Download the improved Moneris API libraries.

   Original Moneris API libraries that can be downloaded from the Developer
   Portal do not support several features required by this module (like working
   with both test and prod servers, support for cURL CA root certificates,
   fetching cURL responses). Therefore they have been slightly improved and
   are now available on GitHub, where they should be downloaded from.

   US library, based on Moneris API US library v3.2.0 is available at:
   https://github.com/maciejzgadzaj/moneris-api-us

   CA library, based on Moneris API CA library v2.5.6 is available at:
   https://github.com/maciejzgadzaj/moneris-api-ca

   They could be downloaded using the following drush commands:

   drush dl-moneris-api-us
   drush dl-moneris-api-ca

   Alternatively, you can download them manually into:

   * if your site has Libraries module enabled:

     sites/all/libraries/moneris-api-us
     sites/all/libraries/moneris-api-ca

   * if your site doesn't have Libraries module enabled:

     <moneris_module_directory>/modules/api/includes/moneris-api-us
     <moneris_module_directory>/modules/api/includes/moneris-api-ca

   The only file that is required in both cases is mpgClasses.php - all other
   files (especially the examples/ subdirectory) could be safely deleted.

3. Enable and configure the Moneris API payment method.

   Enable the Moneris API payment method on your store's Payment methods page
   (admin/commerce/config/payment-methods).

   Next go to the payment method configuration page, select required gateway
   and environment, enter relevant Store ID and API Token values (you can find
   test ones for both gateways below in "Test store_ids and api_tokens"
   section), and configure other options as required.

   If you want to use SSL (and you want to do it), you need to download
   the cacert.pem and upload it to your server (if it is not already there).
   On the configuration page, you have to set the path to that file, either
   absolute (if you downloaded it outside of your Drupal installation directory)
   or relative to your Drupal installation.



TEST STORE_IDS AND API_TOKENS
=============================

When using the APIs in the test environment you will need to use test
store_id and api_token. These are different than your production IDs.
The IDs that you can use in the test environment are in the table below.


* US gateway

  | store_id      | api_token | Username  | Password  |
  |---------------|-----------|-----------|-----------|
  | monusqa002*** | qatoken   | demouser  | abc1234   |
  | monusqa003    | qatoken   | demouser  | abc1234   |
  | monusqa004    | qatoken   | demouser  | abc1234   |
  | monusqa005    | qatoken   | demouser  | abc1234   |
  | monusqa006    | qatoken   | demouser  | abc1234   |
  | monusqa024*   | qatoken   | demouser  | abc1234   |
  | monusqa025**  | qatoken   | demouser  | abc1234   |

  * test store "monusqa024" is intended for testing ACH transactions only
  ** test store "monusqa025" is intended for testing both ACH and Credit Card transactions
  *** test store "monusqa002" is intended for testing the Pinless Debit transactions


* CA gateway

  | store_id  | api_token | Username  | Password  |
  |-----------|-----------|-----------|-----------|
  | store1    | yesguy    | demouser  | password  |
  | store2    | yesguy    | demouser  | password  |
  | store3    | yesguy    | demouser  | password  |
  | store5*   | yesguy    | demouser  | password  |
  | moneris** | hurgle    | demouser  | password  |

  * "store5" is for testing eFraud (AVS & CVD)
  ** "moneris" is for testing VBV



TEST CARD NUMBERS
=================

When testing you may use the following test card numbers with any future
expiry date.


* US gateway

  | Card plan     | Card number       |
  |---------------|-------------------|
  | MasterCard    | 5454545454545454  |
  | Visa          | 4242424242424242  |
  |               | 4005554444444403  |
  | Visa Debit    | 4506441111111115  |
  | Amex          | 373599005095005   |
  | Discover      | 6011000000000012  |
  | Pinless Debit | 4496270000164824  |


* CA gateway

  | Card plan     | Card number       |
  |---------------|-------------------|
  | MasterCard    | 5454545454545454  |
  | Visa          | 4242424242424242  |
  | Visa Debit    | 4506441111111115  |
  | Amex          | 373599005095005   |
  | JCB           | 3566007770015365  |
  | Diners        | 36462462742008    |
  | Discover      | 6011000000000012  |




TESTING APPROVAL, DECLINE AND ERROR SITUATIONS
==============================================

Extract from Moneris API Merchant Integration Guide:

The test environment has been designed to replicate our production
environment as closely as possible. One major difference is that we are
unable to send test transactions onto the production authorization network
and thus Issuer responses are simulated. Additionally, the requirement
to emulate approval, decline and error situations dictates that we use
certain transaction variables to initiate various response and error situations.

The test environment will approve and decline credit card transactions
based on the penny value of the amount field.

For example, a credit card transaction made for the amount of $9.00 or $1.00
will approve since the .00 penny value is set to approve in the test environment.

Transactions in the test environment should not exceed $11.00. This limit
does not exist in the production environment.

For a list of all current test environment responses for various penny values,
please see the "Test Environment Penny Value Response Table" table as well as
the "Test Environment eFraud (CVD & AVS) Result Codes", available on the
Developer Portal in "Other Useful Stuff" section.



TESTING EFRAUD (AVS AND CVD)
============================

When testing eFraud (AVS and CVD) you must only use the Visa
test card numbers (4242424242424242 or 4005554444444403 - both gateways)
and the amounts described in the "Simulator eFraud Response Codes" document
available on the Developer Portal in "Other Useful Stuff" section.



ACCESSING DEVELOPER PORTAL
==========================

Access the Moneris Solutions Developer Portal at https://developer.moneris.com/

Here you will find descriptions and documentation for all Moneris payment
solutions, including integration guides, code samples, libraries etc.



ACCESSING MERCHANT RESOURCE CENTER
==================================

Access the Merchant Resource Center using the links provided below
and username/password pair provided above.

Once logged in, go to Reports -> Transactions and perform a search.
You will see a listing with all transactions
(yours and other users that are testing Moneris).


* US gateway

  To access the Merchant Resource Center in the test environment go to
  https://esplusqa.moneris.com/usmpg

  To access the Merchant Resource Center in the production environment go to
  https://esplus.moneris.com/usmpg


* CA gateway

  To access the Merchant Resource Center in the test environment go to
  https://esqa.moneris.com/mpg

  To access the Merchant Resource Center in the production environment go to
  https://www3.moneris.com/mpg



GETTING HELP
============

If you require technical assistance while integrating your store,
please contact the eSELECTplus Support Team.


* US gateway

  - For financial support:
    Phone: 1-800-471-9511
    Email: supportinfo@moneris.com

  - For technical and integration support:
    Phone: 1-866-696-0488
    Email: eselectplus@moneris.com


* CA gateway

  - For Technical Support:
    Phone: 1-866-319-7450 (Technical Difficulties)
    Email: eselectplus@moneris.com

  - For Integration Support:
    Phone: 1-866-562-4354
    Email: eproducts@moneris.com
