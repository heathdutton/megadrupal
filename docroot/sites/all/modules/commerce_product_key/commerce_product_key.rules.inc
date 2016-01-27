<?php

/**
 * @file
 * Rules integration for product keys.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_product_key_rules_event_info() {
  $events = array();

  $events['commerce_product_key_presave'] = array(
    'label' => t('Before saving a product key'),
    'group' => t('Commerce Product Key'),
    'variables' => commerce_product_key_rules_event_variables(t('Product key'), TRUE, TRUE),
    'access callback' => 'commerce_product_key_rules_access',
  );
  $events['commerce_product_key_insert'] = array(
    'label' => t('After saving a new product key'),
    'group' => t('Commerce Product Key'),
    'variables' => commerce_product_key_rules_event_variables(t('Created product key'), TRUE),
    'access callback' => 'commerce_product_key_rules_access',
  );
  $events['commerce_product_key_update'] = array(
    'label' => t('After updating an existing product key'),
    'group' => t('Commerce Product Key'),
    'variables' => commerce_product_key_rules_event_variables(t('Updated product key'), TRUE),
    'access callback' => 'commerce_product_key_rules_access',
  );
  $events['commerce_product_key_delete'] = array(
    'label' => t('After deleting a product key'),
    'group' => t('Commerce Product Key'),
    'variables' => commerce_product_key_rules_event_variables(t('Deleted product key')),
    'access callback' => 'commerce_product_key_rules_access',
  );

  return $events;
}

/**
 * Returns a variables array for product key events.
 *
 * @param $label
 *   The label for the primary product key variable.
 * @param $unchanged
 *   Boolean indicating whether or not to include the unchanged product.
 * @param $skip_save
 *   Boolean indicating whether or not the primary product key variable should skip
 *     saving after event execution.
 */
function commerce_product_key_rules_event_variables($label, $unchanged = FALSE, $skip_save = FALSE) {
  $variables = array(
    'product_key' => array(
      'type' => 'commerce_product_key',
      'label' => $label,
      'skip save' => $skip_save,
    ),
  );

  if ($unchanged) {
    $variables['product_key_unchanged'] = array(
      'type' => 'commerce_product_key',
      'label' => t('Unchanged product key'),
      'skip save' => TRUE,
      'handler' => 'rules_events_entity_unchanged',
    );
  }

  return $variables;
}

/**
 * Rules integration access callback.
 */
function commerce_product_key_rules_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return commerce_product_key_access('view');
  }
}

/**
 * Rules integration access callback.
 */
function commerce_product_key_rules_action_info() {
  $actions = array();

  $actions ['assign_product_key'] = array(
    'label' => t('Assign a product key to a order and user'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to update'),
        ),
      ),
    'group' => t('Commerce Product Key'),
    'callbacks' => array(
      'execute' => 'commerce_product_key_rules_assign_product_key',
      ),
    );
  /*
  $actions ['assign_product_key_payment_successful'] = array(
    'label' => t('Assign a product key to a order and user, if payment is successful'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to update'),
        ),
      ),
    'group' => t('Commerce Product Key'),
    'callbacks' => array(
      'execute' => 'commerce_product_key_rules_assign_product_key_payment_successful',
      ),
    );
  */
  $actions ['remove_product_key'] = array(
    'label' => t('Remove all product keys from a order and user'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to remove product keys from'),
        ),
      ),
      'group' => t('Commerce Product Key'),
      'callbacks' => array(
      'execute' => 'commerce_product_key_rules_remove_product_key_from_order',
      ),
    );

  $actions ['update_status_product_key'] = array(
    'label' => t('Update the status on all product keys for a order'),
    'parameter' => array(
      'order' => array(
      'type' => 'commerce_order',
      'label' => t('Order to update product keys on'),
    ),

  'product_key_status' => array(
    'type' => 'text',
    'label' => t('Product Key status'),
    'options list' => 'commerce_product_key_status_options_list',
    'restriction' => 'input',
    ),
  ),
  'group' => t('Commerce Product Key'),
  'callbacks' => array(
    'execute' => 'commerce_product_key_rules_update_status_product_key',
    ),
  );

  return $actions;
}

/**
 * Rules action: assign product keys to user.
 */
function commerce_product_key_rules_assign_product_key($order) {
  if (!empty($order)) {
    //push the order to our function todo the heavy lifting.
    commerce_product_key_update($order);
  }
}

/**
 * Rules action: assign product keys to user, only if the payment was successful.
 */
function commerce_product_key_rules_assign_product_key_payment_successful($order) {
  if (!empty($order)) {
    //push the order to our function todo the heavy lifting.
    commerce_product_key_update_payment_successful($order);
  }
}

/**
 * Rules action: remove product key from user.
 */
function commerce_product_key_rules_remove_product_key_from_order($order) {
  if (!empty($order)) {
    //push the $order to our function todo the heavy lifting.
    commerce_product_key_remove_from_order($order);
  }
}

/**
 * Rules action: set status on a product key.
 */
function commerce_product_key_rules_update_status_product_key($order, $status) {
  if (!empty($order)) {
    //push the $order to our function todo the heavy lifting.
    commerce_product_key_status_product_key($order, $status);
  }
}