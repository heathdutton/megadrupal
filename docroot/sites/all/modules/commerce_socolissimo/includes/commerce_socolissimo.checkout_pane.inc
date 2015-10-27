<?php

/**
 * @file
 * Checkout pane hooks
 */

/**
 * Pane form: renders.
 */
function commerce_socolissimo_pane_review($form, $form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    // If this line item is a shipping line item...
    if ($line_item_wrapper->getBundle() == 'shipping') {
      $line_item = $line_item_wrapper->value();
      $point = isset($line_item->data['service_details']['commerce_socolissimo_point']) ? $line_item->data['service_details']['commerce_socolissimo_point'] : NULL;
    }
  }

  if (isset($point)) {
    // We have to display the customer phone number.
    $customer_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
    $element = _commerce_socolissimo_convert_into_render_array($point);
    $element['#phone_number'] = $customer_address['phone_number'];
    $element['#delivery_delay'] = _commerce_socolissimo_delivery_delay();

    return theme('commerce_socolissimo_point_details', array('element' => $element));
  }

  return '';
}
