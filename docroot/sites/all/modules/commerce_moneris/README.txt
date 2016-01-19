
DESCRIPTION
===========

Moneris (http://www.moneris.com) integration modules
for the Drupal Commerce payment and checkout system.

Commerce Moneris comes with 2 submodules, providing integration with
2 independent Moneris payment solutions:

1) API Integration Method

   On-site payment method provided by Commerce Moneris API module.

   * support for Purchase/Refund/PreAuth/Capture/Reverse/ReAuth transactions
   * support for eFraud (CVD and AVS verification)
   * support for cURL CA Root Certificate for SSL connection
   * support for currency conversion
   * independent configurations for US/CA gateways and test/prod environments
   * additional request/response debugging

   Detailed payment solution description is available
   on the Moneris Solutions Developer Portal:

   - US: https://developer.moneris.com/Downloads/US/APIs.aspx
   - Canada: https://developer.moneris.com/Downloads/Canada/API.aspx

2) Hosted Pay Page Method

   Off-site payment method provided by Commerce Moneris HPP module.

   * support for Purchase/PreAuth transaction (configured in Hosted Paypage
     configuration in Merchant Resource Centre)
   * support for Refund/Capture/Reverse/ReAuth transactions
     through Moneris API payment method
   * support for transaction verification through Asyncronous Transaction
     Response
   * support for currency conversion
   * independent configurations for US/CA gateways and test/prod environments
   * additional request/response debugging

   Detailed payment solution description is available
   on the Moneris Solutions Developer Portal:

   - US: https://developer.moneris.com/Downloads/US/Hosted%20Pay%20Page.aspx
   - Canada: https://developer.moneris.com/Downloads/Canada/Hosted%20Pay%20Page.aspx



GATEWAY CURRENCIES
==================

Both gateways support only one currency native to their relevant country - USD
for US gateway and CAD in case of CA gateway. There is no way to use any other
currency when sending a transaction request, as the Moneris API does not define
any currency-related request fields. Therefore, if a transaction is being made
and the payment is requested in any other currency, the module will return
an error before even trying to send the payment request to the gateway.



INSTALLATION
============

Please refer to the README.txt file available in relevant module directory
on how to install and configure each module and provided payment method.



UPGRADING FROM 1.X
==================

Please note there is no upgrade path from version 1.x of the module.

Generally it is advised to disable and uninstall previous version of the
module completely (including deleting the remaining payment methods after
the module has been uninstalled) before downloading and enabling the new
version.

The most important changes introduced in version 2.x compared to previous
1.x include:

  * on-site Moneris payment method ID has been changed to better reflect the
    solution name - previously it has been 'commerce_moneris', now it is
    'commerce_moneris_api'
  * payment method settings use different structure compared to the previous
    module version - they will need to be configured again, as the old settings
    will not work
  * the following hooks have been changed:
    - hook_commerce_moneris_txnarray_alter() - replaced by
      hook_commerce_moneris_api_data_alter(), also the transaction array being
      passed to this hook in the first parameter does not include some data
      anymore (for example credit card details)
    - hook_commerce_moneris_hpp_settings_alter() - removed completely
    - hook_commerce_moneris_hpp_data_alter() - added $order as a second
      parameter
