<?php
/**
 * @file
 * Contains all code for the gift wrap pane. Called from
 *  commerce_giftwrap_commerce_checkout_pane_info()
 */

/**
 * Implements base_settings_form().
 */
function commerce_giftwrap_pane_settings_form($checkout_pane) {
  // Load the default currency for this instance.
  $default_currency = commerce_default_currency();
  $currency = commerce_currency_load($default_currency);
  // Round the default value.
  $default_amount = commerce_currency_amount_to_decimal(
      variable_get('commerce_giftwrap_price', '200'),
      $currency['code']
  );
  $form = array();
  $form['commerce_giftwrap_price'] = array(
    '#type' => 'textfield',
    '#title' => t('Gift Wrapping Price'),
    '#size' => 10,
    '#default_value' => $default_amount,
    '#field_suffix' => $currency['code'],
  );
  $form['commerce_giftwrap_additional_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Gift Wrapping Information'),
    '#description' => t('Add some more infomation about the Gift Wrapping service.'),
    '#default_value' => variable_get('commerce_giftwrap_additional_info', ''),
  );
  $form['commerce_giftwrap_show_message'] = array(
    '#type' => 'radios',
    '#title' => t('Allow the user to add a message?'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => variable_get('commerce_giftwrap_show_message', '1'),
  );
  $form['commerce_giftwrap_limit_message'] = array(
    '#type' => 'textfield',
    '#title' => t("Limit the length of the user's message"),
    '#size' => 5,
    '#default_value' => variable_get('commerce_giftwrap_limit_message', '60'),
  );
  return $form;
}

/**
 * Custom submit function to handle the commerce_giftwrap_price.
 */
function commerce_giftwrap_pane_settings_form_submit($form, &$form_state) {
  // Load the default currency for this instance.
  $default_currency = commerce_default_currency();
  $currency = commerce_currency_load($default_currency);
  // Get the decimal price.
  $decimal_price = $form_state['values']['commerce_giftwrap_price'];
  // Convert decimal price to amount for storage.
  $amount_price = commerce_currency_decimal_to_amount($decimal_price, $currency['code']);
  variable_set('commerce_giftwrap_price', $amount_price);
}

/**
 * Implements base_checkout_form().
 */
function commerce_giftwrap_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  // Set $default_value
  if (commerce_giftwrap_line_item_exists($order)) {
    $default_giftwrap_decision = 1;
    $default_giftwrap_message = commerce_giftwrap_get_message($order);
  }
  else {
    $default_giftwrap_decision = 0;
    $default_giftwrap_message = '';
  }
  $checkout_form['commerce_giftwrap_decision'] = array(
    '#type' => 'select',
    '#title' => t('Would you like your order gift wrapped?'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => $default_giftwrap_decision,
  );
  if (variable_get('commerce_giftwrap_additional_info', '') != '') {
    $checkout_form['commerce_giftwrap_additional_info'] = array(
      '#markup' => '<div class="giftwrap-additonal-info"><p>'
      . filter_xss(variable_get('commerce_giftwrap_additional_info', '')) .
      '</p></div>',
    );
  }
  if (variable_get('commerce_giftwrap_show_message', '1')) {
    $checkout_form['commerce_giftwrap_message'] = array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#description' => t('Add a personal message to be included with your gift. Your message must be no longer than @char characters.',
                            array('@char' => variable_get('commerce_giftwrap_limit_message', '60'))),
      '#maxlength' => variable_get('commerce_giftwrap_limit_message', '60'),
      '#default_value' => $default_giftwrap_message,
      '#states' => array(
        'visible' => array(
          ':input[name="commerce_giftwrap[commerce_giftwrap_decision]"]' => array('value' => '1'),
        ),
      ),
    );
  }
  return $checkout_form;
}

/**
 * Implements base_checkout_form_submit().
 */
function commerce_giftwrap_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $giftwrap_line_item_exists = commerce_giftwrap_line_item_exists($order);
  $giftwrap_decision = $form_state['values']['commerce_giftwrap']['commerce_giftwrap_decision'];
  $giftwrap_message = isset($form_state['values']['commerce_giftwrap']['commerce_giftwrap_message']) ?
    filter_xss($form_state['values']['commerce_giftwrap']['commerce_giftwrap_message'], array()) : NULL;
  if ($giftwrap_decision) {
    if ($giftwrap_line_item_exists && $giftwrap_message != commerce_giftwrap_get_message($order)) {
      // Update the line item if message has been changed.
      $line_item_id = commerce_giftwrap_get_line_item_id($order);
      $line_item = commerce_line_item_load($line_item_id);
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      $line_item_wrapper->commerce_giftwrap_message = $giftwrap_message;
      // Save the line item..
      commerce_line_item_save($line_item);
    }
    elseif (!$giftwrap_line_item_exists) {
      // Set the currency code.
      $default_currency_code = commerce_default_currency();
      if ($balance = commerce_payment_order_balance($order)) {
        $default_currency_code = $balance['currency_code'];
      }
      // Create the new line item.
      $line_item = commerce_line_item_new('giftwrap', $order->order_id);
      // Wrap the line item and order to simplify manipulating their field data.
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      // Populate the $line_item_wrapper...
      $line_item_wrapper->line_item_label = t('Gift Wrapping');
      $line_item_wrapper->commerce_giftwrap_message = $giftwrap_message;
      $line_item_wrapper->quantity = 1;
      $line_item_wrapper->commerce_unit_price->amount = variable_get('commerce_giftwrap_price', '200');
      $line_item_wrapper->commerce_unit_price->currency_code = $default_currency_code;
      // Set the price component of the unit price.
      $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
        $line_item_wrapper->commerce_unit_price->value(),
        'giftwrap',
        $line_item_wrapper->commerce_unit_price->value(),
        TRUE,
        FALSE
      );
      // Invoke the 'commerce_giftwrap_calculate' event.
      rules_invoke_all('commerce_giftwrap_calculate', $line_item);
      // Save the incoming line item now so we get its ID.
      commerce_line_item_save($line_item);
      // Add it to the order's line item reference value.
      $order_wrapper->commerce_line_items[] = $line_item;
    }
    // Save the order.
    commerce_order_save($order);
  }
  elseif (!$giftwrap_decision) {
    // If the user selects 'No' check if giftwrapping has been added, if so
    // delete it.
    commerce_giftwrap_delete_giftwrap_line_items($order);
  }
}

/**
 * Implements base_review().
 */
function commerce_giftwrap_pane_review($form, &$form_state, $checkout_pane, $order) {
  $message = commerce_giftwrap_get_message($order);
  if (commerce_giftwrap_line_item_exists($order)) {
    $output = array(
      'giftwrap_decision' => array(
        '#type' => 'markup',
        '#markup' => '<h3>' . t('Would you like your order gift wrapped?') . ' <strong>' . t('Yes') . '</strong></h3>',
      ),
      'title' => array(
        '#type' => 'markup',
        '#markup' => '<h3>' . t('Message:') . '</h3>',
      ),
      'giftwrap_message' => array(
        '#type' => 'markup',
        '#markup' => '<p>' . filter_xss($message) . '</p>',
      ),
    );
  }
  else {
    $output['giftwrap_decision'] = array(
      '#type' => 'markup',
      '#markup' => '<h3>' . t('Would you like your order gift wrapped?') . '<strong>' . t('No') . '</strong></h3>',
    );
  }
  return drupal_render($output);
}
