eps Payment Standard - Online Bank Transfer for Drupal Commerce
----------------------------------------------------------------

This project integrates eps payment standard into the Drupal Commerce payment and checkout systems. The eps
Online-Überweisung is an off-site payment service of Austrian banks. It is based on the technical eps e-payment standard
which is an open and standardized XML interface between online-shops and banks to initiate irrevocable online payments.
Extensive documentation can be found here, current version v2.5:

- http://www.eps.or.at/de/eps-payment-standard.html (German)
- http://www.eps.or.at/en/eps-payment-standard.html

The payment process is explained in brief: after choosing the EPS payment method in the web shop the customer is
redirected to a central bank selection. Hence, the customer bank has to be chosen which again redirects to the online
banking system. Then, the payment is conducted as any other online bank transaction including usage of TANs by the
customer. Finally, the customer is redirected to the web shop showing success or possible failure. No confidential data
is collected, the scheme operator functions as an intermediary between the parties building on top of existing systems.

eps/giropay Inter-Scheme Standards is supported. Therefore, payments can also be processed via German bank accounts.


Installation and Configuration
-------------------------------

As a prerequisite you need to have a contract with an acquirer, i. e. an Austrian bank, see
http://www.eps.or.at/de/eps-payment-standard/eps-ansprechpartner.html

- Enable the module :)
- Provide the following beneficiary credentials in the payment method settings (admin/commerce/config/payment-methods):
  - User Id
  - PIN
  - IBAN (International Bank Account Number)
  - BIC (Business Identifier Code)
  - Beneficiary name and address
- Activate EPS payment method

That should be it and you should be ready to go.