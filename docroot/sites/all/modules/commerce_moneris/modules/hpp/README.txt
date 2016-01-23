
INSTALLATION
============

1. Enable the Commerce Moneris API module.

2. Enable and configure the Moneris HPP payment method.

   Enable the Moneris HPP payment method on your store's Payment methods page
   (admin/commerce/config/payment-methods).

   Next go to the payment method configuration page, select required gateway
   and environment, enter relevant Store ID and HPP Key values (you can find
   test ones for both gateways below in "Test store_ids and hpp_keys" section),
   and configure other options as required.

   Note that Refund/Reverse/Capture transactions are done using Moneris API
   payment solution (they are not supported by HPP). If you are planning to use
   them, you need to have Moneris API module enabled and API payment method
   configured (please refer to API README.txt file for instructions how to do
   it). One important thing here is that both payment method must use the same
   Moneris store, otherwise these additional transaction types will not work.

3. Configure your Hosted Paypage in Merchant Resource Centre.

   For details on accessing eSELECTplus Merchant Resource Centre please see
   "Accessing Merchant Resource Centre" and "Test store_ids and login details"
   sections.

   Once you have successfully logged in, click on the "ADMIN" menu item on the
   left and then in the submenu that appears click on "HOSTED CONFIG".

3.1. Main configuration.

     To create a new Hosted Pay Page configuration click on "Generate a New
     Configuration". You will be assigned a Hosted Pay Page ID (hpp_id, Store ID),
     which is the identifier for this unique configuration. You will also be
     assigned a Hosted Pay Page Token (hpp_key).

     You then need to copy both these values into relevant fields in your payment
     method configuration.

     In "Payment Methods" section make sure you have "Credit Card" enabled.

     In "Response Method" section make sure you have either "Sent to your server
     as a POST" or "eSELECTplus plus will generate a receipt" option selected.

     Also, provide the "Response URL" (as displayed on payment method settings
     page, something like http://your-site.com/commerce-moneris-hpp/callback).

     Save changes.

3.2. Configure Appearance.

     In "Hosted Paypage Input Fields" section make sure both "Display CVD input"
     and "Display AVS input" options are enabled.

3.3. Configure Response Fields.

     In "Response/Receipt Field Configuration" enable "Return VBV/SC result code
     (cavv_result_code)" option.

     In "Asyncronous Transaction Response" section enable "Perform asyncronous
     data post" option and provide "Async Response URL" value, again as displayed
     on payment method settings page (something like
     http://your-site.com/commerce-moneris-hpp/validate).

     Please note that receiving this asyncronous response is required for
     the payment transaction to be marked as completed. Without receiving such
     response the payment transaction will keep its pending status.

     Also, asyncronous transaction responses are always returned over port 443
     (even in the test environment). Make sure this port is open on your
     firewall, and properly redirected to your testing server if required.

     Finally, in the production environment response url must be secure (HTTPS).
     Self signed certificates will work. HTTP addresses will not work.




TEST STORE_IDS AND LOGIN DETAILS
================================

Before you can send a transaction to the Hosted Paypage you will need to
configure several settings through the eSELECTplus Merchant Resource Centre (MRC).
To log in to the MRC test environment go to the relevant URL (see the "Accessing
Merchant Resource Centre" section) and use one of the following login IDs:


* US gateway

  | store_id      | Username  | Password  |
  |---------------|-----------|-----------|
  | monusqa002*** | demouser  | abc1234   |
  | monusqa003    | demouser  | abc1234   |
  | monusqa004    | demouser  | abc1234   |
  | monusqa005    | demouser  | abc1234   |
  | monusqa006    | demouser  | abc1234   |
  | monusqa024*   | demouser  | abc1234   |
  | monusqa025**  | demouser  | abc1234   |

  * test store "monusqa024" is intended for testing ACH transactions only
  ** test store "monusqa025" is intended for testing both ACH and Credit Card transactions
  *** test store ‘monusqa002’ is intended for testing Pinless Debit, Gift and Loyalty transactions


* CA gateway

  | store_id  | Username  | Password  |
  |-----------|-----------|-----------|
  | store1    | demouser  | password  |
  | store2    | demouser  | password  |
  | store3    | demouser  | password  |
  | store5*   | demouser  | password  |




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

Extract from Moneris HPP Merchant Integration Guide:

The test environment has been designed to replicate our production environment
as closely as possible. One major difference is that we are unable to send test
transactions onto the production authorization network and thus issuer responses
are simulated. Additionally, the requirement to emulate approval, decline and
error situations dictates that we use certain transaction variables to initiate
various response and error situations.

The test environment will approve and decline transactions based on the penny
value of the amount field.
For example, a transaction made for the amount of $9.00 or $1.00 will approve
since the .00 penny value is set to approve in the test environment.
Transactions in the test environment should not exceed $10.00. This limit does
not exist in the production environment.
For a list of all current test environment responses for various penny values,
please see the Test Environment Penny Response table as well as the Test
Environment eFraud Response table, available for download at
https://developer.moneris.com




ACCESSING MERCHANT RESOURCE CENTRE
==================================

Access the Merchant Resource Center using the links provided below
and username/password pair provided above.


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

  - For technical support:
    Phone: 1-866-696-0488

  - For integration support:
    Phone: 1-866-562-4354
    Email: eproducts@moneris.com


* CA gateway

  - For Technical Support:
    Phone: 1-866-319-7450 (Technical Difficulties)
    Email: eselectplus@moneris.com

  - For Integration Support:
    Phone: 1-866-562-4354
    Email: eproducts@moneris.com
