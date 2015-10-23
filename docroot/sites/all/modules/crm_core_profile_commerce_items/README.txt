CRM Core Profile Commerce Items for Drupal 7.x
-----------------------------------------------
CRM Core Profile Commerce Items (called Commerce Items) is an entity
handler for CRM Core Profile. Administrators can use it to create forms
that collect online payments.

Commerce Items directly integrates with Drupal Commerce and allows
CRM Core Profile to add various items related to payment processing within
profile forms.

Requirements
------------
* CRM Core
* CRM Core Profile
* CRM Core Contact
* Commerce
* Product
* Cart
* Checkout
* Payment
* Price

Before attaching any Commerce Items components to a profile, do this:

1) Ensure there is at least one product defined in your site.

2) Ensure there is at least one payment method activated in your site.

Installation
------------
1) Download CRM Core Profile Commerce Items to the modules directory of
   your website.

2) Enable the module from the admin/build/modules page.

3) Commerce Items will appear as an entity option when creating / editing
   profiles. Commerce fields can now be attached to any profile.

Notes on Usage
--------------
When enabled, Commerce Items adds support for payment processing to CRM Core
Profile forms. This includes the following items:

- Amount field: for setting amounts.
- Shopping cart: for selecting line items.
- Payment form: for collecting payment details.
- Billing address: for including an optional billing address.

Commerce Items presents what are called meta-fields, meaning these are form
elements that are not attached directly to any entity. All configuration for
meta-fields happens on the settings page for the profile, there are no
options to select when attaching these items.

When a profile containing Commerce Items is submitted, an order is created
in Drupal Commerce. This module supports tokens that can be used to pass
information along to other entities in your profile. The two main ones to
be aware of are:

- [commerce-order:order-id]: the order id for the order.

- [commerce-order:commerce-order-total:amount_decimal]: the total amount
  of the order.

Configuration
-------------
Commerce Items are configured on the settings page for a CRM Core Profile.
The specific options displayed depends on what meta-fields have been added.

1) General

- Product: all profiles using commerce_items must have a product added. Use
  this field to select a product.

2) Amount field

- The Display option controls how the amount field will be presented.

  Select 'static' to display the field as text, where users cannot update
  the amount.

  Select 'variable' to display the field as a field, where users can change
  the amount.

  Select 'variable with buttons' to display the field as a field with buttons,
  which can be used to set the amount.

- Label is used to set a label for the Amount field when it appears. For example:
  instead of Amount it could be called Cost, Price, etc.

- Amounts is used to incidate the values that appear with buttons.

3) Cart

- Line items controls what will appear in the cart. Enter line items in the
  format of 'LABEL|AMOUNT' with one item per line. Commerce Items will convert
  this text to labels and values in the cart when the form is submitted.

4) Payment Form and Billing Information

- Available Payment Processors: allows you to control the payment processors
  available to users when they checkout using this form.

- Select Name Fields: allows you to control what name fields are passed into the
  payment processor. In general, it is best to use GIVEN and FAMILY.

- Select Billing Address: allows you to control what field is used for the billing
  address. Useful when a user is asked to submit more than one address field in
  a profile.

Developers
----------
Commerce Items allows developers to programatically set values for amount and
cart meta-fields using hook_crm_core_profile_data_alter() and hook_node_load().

To set items in a cart using hook_crm_core_profile_data_alter():

function hook_crm_core_profile_data_alter(&$profile, &$data) {
  $cart_items = array(
    'key1' => array(
      'title' => t('Item1 label'),
      // Decimal amount representing item price.
      'amount' => 100,
    ),
    'key2' => array(
      'title' => t('Item2 label'),
      // Decimal amount representing item price.
      'amount' => 100,
    ),
  );

  $data['commerce_items_cart_items'] = $cart_items;
}

As you can see each item is array containing 'title' and 'amount' keys, who's respective
values represents item label and price. Also you can use item key('key1' and 'key2' in
example above) to specify which product to use in the order. For this purpose use SKU of
your product of choice as the key for the item.

To do this through hook_node_load, use the same syntax for keys with the following
structure:

  $node->crm_core_profile_data['commerce_items_cart_items'] = array();

Within each array, add items using the following syntax:

  $node->crm_core_profile_data['commerce_items_cart_items'][] = array(
    'title'  => 'Platinum',
    'amount' => 100
  );

To set an amount:

Use $form_state['crm_core_profile_data']['commerce_items']['commerce_items_amount_single_settings']['amount']
to preset an amount field.

Maintainers
-----------

- techsoldaten - http://drupal.org/user/16339
- RoSk0 - https://drupal.org/user/325151

Development sponsored by Trellon.


