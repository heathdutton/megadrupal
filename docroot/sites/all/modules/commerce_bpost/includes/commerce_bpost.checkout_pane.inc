<?php

/**
 * @file
 * Checkout pane hooks
 */

/**
 * bpost shipping pane form render.
 */
function commerce_bpost_pane_review($form, $form_state, $checkout_pane, $order) {
  // Only display if we have a bpost shipping service selected.
  if ($line_item_wrapper = _commerce_bpost_get_shipping_line_item($order)) {
    $line_item = $line_item_wrapper->value();
    $content = array(
      'title' => $line_item_wrapper->line_item_label->value(),
      'price' => commerce_currency_format($line_item_wrapper->commerce_unit_price->amount->value(), $line_item_wrapper->commerce_unit_price->currency_code->value()),
    );
    // If there is a delivery point, add details about it.
    if ($line_item_wrapper->commerce_shipping_service->value() == 'bpost_bpack' || $line_item_wrapper->commerce_shipping_service->value() == 'bpost_postoffice_postpoint') {
      $content['point_details'] = theme_commerce_bpost_point(array('point' => $line_item->data['point_details']['Poi']));
    }
    return theme('commerce_bpost_shipping_service', array('content' => $content));
  }
}
