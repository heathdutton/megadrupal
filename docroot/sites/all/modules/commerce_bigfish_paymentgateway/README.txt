BIG FISH Hungarian paymentgateway for Drupal Commerce.

Implements the next gateways:
- CIB bank
- Escalion
- FHB Bank
- KHB (K&H Bank)
- MPP (MobilPay)
- MPP2 (MasterCard Mobile)
- OTP
- PayPal
- PayU
- SMS
- UniCredit Bank
- Wirecard QPAY

Dependency: Commerce payment
            Libraries

Try demo: http://tesztshop.gfxpro.hu/

Install module as usualy.
(http://drupal.org/documentation/install/modules-themes).

- Dowload sdk from https://www.paymentgateway.hu/static/downloads/sdk/php.zip
- Unpack files (config.php and paymentgateway.php) from php/lib directory to
  sites/all/libraries/bigfish_paymentgateway_sdk folder.
- Download Commerce BIG FISH Paymentgateway module from modul site.
- Unpack to sites/all/modules.
- Go to admin/modules site.
- Enable Commerce BIG FISH Payment Gateway module.
- Go to admin/commerce/config/bigfish-paymentgateway admin site and configure
  shop. Include data what get from BIG FISH paymentgateway. 
  If you want just testing module do not change default values but save data.
- Go to admin/commerce/config/payment-methods to setup which paymentgateway want
  to use.

Test data find in downloaded SDK php folder. File name: Test_Data.pdf

This module sponsored: https://www.paymentgateway.hu
