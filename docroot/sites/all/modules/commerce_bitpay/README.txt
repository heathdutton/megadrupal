Commerce BitPay
---------------

This module provides BitPay payment option for Drupal Commerce.

For a full description of the module, visit the project page:
  https://drupal.org/project/commerce_bitpay

To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/commerce_bitpay


** REQUIREMENTS **

Drupal Commerce
  http://www.drupalcommerce.org/
  https://drupal.org/project/commerce

BitPay php-client
  https://github.com/bitpay/php-client

** CONFIGURATION **

Configure BitPay options in admin/commerce/config/payment-methods
by editing the BitPay payment method. The configuration
options are part of the reaction rule settings.

    - BitPay API key

        BitPay API access key. Something like
        aW4x5kLr4mer4fovDJLGTMXSATkf81DLKcm349ajd12

        Login to your merchant account, then create a key at:
        https://bitpay.com/api-keys

    - Redirect URL

        This is the URL for a return link that is displayed on the receipt, to
        return the shopper back to your website after a successful purchase.
        Leave blank to use default commerce "checkout complete" -page.

    - Notification email

        This is the email where invoice update notifications should be sent.
        Leave blank to use default settings defined in your BitPay account.

    - Transaction speed

        How quickly BitPay will report a successful payment based on payment
        confirmations.

    - Full notifications

        Changes the verbosity of status changes. Full notifications will report
        updates back to Commmerce more frequently than when disabled.

    - Checkout redirect mode

        Toggles the payment process to use either a hosted checkout page or
        embed an invoice iframe in the Commerce checkout process.

Copy the BitPay php-client library to:

 * sites/all/libraries/php-client OR
 * sites/<domain>/libraries/php-client

** CONTACT **

Current maintainers:

* David Norman (deekayen)
  https://drupal.org/user/972
  1Drupa1oXCheXrbZbo6LQEu15ZNbFRGjXF
