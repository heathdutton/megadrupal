<?php

/**
 * @file
 * Preprocess function for all ec_store templates.
 */

/**
 * Implementation of hook_preprocess_hook().
 */
function template_preprocess_invoice(&$variables) {
  drupal_add_css(drupal_get_path('module', 'ec_store') . '/templates/invoice.css');

  $txn = & $variables['transaction'];

  $variables['template_files'][] = 'invoice-' . $txn->type;
  $variables['template_files'][] = 'invoice-' . $txn->payment_method;
  $variables['template_files'][] = 'invoice-' . $txn->type . '-' . $txn->payment_method;

  $variables['billing_address'] = theme('formatted_address', array('address' => $txn->address['billing']));
  if (isset($txn->shippable) && $txn->shippable) {
    $variables['shipping_address'] = theme('formatted_address', array('address' => $txn->address['shipping']));
  }

  $subtotal = 0;
  foreach ($txn->items as $nid => $item) {
    $txn->items[$nid]->price = ec_store_adjust_misc($txn, $item);
    $txn->items[$nid]->formatted_price = format_currency($txn->items[$nid]->price);
    $txn->items[$nid]->quantity_visible = ec_product_has_quantity($item);
    $subtotal += $txn->items[$nid]->price * ec_product_check_qty($txn->items[$nid], $txn->items[$nid]->qty);
  }

  $variables['subtotal'] = format_currency($subtotal);

  $variables['gross'] = format_currency(ec_store_transaction_calc_gross($txn));

  if (!empty($txn->additional)) {
    $variables['additional'] = drupal_render($txn->additional);
  }

  $variables += (array) $variables['transaction'];

  // TODO: Need to fill in more information in the misc variables
  uasort($variables['misc'], 'ec_sort');
  foreach ($variables['misc'] as $id => $misc) {
    if ($misc->seen) {
      unset($variables['misc'][$id]);
    }
  }

  $defaults = array(
    'additional' => '',
  );

  $variables += $defaults;
}

/**
 *
 */
function template_preprocess_formatted_address(&$variables) {
  if (is_object($variables['address'])) {
    $variables['address'] = (array) $variables['address'];
  }
  $address = & $variables['address'];

  $info = ec_store_get_region_info($address['country']);

  $variables['name'] = check_plain(!empty($address['fullname']) ? $address['fullname'] : $address['firstname'] . ' ' . $address['lastname']);

  $variables['street_address'] = '';
  if ($variables['include_name']) {
    $variables['street_address'] = $variables['name'] . '<br />';
  }
  $variables['street_address'] .= $address['street1'];
  if ($address['street1'] && $address['street2']) {
    $variables['street_address'] .= '<br />';
  }
  if ($address['street2']) {
    $variables['street_address'] .= $address['street2'];
  }

  $variables['country'] = ec_store_get_country($address['country']);

  if ($info['use_state_names'] && isset($info['states'][$address['state']])) {
    $variables['state'] = $info['states'][$address['state']];
  }

  $variables += $address;

  $country = drupal_strtolower($variables['address']['country']);
  $variables['template_files'][] = 'formatted-address-' . $country;
}
