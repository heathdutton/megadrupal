<?php

/**
 * @file
 * MS Core Rules Includes
 * Licensed under the GNU GPLv2 License
 *
 */
// ======================================
// RULES - CONDITIONS
// ======================================
/*
 * Conditions:
 * + Check the Status of an order
 * + Check the payment type
 * + Check the payment amount greater than
 * + Check the product
 * - @todo - Check that a product is within an order
 * - @todo - Check the product module
 * + Check the payment gateway
 * + Check the payment number
 */

/**
 * Implements  hook_rules_condition_info().
 */
function ms_core_rules_condition_info() {
  return array(
    'ms_core_condition_order_status' => array(
      'label' => t('Check the status of the order'),
      'parameter' => array(
        'order' => array(
          'type' => 'ms_order',
          'label' => t('Order', array(), array('context' => 'ms_core')),
        ),
        'status' => array(
          'type' => 'text',
          'label' => t('Status'),
          'options list' => 'ms_core_get_order_statuses',
        ),
      ),
      'group' => 'MS Core',
    ),
    'ms_core_condition_payment_type' => array(
      'label' => t('Check the type of payment'),
      'parameter' => array(
        'payment' => array(
          'type' => 'ms_payment',
          'label' => t('Payment'),
        ),
        'type' => array(
          'type' => 'text',
          'label' => t('Payment Type'),
          'options list' => 'ms_core_get_payment_types',
        ),
      ),
      'group' => 'MS Core',
    ),
    'ms_core_condition_payment_amount_greater_than' => array(
      'label' => t('Check if payment amount is greater than X'),
      'parameter' => array(
        'payment' => array(
          'type' => 'ms_payment',
          'label' => t('Payment'),
        ),
        'amount' => array(
          'type' => 'number',
          'label' => t('Amount'),
        ),
      ),
      'group' => 'MS Core',
    ),
    'ms_core_condition_payment_number' => array(
      'label' => t('Check the payment number'),
      'parameter' => array(
        'order' => array(
          'type' => 'ms_order',
          'label' => t('Order', array(), array('context' => 'ms_core')),
        ),
        'payment' => array(
          'type' => 'ms_payment',
          'label' => t('Payment'),
        ),
      ),
      'group' => 'MS Core',
    ),
    'ms_core_condition_product_id' => array(
      'label' => t('Check the product'),
      'parameter' => array(
        'product' => array(
          'type' => 'ms_product',
          'label' => t('Product'),
        ),
        'id' => array(
          'type' => 'text',
          'label' => t('Product'),
          'options list' => 'ms_core_get_module_products_list',
        ),
      ),
      'group' => 'MS Core',
    ),
    'ms_core_condition_gateway' => array(
      'label' => t('Check the chosen Payment Gateway'),
      'parameter' => array(
        'gateway' => array(
          'type' => 'text',
          'label' => t('Gateway'),
          'options list' => 'ms_core_get_payment_gateways_list',
        ),
      ),
      'group' => 'MS Core',
    ),
  );
}

/**
 * Condition: Check th
 */
function ms_core_condition_gateway($gateway) {
  return $_SESSION['ms_gateway'] == $gateway;
}

/**
 * Condition: Check the payment number
 */
function ms_core_condition_payment_number($order, $payment) {
  return array_search($payment, $order);
}

/**
 * Condition: Check the payment number
 */
function ms_core_condition_payment_type($payment, $type) {
  return $payment->type == $type;
}

/**
 * Condition: Check the payment amount is greater than X
 */
function ms_core_condition_payment_amount_greater_than($payment, $amount) {
  return $payment->amount > $amount;
}

/**
 * Condition: Check the product id
 */
function ms_core_condition_product_id($product, $id) {
  $product_id = $product->module . '-' . $product->id;
  return $product_id == $id;
}

/**
 * Condition: Check the value of a field.
 */
function ms_core_condition_order_status($order, $status) {
  if ($order->status == $status) {
    return TRUE;
  }
  return FALSE;
}

// ======================================
// RULES - ACTIONS
// ======================================

