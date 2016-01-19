-- SUMMARY --

This module integrates the payment method SOFORT Banking from SOFORT AG into
Drupal Commerce. The integration follows the API available from
https://www.sofort.com/integrationCenter-eng-DE/integration/API-SDK and has been
guided by the documentation https://www.sofort.com/integrationCenter-eng-DE/content/view/full/2513

-- REQUIREMENTS --

* Drupal 7
* Commerce 1
  - commerce_ui
  - commerce_payment
  - commerce_order
  - commerce_customer

-- INSTALLATION --

Install this module following instructions from https://drupal.org/documentation/install/modules-themes/modules-7

-- CONFIGURATION --

* Go to Administration > Store settings > Advanced store settings > Payment methods
  admin/commerce/config/payment-methods
* Click on "Edit" in the row of "SOFORT Banking"
* Click on "Edit" in the row of "SOFORT Banking" in the 'Actions' block
* Include your configuration key of your gateway project from your account
  at www.sofort.com
* Option: Enhance the section "Extended settings" and adjust the settings
  according to your needs
* Finally click "Save" and you're ready to go receiving payments
  from your customers via SOFORT Banking

-- SUPPORT --

Support for this module is provided in the issue queue at https://drupal.org/project/issues/commerce_sofortbanking

-- CONTACT --

Maintainer:
* Jürgen Haas (jurgenhaas) - https://drupal.org/user/168924

This project has been sponsored by:
* PARAGON Executive Services GmbH
  Providing IT services as individual as the requirements.
  Find out more from http://www.paragon-es.de
* SOFORT AG
  The Payment network
  Find out more from https://www.sofort.com
