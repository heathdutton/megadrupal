About this module and WinBIZ
----------------------------
This module provides ability to export Drupal Commerce order data to the external 
bookkeeping / ERP / CRM application WinBIZ. WinBIZ is a third party software developed
by La Gestion Electronique SA. See also http://www.winbiz.ch .
This module aims to provide an easy to use out-of-the box integration, which in combination
with WinBIZ covers all relevant administrative/bookkeeping tasks. For more information
about this module please visit the project page on drupal.org. (@todo provide the link
once the module is not a sandbox project anymore).


Features
--------
- Export order data of Drupal Commerce
- Configure the value of some of the most important export fields on the settings page
- Choose whether you want to use a custom tax rate or you want to use the tax rate of Drupal
  Commerce on the settings page
- Developers can alter the exported values before the data is sent to WinBIZ
- Configure payment method mapping and if a certain payment method is threatened as online (instant) payment in WB


Installation and Setup in WinBIZ
-------------------------------
- Enable the module
- Go to admin/commerce/config/winbiz and copy value of the "Authentication token" field
- In your Website configuration in the WinBIZ appication enter the URL of your site and add 
  /winbiz/<authentication token> i.e. http://www.example.com/winbiz/<authenticationtoken> 
  (For more information on how to set up a website within WinBIZ please refer to the the WinBIZ manual.)


Settings
--------
- The settings page is located at admin/commerce/config/winbiz.
- All possible settings are described on this page.
- For taxes it is either possible to enter a custom tax rate or to use the rate provided by
  the Drupal Commerce tax module. Also you can configure wether tax calculation in WinBIZ is
  done inclusive or exclusive.
- If you change the authentication token, make sure you also change your URL within the WinBIZ 
  configuration.


How to deal with online payments
--------------------------------
If your webshop features online payment methods like credit card or paypal payment,
you will have to activate those payment methods on the payment settings page.
In order to do such go to admin/commerce/config/winbiz/payments, enable your online
payment method(s) and optionally add the payment method code as used in WinBIZ.
It is recommended, to create the payment method first in WinBIZ and add a payment method code.
This code used in WinBIZ will have to be set on the corresponding payment method row on the
payment settings page. The payment of orders imported with a payment method code that matches a
corresponding code in WinBIZ are then connected to this payment method in WinBIZ.  


Notes
-----
If you have to export additional data to WinBIZ, for example let's say your webshop allows
to users to enter a different delivery address, you will have to implement 
hook_commerce_winbiz_export_row(). All collected data is available and can be overridden
or additional data can be added there. For more information about at which index you are
supposed to add/override your data please refer to the WinBIZ manual (around page nr. 752).
Every field nr. in the manual has a corresponding index in the array that's available in the hook.
