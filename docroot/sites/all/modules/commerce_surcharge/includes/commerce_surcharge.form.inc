<?php

/**
 * @file
 * Form functions.
 */

/**
 * Rebuild checkout review order details views.
 * @param $form
 * @param $form_state
 */
function _commerce_surcharge_checkout_review_rebuild($form, $form_state) {
  // Return if no valid order.
  if (!isset($form_state['order'])) {
    return;
  }

  // If cardonfile module is available.
  if (isset($form['commerce_payment']['payment_details']['cardonfile'])) {
    if (isset($form_state['values'])) {
      $cardonfile = $form_state['values']['commerce_payment']['payment_details']['cardonfile'];
    }
    else {
      if(isset($form['commerce_payment']['payment_details']['cardonfile']['#default_value'])) {
        $cardonfile = $form['commerce_payment']['payment_details']['cardonfile']['#default_value'];
      }
    }
  }

  if (isset($cardonfile) && $cardonfile !== 'new') {
    $card = commerce_cardonfile_load($cardonfile);
    $current_card_type = $card->card_type;
  }
  else {
    // Get credit card type.
    $options = $form['commerce_payment']['payment_details']['credit_card']['type']['#options'];
    reset($options);
    $current_card_type = key($options);
    if (isset($form_state['values']['commerce_payment']['payment_details'])) {
      $payment_details = $form_state['values']['commerce_payment']['payment_details'];
      if ($payment_details['credit_card']['type']) {
        $current_card_type = $payment_details['credit_card']['type'];
      }
    }
  }

  variable_set('commerce_surcharge_current_card', $current_card_type);

  // Invoke custom rule - order id is required.
  $order = commerce_order_load($form_state['order']->order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Delete existing surcharge line item.
  $line_item_ids = array();

  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() == COMMERCE_SURCHARGE_TYPE) {
      // Delete the line item, and remove it from the order.
      $line_item_ids[] = $line_item_wrapper->line_item_id->value();
      $order_wrapper->commerce_line_items->offsetUnset($delta);
    }
  }

  // If we found any fee type line items...
  if (!empty($line_item_ids)) {
    // Then delete the line items.
    commerce_line_item_delete_multiple($line_item_ids);
  }

  // Hook rule event.
  rules_invoke_event('commerce_surcharge_order', $order_wrapper);

  //@todo: user can setup views name.
  $views_pay_summary = views_embed_view('commerce_cart_summary', $display_id = 'default', $form_state['order']->order_id);
  if (isset($form['checkout_review']['review']['#data']['cart_contents']['data'])) {
    $form['checkout_review']['review']['#data']['cart_contents']['data'] = $views_pay_summary;
  }
  return $form['checkout_review'];
}
