<?php

/**
 * Checkout pane view callback.
 */
function uc_shipping_insurance_checkout_pane_view(&$order, $form = NULL, &$form_state = NULL) {
  $label = check_plain(variable_get('uc_shipping_insurance_checkout_pane_label', t('Add shipping insurance to my order')));
  $price = uc_currency_format(_uc_shipping_insurance_calculate($order));
  $contents['shipping_insurance'] = array(
    '#type' => 'checkbox',
    '#title' => $label . ': ' . $price,
    '#default_value' => _uc_shipping_insurance_order_has_insurance($order),
    '#ajax' => array(
      'callback' => 'uc_shipping_insurance_update_line_items',
      'effect' => 'fade',
    ),
  );
  return array(
    'contents' => $contents,
    'description' => nl2br(filter_xss_admin(variable_get('uc_shipping_insurance_checkout_pane_description', ''))),
  );
}

/**
 * Checkout pane process callback.
 */
function uc_shipping_insurance_checkout_pane_process(&$order, $form = NULL, &$form_state = NULL) {
  $line_items = _uc_shipping_insurance_line_items($order);
  if ($form_state['values']['panes']['shipping_insurance']['shipping_insurance'] == 1) {
    // Add shipping insurance line item if it doesn't already exist.
    if (empty($line_items)) {
      uc_order_line_item_add($order->order_id, 'shipping_insurance', 'Shipping Insurance', _uc_shipping_insurance_calculate($order));
    }
  }
  else {
    // Remove all shipping insurance line items.
    foreach ($line_items as $line_item) {
      uc_order_delete_line_item($line_item['line_item_id']);
    }
  }
}

/**
 * Checkout pane settings callback.
 */
function uc_shipping_insurance_checkout_pane_settings(&$order, $form = NULL, &$form_state = NULL) {
  $form = array(
    'uc_shipping_insurance_checkout_pane_title' => array(
      '#title' => t('Pane title'),
      '#type' => 'textfield',
      '#maxlength' => 60,
      '#default_value' => variable_get('uc_shipping_insurance_checkout_pane_title', t('Shipping insurance')),
      '#description' => t('Enter the checkout pane title.'),
    ),
    'uc_shipping_insurance_checkout_pane_description' => array(
      '#title' => t('Description'),
      '#type' => 'textarea',
      '#rows' => 5,
      '#default_value' => variable_get('uc_shipping_insurance_checkout_pane_description', ''),
      '#description' => t('Enter a description to appear on this checkout pane.'),
    ),
    'uc_shipping_insurance_checkout_pane_label' => array(
      '#title' => t('Checkbox label'),
      '#type' => 'textfield',
      '#maxlength' => 60,
      '#default_value' => variable_get('uc_shipping_insurance_checkout_pane_label', t('Add shipping insurance to my order')),
      '#description' => t('Enter the label to use for the shipping insurance option.'),
    ),
    'uc_shipping_insurance_checkout_pane_calculation_mode' => array(
      '#title' => t('Calculation mode'),
      '#type' => 'radios',
      '#options' => array(
        'percentage' => t('Percentage'),
        'fixed' => t('Fixed'),
      ),
      '#default_value' => variable_get('uc_shipping_insurance_checkout_pane_calculation_mode', 'percentage'),
    ),
  );
  $form['uc_shipping_insurance_checkout_pane_amount'] = array(
    '#title' => 'Amount',
    '#type' => 'textfield',
    '#field_prefix' => '',
    '#field_suffix' => '',
    '#size' => 5,
    '#maxlength' => 10,
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('uc_shipping_insurance_checkout_pane_amount', 1.5),
    '#description' => t('The amount or percentage amount to apply to the order'),
    '#attached' => array(
      'js' => array(
        array(
          'data' => array(
            'uc_shipping_insurance' => array(
              'amount_sign' => variable_get('uc_currency_sign', '$'),
              'sign_after' => variable_get('uc_sign_after_amount', FALSE),
            )
          ),
          'type' => 'setting',
        ),
        array(
          'data' => drupal_get_path('module', 'uc_shipping_insurance') . '/uc_shipping_insurance.js',
          'type' => 'file',
        ),
      ),
    ),
  );
  return $form;
}

/**
 * AJAX callback for the checkout form.
 */
function uc_shipping_insurance_update_line_items($form, $form_state) {
  $commands = array();
  if (isset($form['panes']['payment']['line_items'])) {
    $commands[] = ajax_command_replace('#line-items-div', drupal_render($form['panes']['payment']['line_items']));
    $commands[] = ajax_command_prepend('#line-items-div', theme('status_messages'));
  }
  return array('#type' => 'ajax', '#commands' => $commands);
}
