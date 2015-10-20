ABOUT
-----

The module commerce_futurepay adds a new payment method to Drupal commerce
allowing customers to pay their orders using the FuturePay payment gateway.

REQUIREMENTS
------------

Required : Drupal Commerce


INSTALLATION
------------

1. Download and enable the module.

2. Configure the FuturePay payment gateway:

   Go to Payment methods page: admin/commerce/config/payment-methods
   Click on edit link from "FuturePay Payment Gateway" payment method. Then,
   Edit the rule action "Enable payment method: FuturePay Payment Gateway" in
   order to access to the payment gateway settings.

   The only setting you need to fulfill is the Merchant API Key which is
   the credential given by FuturePay. If you don't fulfill this setting, the
   FuturePay payment method will not be displayed to your customers.

3. Business Rules:

   A Customer must select United States as their country selected value = US.

   Only zip codes that are not a PO box or Military Address will be accepted.

   A Customer can only signup and purchase once in FuturePay subsequent to
   customer account activation the Customer may login and purchase.

4. Payment:

   The payment process is shown to customers only if business rules are valid in
   the current context.

5. Refund:

   Administrator can refund an order payed with FuturePay from the payment tab
   of an order. Once you reach that page, a button "Refund with Futurepay"
   allows the administrator to reimburse partially/fully the order amount
   through a refund terminal.

5. Enjoy !

   Now you should be able to expose the FuturePay payment method.