<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup ms_core_api MS Core API
 * @{
 * Hooks and functions that are used by MoneySuite modules.
 *
 * This includes functions for adding gateways, products, dealing with orders,
 * carts, etc.
 * @}
 */

/**
 * Allows for adding fields to the shopping cart page.
 *
 * This can be used to display forms or other information on the cart page.
 *
 * @return array
 *   An array of fields that should be added to cart page, containing:
 *     id: A unique field id.
 *     title: The field name.
 *     html: The html that should be displayed.
 *     weight: The weight of the field.
 *
 * @ingroup ms_core_api
 */
function hook_ms_cart_fields() {
  $fields = array();
  $fields[] = array(
    'id' => 'unique_field_id',
    'title' => t('Field name'),
    'html' => drupal_get_form('my_form'),
    'weight' => 11,
  );

  return $fields;
}

/**
 * Allows for adding fields to the checkout page.
 *
 * This can be used to display forms or other information on the checkout page.
 *
 * @return array
 *   An array of fields that should be added to checkout page, containing:
 *     id: A unique field id.
 *     title: The field name.
 *     html: The html that should be displayed.
 *     weight: The weight of the field.
 *
 * @ingroup ms_core_api
 */
function hook_ms_checkout_fields() {
  $fields = array();
  $fields[] = array(
    'id' => 'unique_field_id',
    'title' => t('Field name'),
    'html' => drupal_get_form('my_form'),
    'weight' => 11,
  );

  return $fields;
}

/**
 * Called whenever a product id is changed.
 *
 * This can be used to update any stored product ids in a database or other
 * places, so that the product ids are not orphaned.
 *
 * @param string $old_id
 *   The old product id.
 * @param string $new_id
 *   The new product id.
 *
 * @ingroup ms_core_api
 */
function hook_ms_product_id_change($old_id, $new_id) {
  // No example.
}

/**
 * Adds override settings to product forms.
 *
 * Using '#ms_override' => TRUE, you can specify a form element to be shown
 * on product plan pages so that users can override the setting on a per-plan
 * basis.
 *
 * @return array
 *   An array of modules keyed by module id containing:
 *     title: The title of the module
 *     form: The form builder containing the overridable settings.
 *
 * @see ms_core_variable_get()
 * @ingroup ms_core_api
 */
function hook_ms_core_overrides() {

}

/**
 * Registers products with MS Core.
 *
 * This is used to register all of a module's products for use with select lists
 * and other places.
 *
 * @return array
 *   An array of MsProduct objects containing the following extra keys:
 *     owner: The user id of the product owner.
 *     module_title: The title of the module.
 *     type: The type of product (recurring or cart).
 *     data: An array of product data.
 *     edit_path: The url of the edit product page.
 *     purchase_path: The url to purchase the product.
 *
 * @ingroup ms_core_api
 */
function hook_ms_products() {
  // No example.
}

/**
 * Assigns a user to an order.
 *
 * @param string $type
 *   The type of order, recurring or cart.
 * @param MsProduct $product
 *   The product object.
 * @param MsOrder $order
 *   The order object.
 * @param MsPayment $payment
 *   The payment object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_assign_user($type, $product, $order, $payment) {
  // No example.
}

/**
 * Act on a payment.
 *
 * @param string $type
 *   The type of payment, which could be:
 *   -cart: A one-time payment for a non-recurring orders.
 *   -rec_payment: A payment for a recurring order.
 *   -rec_signup: The first payment for a recurring order.
 *   -rec_cancel: Signals that a recurring order has been cancelled.
 *   -rec_modify: Signals that an order has been modified from one payment plan
 *     to another.
 *   -refund: The order has been refunded.
 *   -reversal: The order has undergone reversal.
 * @param MsProduct $product
 *   The product object.
 * @param MsOrder $order
 *   The order object.
 * @param MsPayment $payment
 *   The payment object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_payment($type, $product, $order, $payment) {
  switch ($product->module) {
    case 'mymodule':
      // Logic here
      break;
  }
}

/**
 * Allows modules to modify which payment gateways are shown at checkout.
 *
 * This gets called just after the gateways are fetched via hook_payment_gateway
 * for display as options on the checkout page. Useful for removing gateways
 * depending on other criteria such as user location in a profile field.
 *
 * @param array $gateways
 *   An array of payment gateways that can be altered.
 * @param object $cart
 *   The cart object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_payment_gateway_alter($gateways, $cart) {
  // Hide the PayPal WPS option.
  if (!empty($gateways['paypal_wps'])) {
    unset($gateways['paypal_wps']);
  }
}

/**
 * Allows payment gateways to show their particular billing info form.
 *
 * This is used by some gateways that save payment profiles for users and allow
 * them to edit them, such as Stripe and Authorize.net.
 *
 * @param string $html
 *  A display array that can have elements added to it.
 * @param object $account
 *  The user account.
 *
 * @ingroup ms_core_api
 */