/*
 * Actions:
 * + Change Order Status (Active, Cancelled, EOT, etc)
 * + Add Adjustment to Cart
 * + Remove Adjustment from Cart
 * - @todo Add a product to the cart
 */

/**
 * Implements  hook_rules_action_info().
 */
function ms_core_rules_action_info() {
  return array(
    'ms_core_action_change_order_status' => array(
      'label' => t('Change the Status of an Order'),
      'parameter' => array(
        'order' => array(
          'type' => 'ms_order',
          'label' => t('Order', array(), array('context' => 'ms_core')),
        ),
        'status' => array(
          'type' => 'text',
          'label' => t('Status'),
          'options list' => 'ms_core_get_order_statuses',
        ),
      ),
      'group' => 'MS Core',
    ),
    'ms_core_action_add_cart_adjustment' => array(
      'label' => t('Add an Adjustment to a Cart'),
      'parameter' => array(
        'adjustment' => array(
          'type' => 'ms_cart_adjustment',
          'label' => t('Adjustment'),
          'restriction' => 'input',
        ),
      ),
      'group' => 'MS Core',
    ),
    'ms_core_action_remove_cart_adjustment' => array(
      'label' => t('Remove an adjustment from a Cart'),
      'parameter' => array(
        'adjustment' => array(
          'type' => 'ms_cart_adjustment',
          'label' => t('Adjustment'),
          'restriction' => 'input',
        ),
      ),
      'group' => 'MS Core',
    ),
  );
}

/**
 * Action: Add an Adjustment to a Cart
 */
function ms_core_action_add_cart_adjustment($values) {
  $adjustment = ms_core_rules_create_adjustment($values);
  ms_core_add_cart_adjustment($adjustment);
}

/**
 * Action: Remove an Adjustment from a Cart
 */
function ms_core_action_remove_cart_adjustment($values) {
  $adjustment = ms_core_rules_create_adjustment($values);
  ms_core_remove_cart_adjustment($adjustment);
}

/**
 * Action: Change the status of an Order
 */
function ms_core_action_change_order_status($order, $status) {
  // Change the Order Status
  return ms_core_set_order_status($order, $status);
}

/*
 * Events:
 * + Order is marked as "X"
 * + Payment is received for an order
 * + Product added to cart
 * + Cart moves to checkout
 */

/**
 * Implements  hook_rules_event_info()
 */
function ms_core_rules_event_info() {
  $items = array(
    'ms_core_event_order_status_changed' => array(
      'label' => t('Order Status Changed'),
      'group' => 'MS Core',
      'help' => t("Act when an order is marked as completed."),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User Account'),
        ),
        'order' => array(
          'type' => 'ms_order',
          'label' => t('Order', array(), array('context' => 'ms_core')),
        ),
        'old_status' => array(
          'type' => 'text',
          'label' => t('Old Order Status'),
        ),
        'new_status' => array(
          'type' => 'text',
          'label' => t('New Order Status'),
        ),
      ),
    ),
    'ms_core_event_order_payment_received' => array(
      'label' => t('Order Payment Received'),
      'group' => 'MS Core',
      'help' => t("Act when a payment is received for an order"),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User Account'),
        ),
        'order' => array(
          'type' => 'ms_order',
          'label' => t('Order', array(), array('context' => 'ms_core')),
        ),
        'payment' => array(
          'type' => 'ms_payment',
          'label' => t('Payment'),
        ),
      ),
    ),
    'ms_core_event_cart_checkout' => array(
      'label' => t('User begins Checkout'),
      'group' => 'MS Core',
      'help' => t("Gets triggered when the user moves to the Checkout Page"),
      'variables' => array(
        'cart' => array(
          'type' => 'ms_cart',
          'label' => t('Cart'),
        ),
      ),
    ),
    'ms_core_event_product_add_to_cart' => array(
      'label' => t('Product Added to Cart'),
      'group' => 'MS Core',
      'help' => t("Triggered when a Product is added to a cart"),
      'variables' => array(
        'product' => array(
          'type' => 'ms_product',
          'label' => t('Product'),
        ),
        'cart' => array(
          'type' => 'ms_cart',
          'label' => t('Cart'),
        ),
      ),
    ),
    'ms_core_event_gateway_checkout' => array(
      'label' => t('Payment Gateway Chosen'),
      'group' => 'MS Core',
      'help' => t("Triggered when a Payment Gateway is Chosen for Checkout"),
      'variables' => array(),
    ),
  );

  return $items;
}

