
-- SUMMARY --

This module enables the QuickBooks Merchant Service payment method for Drupal
Commerce. The QuickBooks PHP DevKit module is used to integrate with the
QuickBooks Merchant Service API. The Encryption module is used for protecting
sensitive QuickBooks Merchant Service API information.

For a full description of the module, visit the project page:
  http://drupal.org/project/commerce_qbms

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/commerce_qbms


-- REQUIREMENTS --

* Drupal Commerce
  https://drupal.org/project/commerce

* Encryption (2.x)
  https://drupal.org/project/encrypt

* QuickBooks PHP DevKit
  https://drupal.org/project/quickbooks_php_devkit

* A QuickBooks Merchant Service account is required and Process with Web Store
  must be enabled. The QuickBooks Merchant Service account will also need to be
  connected to the App ID for this module. The following steps will assist in
  fulfilling these requirements and must be completed before this module can be
  used. Be sure to use the correct production or test URLs based on usage.

  1 Sign up for a QuickBooks Merchant Service account if you don't have one.
    - Production: https://merchant.intuit.com/signup/start.wsp
    - Test: https://merchant.ptcfe.intuit.com/signup/start.wsp

  2 Ensure Payments for Web Store is enabled.
    1 Sign into your QuickBooks Merchant Service account.
      - Production: https://merchantcenter.intuit.com
      - Test: https://merchantcenter.ptcfe.intuit.com

    2 Click the 'Account > Processing Information' link in the main menu.

    3 Ensure 'Processing with Web Store' is 'ON' and submit the changes if any.

  3 Connect your QuickBooks Merchant Service account to the Intuit App ID for
    this module.
    - Production: https://merchantaccount.quickbooks.com/j/sdkconnection?appid=808620730&appdata=mydata
    - Test: https://merchantaccount.ptc.quickbooks.com/j/sdkconnection?appid=1005676477&appdata=mydata

    This step will produce a Connection Ticket that is used to configure this
    module. This Connection Ticket in combination with the App Login for this
    module is used for authenticating transactions against the QuickBooks
    Merchant Service account, so be sure to keep it confidential. If a
    Connection Ticket is compromised, it can be deleted and a new one created by
    revisiting the provided URLs.


-- INSTALLATION --

* Install as usual, see
  http://drupal.org/documentation/install/modules-themes/modules-7


-- CONFIGURATION --

1 Visit admin/commerce/config/payment-methods and click the 'enable' operation
  for the 'Commerce QBMS - Credit Card' payment method.

2 Click the 'edit' operation for the 'Commerce QBMS - Credit Card' payment
  method.

3 Click the 'edit' operation for the 'Enable payment method: Commerce QBMS'
  action.

4 Enter the Connection Ticket obtained when fulfilling the requirements of this
  module. Complete the rest of the configuration form and click 'save'.


-- CONTACT --

Current maintainers:
* Jon Antoine (jantoine) - https://drupal.org/user/192192

This project has been sponsored by:
* Showers Pass
  Technically engineered cycling gear for racers, commuters, messengers and
  everyday cycling enthusiasts. Visit http://showerspass.com for more
  information.
