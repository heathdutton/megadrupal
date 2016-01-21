<?php
/**
 * @file
 * Form callbacks for Commerce Flo2Cash.
 */

/**
 * Implements CALLBACK_commerce_payment_method_settings_form().
 */
function commerce_flo2cash_settings_form($settings) {
  // Add default settings if not yet set.
  $settings += array(
    'web2pay_mode' => 'demo',
    'web2pay_url' => 'https://demo.flo2cash.co.nz/web2pay/default.aspx',
    'web2pay_mns_url' => 'https://demo.flo2cash.co.nz/web2pay/MNSHandler.aspx',
    'flo2cash_account_id' => '',
  );

  // Web2Pay mode.
  $default_mode = $settings['web2pay_mode'] ? $settings['web2pay_mode'] : 'demo';
  $form['web2pay_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction Mode'),
    '#options' => array(
      'demo' => t('Demo - use for testing, requires a demo account'),
      'live' => t('Live - use for processing real transactions'),
    ),
    '#default_value' => $settings['web2pay_mode'],
  );

  // Web2Pay URL.
  $form['web2pay_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Flo2Cash Web2Pay URL'),
    '#description' => t('Flo2Cash hosted paged URL where the payment request is to be sent. <strong>Demo should use <em>https://demo.flo2cash.co.nz/web2pay/default.aspx</em> and live should use <em>https://secure.flo2cash.co.nz/web2pay/default.aspx</em>.</strong>'),
    '#default_value' => $settings['web2pay_url'],
    '#required' => TRUE,
  );

  // Flo2Cash MNS (verification) URL.
  $form['web2pay_mns_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Flo2Cash MNS (verification) URL'),
    '#description' => t('Flo2Cash hosted page URL where the verification request is to be sent. <strong>Demo should use <em>https://demo.flo2cash.co.nz/web2pay/MNSHandler.aspx</em> and live should use <em>https://secure.flo2cash.co.nz/web2pay/MNSHandler.aspx</em>.</strong>'),
    '#default_value' => $settings['web2pay_mns_url'],
    '#required' => TRUE,
  );

  // Flo2Cash Account ID.
  $form['flo2cash_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Flo2Cash Account ID'),
    '#description' => t('Your account ID issued by Flo2Cash.'),
    '#default_value' => $settings['flo2cash_account_id'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements CALLBACK_commerce_payment_method_redirect_form().
 */
function commerce_flo2cash_redirect_form($form, &$form_state, $order, $payment_method) {
  $base_path = 'checkout/' . $order->order_id . '/flo2cash';
  $key = $order->data['payment_redirect_key'];
  $options = array(
    'absolute' => TRUE,
  );

  // Generate the Merchant Notification Service and return URLs.
  $settings = $payment_method['settings'] + array(
    'notification_url' => url($base_path . '/mnscheck/' . $key, $options),
    'return_url' => url($base_path . '/return/' . $key, $options),
  );

  return commerce_flo2cash_order_form($form, $form_state, $order, $settings);
}

/**
 * Builds a Flo2Cash integration payment form from an order object.
 */
function commerce_flo2cash_order_form($form, &$form_state, $order, $settings) {
  $i = 1;
  foreach ($order->commerce_line_items[LANGUAGE_NONE] as $value) {
    $line_item = commerce_line_item_load($value['line_item_id']);

    if (isset($line_item->data['context']['entity'])) {
      // Get the node title if we can find one.
      $product_id = $line_item->data['context']['entity']['entity_id'];
      $node = node_load($product_id);
      $title = $node->title;
    }
    else {
      $title = $line_item->line_item_label;
    }
    $product_quantity = $line_item->quantity;
    $quantity = round($product_quantity);
    $amount = $line_item->commerce_unit_price[LANGUAGE_NONE][0]['amount'];
    $price = $amount / 100;
    $data['item_name' . $i] = $title;
    $data['item_code' . $i] = $value['line_item_id'];
    $data['item_price' . $i] = $price;
    $data['item_qty' . $i] = $quantity;
    $i++;
  }

  // Build the data array that will be used as hidden values
  // in the form and later sent to Flo2Cash.
  $data['cmd'] = '_xcart';
  $data['account_id'] = $settings['flo2cash_account_id'];
  $data['notification_url'] = $settings['notification_url'];
  $data['return_url'] = $settings['return_url'];
  $data['reference'] = $order->order_id;
  $data['particular'] = $order->order_id;

  foreach ($data as $name => $value) {
    $form[$name] = array(
      '#type' => 'hidden',
      '#value' => $value,
    );
  }

  $form['#action'] = $settings['web2pay_url'];
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to external payment gateway'),
  );
  $form['#after_build'][] = 'commerce_flo2cash_checkout_review_form_modify';

  return $form;
}

/**
 * Remove some Drupal form additions from external form.
 *
 * @see http://drupal.org/node/821932#comment-6017528
 */
function commerce_flo2cash_checkout_review_form_modify($form) {
  unset($form['form_token']);
  unset($form['form_build_id']);
  unset($form['form_id']);

  return $form;
}