function hook_ms_core_billing_info_alter(&$html, $account) {
  $payment_profiles = ms_core_payment_profiles_load_by_user($account->uid, 'MODULE_NAME');
  // Load the saved payment profiles and shown them with an Edit link that goes to an edit page
  foreach ($payment_profiles as $payment_profile) {
    $saved_card = (!empty($payment_profile->cc_num)) ? $payment_profile->cc_num : t('N/A');

    $billing_address = t('N/A');
    if ($payment_profile && isset($payment_profile->address)) {
      $billing_address = t("@address - @city , @state", array(
        '@address' => $payment_profile->address,
        '@city' => $payment_profile->city,
        '@state' => $payment_profile->state,
      ));
    }

    $html['GATEWAY_profile'][$payment_profile->id] = array(
      '#type' => 'fieldset',
      '#title' => t('Saved Profile - !edit', array('!edit' => l(t('Edit'), 'user/' . $payment_profile->uid . '/GATEWAY/billing/' . $payment_profile->id))),
    );
    $html['GATEWAY_profile'][$payment_profile->id]['card'] = array(
      '#type' => 'item',
      '#title' => t('Saved Card'),
      '#value' => $saved_card
    );
    $html['GATEWAY_profile'][$payment_profile->id]['billing_address'] = array(
      '#type' => 'item',
      '#title' => t('Billing Address'),
      '#value' => $billing_address,
    );
  }
}

/**
 * Allows modules to modify a product as it is added to cart.
 *
 * This is useful when you need to change something about a product when it is
 * added to the cart.
 *
 * @param MsProduct $product
 *   The product being added.
 * @param object $cart
 *   The cart object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_cart_added_product_alter($product, $cart) {
  // Change the product price.
  switch ($product->name) {
    case 'Silver Membership':
      $product->amount = 10.00;
      break;
  }
}

/**
 * React when a product is added to the cart.
 *
 * @param object $cart
 *   The cart object.
 * @param MsProduct $product
 *   The product that was added.
 *
 * @ingroup ms_core_api
 */
function hook_ms_cart_add($cart, $product) {
  // Add an adjustment to the cart for this product.
  $adjustment = new MsAdjustment();
  $adjustment->id = 'CUSTOM_FEE_1';
  $adjustment->product_id = $product->cart_product_id;
  $adjustment->display = "CUSTOM FEE";
  $adjustment->type = 'percentage';
  $adjustment->value = 10;
  $adjustment->weight = 1;
  $adjustment->scope = 'recurring';

  // Add the tax to the order.
  if ($adjustment->value) {
    ms_core_add_cart_adjustment($adjustment, TRUE);
  }
}

/**
 * Allows modules to act when a new order is created.
 *
 * @param MsOrder $order
 *   The order object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_new($order) {
  // No example.
}

/**
 * Allows modules to act when a new order is loaded.
 *
 * @param MsOrder $order
 *   The order object.
 *
 * @ingroup ms_core_api
 */
function hook_ms_order_load($order) {
  // No example.
}

