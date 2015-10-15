"Drupal Commerce Payment Transaction Fields" module
Make the Commerce Payment Transactions entities fieldable.

This module do not work by itself, the Payment method(s) module
should be aware of it and use it.

How it works / usage
 - New "fieldable" property should be used and set to TRUE
 in the hook_commerce_payment_method_info() payment method implementation.
 - Fields widgets available on payment checkout and terminal.
 These implementations will pass the transaction with fields object to payment method submit callback.
 So, saving fields data will not work if there is no implementation for the payment method callback.
 Code example : http://cgit.drupalcode.org/commerce_cop/tree/commerce_cop.module#n305
 from Commerce Custom Offline Payments module.
 - Update payment page (form) available on terminal.
 - The usage of the fields should be implemented in the payment method module,
 for example for "Confirm payment" page form, use field_attach_form() to add the fields widget.
 Code example : http://cgit.drupalcode.org/commerce_cop/tree/commerce_cop.module#n98
 from Commerce Custom Offline Payments module.
 - To get the payment transaction fields data use Views.

Use cases
Commerce Custom Offline Payments (https://drupal.org/project/commerce_cop) uses this module capabilities,
to build extra data for its payments.
