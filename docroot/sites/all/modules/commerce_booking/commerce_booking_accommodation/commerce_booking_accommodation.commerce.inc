<?php
/**
 * @file
 * Contains Drupal Commerce hooks.
 */

/**
 * Implements hook_commerce_checkout_page_info().
 */
function commerce_booking_accommodation_commerce_checkout_page_info() {
  $pages = array();

  $pages['accommodation'] = array(
    'page_id' => 'accommodation',
    'title' => t('Accommodation'),
    'weight' => 3,
    'status_cart' => FALSE,
  );

  return $pages;
}

/**
 * Implements hook_commerce_checkout_pane_info().
 */
function commerce_booking_accommodation_commerce_checkout_pane_info() {
  $panes = array();

  $panes['accommodation'] = array(
    'pane_id' => 'accommodation',
    'title' => FALSE,
    'name' => t('Accommodation'),
    'page' => 'accommodation',
    'review' => FALSE,
    'file' => 'commerce_booking_accommodation.checkout.inc',
    'base' => 'commerce_booking_accommodation_checkout_pane_accommodation',
  );

  return $panes;
}

/**
 * Implements hook_commerce_line_item_type_info().
 */
function commerce_booking_accommodation_commerce_line_item_type_info() {
  $line_item_types = array();

  $line_item_types['commerce_booking_accommodation'] = array(
    'type' => 'commerce_booking_accommodation',
    'name' => t('Accommodation'),
    'description' => t('References an accommodation type.'),
    'product' => TRUE,
    'add_form_submit_value' => t('Add accommodation'),
    'base' => 'commerce_booking_accommodation_line_item_accommodation',
  );

  return $line_item_types;
}

/**
 * Title callback for commerce_booking_accommodation line item.
 */
function commerce_booking_accommodation_line_item_accommodation_title($line_item) {
  $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  return $wrapper->cba_accommodation_type->label();
}

/**
 * Form constuctor for adding a custom charge line item.
 */
function commerce_booking_accommodation_line_item_accommodation_add_form($element, &$form_state) {
  form_load_include($form_state, 'inc', 'commerce_booking_accommodation', 'commerce_booking_accommodation.commerce');
  $form = array();

  // Build a dummy line item.
  $values = array('type' => 'commerce_booking_accommodation');

  // If possible, get the order from the build info.
  if (isset($form_state['build_info']['args'][0]->order_id)) {
    $values['order_id'] = $form_state['build_info']['args'][0]->order_id;
  }

  $line_item = entity_create('commerce_line_item', $values);

  $options = array(
    'field_name' => 'cba_accommodation_type',
  );
  field_attach_form('commerce_line_item', $line_item, $form, $form_state, NULL, $options);
  $form['cba_accommodation_type']['#weight'] = -1;

  return $form;
}

/**
 * Submission handler for custom charge line item add form.
 */
function commerce_booking_accommodation_line_item_accommodation_add_form_submit(&$line_item, $element, &$form_state, $form) {
  $errors = form_get_errors();
  if (!empty($errors)) {
    return t('Unable to add the line item.');
  }

  $line_item->quantity = 1;
  field_attach_submit('commerce_line_item', $line_item, $form, $form_state);
}

/**
 * Calculate the value of an accommodation line item.
 */
function commerce_booking_accommodation_line_item_accommodation_process(&$line_item) {
  if ($line_item->type != 'commerce_booking_accommodation') {
    drupal_set_message(t('Invalid line item type.'), 'error');
    return;
  }

  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  if (!$line_item_wrapper->cba_accommodation_type->value()) {
    drupal_set_message(t('No accommodation selected.'), 'error');
    return;
  }

  // Don't recalculate if cancelled or paid in full. Check paid in full value
  // than using the status to allow cancellations etc.
  $line_item_balance = commerce_booking_line_item_balance($line_item_wrapper->value());
  if (!empty($line_item->is_new) || ($line_item_balance['amount'] > 0 && $line_item_wrapper->commerce_line_item_status->value() != 'cancelled')) {
    // @TODO: Cope without flexible pricing and remove it as a dependency
    $handler = CommerceFlexiblePricingHandlerAccommodationType::getHandler($line_item_wrapper->cba_accommodation_type->value());
    $price = $handler->calculatePrice($line_item);
    $line_item_wrapper->commerce_unit_price =
      $line_item_wrapper->commerce_total = $price;

    if (!is_null($line_item_wrapper->commerce_unit_price->value())) {
      // Add the base price to the components array.
      if (!commerce_price_component_load($line_item_wrapper->commerce_unit_price->value(), 'base_price')) {
        $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
          $line_item_wrapper->commerce_unit_price->value(),
          'base_price',
          $line_item_wrapper->commerce_unit_price->value(),
          TRUE
        );
      }
    }

    if (!is_null($line_item_wrapper->commerce_total->value())) {
      // Add the base price to the components array.
      if (!commerce_price_component_load($line_item_wrapper->commerce_total->value(), 'base_price')) {
        $line_item_wrapper->commerce_total->data = commerce_price_component_add(
          $line_item_wrapper->commerce_total->value(),
          'base_price',
          $line_item_wrapper->commerce_total->value(),
          TRUE
        );
      }
    }

    // Re-calculate the balance.
    $line_item_balance = commerce_booking_line_item_balance($line_item_wrapper->value());
  }

  switch ($line_item_wrapper->commerce_line_item_status->value()) {
    case 'cancelled':
      // Do nothing.
      break;

    // Check if we need to set back to deposit_paid.
    case 'paid_in_full':
      if ($line_item_balance['amount'] > 0) {
        $line_item_wrapper->commerce_line_item_status = 'deposit_paid';
      }
      break;

    // Otherwise check if we need to mark as paid in full.
    default:
      if ($line_item_balance['amount'] <= 0) {
        $line_item_wrapper->commerce_line_item_status = 'paid_in_full';
      }
      break;
  }
}

/**
 * Implements hook_commerce_payment_transaction_insert().
 */
function commerce_booking_accommodation_commerce_payment_transaction_insert($transaction, $type = 'commerce_payment_transaction') {
  $transaction_wrapper = entity_metadata_wrapper('commerce_payment_transaction', $transaction);

  $order = $transaction_wrapper->order->value();

  if (empty($order->data['ticket_amounts'])) {
    return;
  }

  foreach ($order->data['ticket_amounts'] as $key => $amount) {
    $key_parts = explode('_', $key);
    if ($key_parts[0] != 'accommodation') {
      continue;
    }

    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $key_parts[1]);

    // Generate the payment.
    $item = entity_create('field_collection_item', array(
      'field_name' => 'commerce_booking_ticket_payments',
    ));
    $item->setHostEntity('commerce_line_item', $line_item_wrapper->value());

    $item_wrapper = entity_metadata_wrapper('field_collection_item', $item);
    $item_wrapper->cbt_payment_payment = $transaction;
    $item_wrapper->cbt_payment_value = $amount;
    $item_wrapper->save();
    $line_item_wrapper->save();

    // Unset so that this doesn't happen twice.
    unset($order->data['ticket_amounts'][$key]);
  }
}
