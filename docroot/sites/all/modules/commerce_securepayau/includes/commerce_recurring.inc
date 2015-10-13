<?php
/**
 * Commerce Recurring module integration.
 */

/**
 * Implements hook_commerce_securepayau_commerce_payment_method_info_alter() on
 * behalf of commerce_recurring.module.
 */
function commerce_recurring_commerce_securepayau_commerce_payment_method_info_alter(&$payment_methods) {
  $payment_methods['securepayau_xmlapi']['cardonfile'] = isset($payment_methods['securepayau_xmlapi']['cardonfile'])
    ? $payment_methods['securepayau_xmlapi']['cardonfile']
    : array();
  $payment_methods['securepayau_xmlapi']['cardonfile']['charge callback'] = 'commerce_securepayau_xml_cardonfile_charge';
}

/*
 * Implements commerce_cardonfile charge callback
 *
 * @param $order object
 *    The order being charged
 * @param $parent_order object
 *    The parent order from which the recurring order was derived
 */
function commerce_securepayau_xml_cardonfile_charge($order, $parent_order) {
  // Retrieve the card on file
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $instance_id = $parent_order->data['payment_method'];
  $payment_method = commerce_payment_method_instance_load($instance_id);
  $settings = $payment_method['settings'];
  $cards_on_file = commerce_cardonfile_data_load_multiple($order->uid, $payment_method['instance_id']);

  // @TODO - Figure out how to deal with multiple cards on file.
  $pane_values = array(
    'cardonfile' => key($cards_on_file)
  );

  // @TODO - Figure out how to extract $charge object from $order object correctly.
  $charge = $order->commerce_order_total['und'][0];

  // Process payment.
  $result = _commerce_cardonfile_commerce_securepayau_xmlapi_submit_form_submit($payment_method, array(), $pane_values, $order, $charge);

  if (!$result) {
    // @TODO - Error
    return FALSE;
  }

  // Mark order as completed.
  $order = commerce_order_status_update($order, "completed");

  return TRUE;
}