/**
 * Allows modules to declare new payment methods to be used during Checkout.
 *
 * @return array
 *   An array of payment gateway implementation arrays, keyed by the machine
 *   name of the payment gateway. Each implementation array may contain the
 *   following keys:
 *   - name: The proper name of the payment gateway.
 *   - display_name: The text that will show as the label for the payment
 *     method on the checkout page.
 *   - description: The text that will show as the description for the payment
 *     method on the checkout page.
 *   - module: The name of the module that supports the payment gateway.
 *   - currency: (Optional) An array of currency codes that are supported by the
 *     payment gateway. If this is provided, and if the order being processed
 *     has an unsupported currency, then MS Core will convert the currency into
 *     the first currency supported by the module at the current exchange rate.
 *   - cards: (Optional) An array of credit card types that the payment gateway
 *     supports. This will also display a list of credit card icons next to the
 *     payment method on the checkout page.
 *   - active: (Optional) Whether or not the payment gateway will show on the
 *     Checkout form as a payment option. Defaults to TRUE.
 *   - active_variable: (Optional) A variable to check by calling variable_get()
 *     to see if the gateway has been marked active or not. The module can
 *     provide a settings page where users can toggle this variable to hide the
 *     payment gateway from the Checkout page, for example.
 *   - cancel_url: (Optional) If the payment gateway supports Recurring
 *     Payments, then it should provide a cancel workflow so that users can
 *     cancel their autobill. This setting is the callback name of a function
 *     that will return the URL where users can cancel their Recurring Payments.
 *     The callback will be passed the $order object as a parameter. MS Core
 *     provides a callback that can be used if the gateway has
 *     'recurring_schedule_support'. This callback is
 *     'ms_core_get_cancel_helper_url'.
 *   - modify_url: (Optional) If the payment gateway supports Recurring
 *     Payments, then it should provide a modify workflow so that users can
 *     change their autobill. This setting is the callback name of a function
 *     that will return the URL where users can modify their Recurring Payments.
 *     The callback will be passed the $order object as a parameter. MS Core
 *     provides a callback that can be used if the gateway has
 *     'recurring_schedule_support'. This callback is
 *     'ms_core_get_modify_helper_url' and it will call
 *     hook_ms_core_modification_charge() if there is any outstanding amount
 *     that must be billed as a part of the modification.
 *   - billing_url: (Optional) If the payment gateway supports Recurring
 *     Payments, then it should provide a change billing info workflow so that
 *     users can change their billing information. This setting is the callback
 *     name of a function that will return the URL where users can change their
 *     billing information. The callback will be passed the $order object as a
 *     parameter. MS Core provides a callback that can be used if the gateway
 *     has 'recurring_schedule_support'. This callback is
 *     'ms_core_get_billing_helper_url' and it will call
 *     hook_ms_core_billing_info_alter() to let modules place their billing
 *     info edit links onto the user account billing info page.
 *   - recurring_schedule_support: (Optional) If this payment gateway supports
 *     saving a 'Payment Profile' key and charging payments to a user's stored
 *     card information during cron, then set this to TRUE. When this is TRUE,
 *     the payment gateway is much more flexible and becomes a first-class
 *     member inside MS Core.
 *   - checkout_form: (Optional) The callback function that will build the
 *     payment form. Either this or checkout_path must be set. The function
 *     will be passed the $order object as a parameter.
 *   - checkout_path: (Optional) The URL where the user will be sent for payment
 *     during checkout. This should be a Drupal URL.
 *   - cart: (Optional) Whether or not the payment gateway supports regular,
 *     non-recurring orders. Defaults to TRUE.
 *   - recurring: (Optional) Whether or not the payment gateway supports
 *     recurring payments. Defaults to TRUE.
 */
function hook_ms_core_payment_gateway() {
  $gateways['mygateway'] = array(
    'name' => 'My Gateway',
    'display_name' => t('Credit Card'),
    'cards' => array('visa', 'mc', 'discover', 'amex'),
    'description' => t('Pay by Credit Card'),
    'module' => 'mygateway',
    'currency' => array('USD', 'CAD'),
    'active_variable' => 'mygateway_show_gateway',
    'cancel_url' => 'ms_core_get_cancel_helper_url',
    'modify_url' => 'ms_core_get_modify_helper_url',
    'billing_url' => 'ms_core_get_billing_helper_url',
    'recurring_schedule_support' => TRUE,
    'checkout_form' => 'mygateway_payment_submit_form',
  );

  // If the user has a saved card, show that as an option.
  global $user;
  if ($user->uid) {
    $payment_profiles = ms_core_payment_profiles_load_by_user($user->uid, 'mygateway');
    foreach ($payment_profiles as $payment_profile) {
      $gateways['mygateway_saved_profile_' . $payment_profile->id] = array(
        'name' => 'My Gateway Saved Card',
        'display_name' => t('Credit Card'),
        'description' => t("Pay with your saved card: @card", array('@card' => $payment_profile->cc_num)),
        'cards' => array($payment_profile->cc_type),
        'module' => 'mygateway_saved_profile_' . $payment_profile->id,
        'currency' => array('USD', 'CAD'),
        'checkout_path' => 'ms/checkout/saved/' . $payment_profile->id,
        'active_variable' => 'mygateway_show_gateway',
        'cancel_url' => 'ms_core_get_cancel_helper_url',
        'modify_url' => 'ms_core_get_modify_helper_url',
        'billing_url' => 'ms_core_get_billing_helper_url',
        'recurring_schedule_support' => TRUE,
      );
    }
  }

  return $gateways;
}