/*
 * Data Types:
 * + Order
 * + Payment
 * + Product
 * + Cart Adjustment
 * + Cart
 */

/**
 * Implements hook_rules_data_info()
 */
function ms_core_rules_data_info() {
  return array(
    'ms_order' => array(
      'label' => t('Order', array(), array('context' => 'ms_core')),
      'wrap' => TRUE,
      'group' => t('MS Core'),
    ),
    'ms_cart' => array(
      'label' => t('Cart'),
      'wrap' => TRUE,
      'group' => 'MS Core',
    ),
    'ms_cart_adjustment' => array(
      'label' => t('Cart Adjustment'),
      'wrap' => TRUE,
      'group' => 'MS Core',
      'ui class' => 'RulesDataUIMsAdjustment',
    ),
    'ms_product' => array(
      'label' => t('Product'),
      'wrap' => TRUE,
      'group' => t('MS Core'),
    ),
    'ms_payment' => array(
      'label' => t('Payment'),
      'wrap' => TRUE,
      'group' => t('MS Core'),
    ),
  );
}

/**
 * Creates an adjustment object from the parameters specified
 */
function ms_core_rules_create_adjustment($values) {
  $adjustment = new MsAdjustment();
  $adjustment->id = 'ms_core_rules_adjustment';
  $adjustment->display = $values['name'];
  $adjustment->type = $values['type'];
  $adjustment->value = $values['value'];
  $adjustment->weight = isset($values['weight']) ? $values['weight'] : 0;
  $adjustment->scope = $values['scope'];
  return $adjustment;
}

/**
 * UI for textual data.
 */
class RulesDataUIMsAdjustment extends RulesDataUI implements RulesDataDirectInputFormInterface
{

  public static function getDefaultMode() {
    return 'input';
  }

  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    $form = array();
    $form['adjustment'] = array(
      '#type' => 'fieldset',
      '#title' => t('Adjustment Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['adjustment']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Display Text for the adjustment'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => isset($settings['adjustment']['name']) ? $settings['adjustment']['name'] : '',
      '#description' => t("This is what will appear next to the adjustment amount on orders."),
      '#required' => TRUE,
    );
    $form['adjustment']['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Adjustment value'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => isset($settings['adjustment']['value']) ? $settings['adjustment']['value'] : 0,
      '#description' => t("The value of the adjustment. Negative numbers will give discounts."),
      '#required' => TRUE,
    );
    $form['adjustment']['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Adjustment weight'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => isset($settings['adjustment']['weight']) ? $settings['adjustment']['weight'] : 0,
      '#description' => t("The weight of the adjustment. Specifies which order the adjustments are calculated in. Can be negative."),
      '#required' => TRUE,
    );
    $form['adjustment']['type'] = array(
      '#type' => 'radios',
      '#title' => t('Percentage or Fixed Amount?'),
      '#default_value' => isset($settings['adjustment']['type']) ? $settings['adjustment']['type'] : 'percentage',
      '#options' => array(
        'percentage' => t('Percentage'),
        'fixed' => t('Fixed'),
      ),
      '#required' => TRUE,
    );
    $form['adjustment']['scope'] = array(
      '#type' => 'radios',
      '#title' => t('Scope'),
      '#default_value' => isset($settings['adjustment']['scope']) ? $settings['adjustment']['scope'] : 'recurring',
      '#options' => ms_core_get_scopes(),
      '#required' => TRUE,
      '#description' => t('Whether the adjustment should affect the first payment only or all payments. Only applies for Recurring Payments.'),
    );
    return $form;
  }

  public static function render($value) {
    return array(
      'content' => array('#markup' => check_plain($value['name'])),
      '#attributes' => array('class' => array('rules-parameter-text')),
    );
  }
}
