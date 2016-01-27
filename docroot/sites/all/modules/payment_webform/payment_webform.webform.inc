<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_payment_webform() {
  return array(
    'extra' => array(
      'payment_currency_code' => 'XXX',
      'payment_description' => '',
      'payment_line_items' => array(),
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_payment_webform($component) {
  $form['extra']['payment_currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => payment_currency_options(),
    '#default_value' => $component['extra']['payment_currency_code'],
    '#required' => TRUE,
  );
  $form['extra']['payment_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $component['extra']['payment_description'],
    '#required' => TRUE,
  );
  $form['extra']['payment_line_items'] = array(
    '#type' => 'payment_line_item',
    '#title' => t('Line items'),
    '#cardinality' => 0,
    '#default_value' => $component['extra']['payment_line_items'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements _webform_render_[component]().
 */
function _webform_render_payment_webform($component, $value = NULL, $filter = TRUE) {
  global $user;

  $form = array();
  if ($user->uid) {
    $form = array(
      '#type' => 'paymentreference',
      '#title' => $component['name'],
      '#default_value' => is_array($value) ? reset($value) : 0,
      '#required' => $component['mandatory'],
      '#payment_line_items' => $component['extra']['payment_line_items'],
      '#payment_currency_code' => $component['extra']['payment_currency_code'],
      '#payment_add_page_path' => 'payment_webform/pay/' . $component['nid'] . '/' . $component['cid'],
      '#payment_load_callback' => 'payment_webform_load',
      '#payment_load_arguments' => array($component['cid'], $user->uid),
    );
  }
  elseif ($component['mandatory']) {
    drupal_set_message(t('You need to be logged in to submit this form.'), 'error');
    $form = array(
      '#required' => TRUE,
      '#title' => $component['name'],
      '#type' => 'value',
    );
  }

  return $form;
}

/**
 * Implements _webform_submit_[component]().
 */
function _webform_submit_payment_webform($component, $value) {
  payment_webform_delete_by_pid($value);

  return $value;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_payment_webform($component, $value, $format = 'html') {
  // The component only supports a single value.
  $pid = $value[0];
  $label = t('Payment !pid', array(
    '!pid' => $pid
  ));
  if ($format == 'html' && payment_access('view')) {
    $label = l($label, 'payment/' . $pid);
  }

  return array(
    '#component' => $component,
    '#format' => $format,
    '#markup' => $label,
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#title' => $component['name'],
    '#value' => $label,
    '#weight' => $component['weight'],
  );
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_payment_webform($component, $export_options) {
  $arguments = array(
    '@name' => $component['name'],
  );
  $header = array(
    t('@name (payment ID)', $arguments),
    t('@name (currency code)', $arguments),
    t('@name (amount)', $arguments),
  );

  return array(array(), array(), $header);
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_payment_webform($component, $export_options, $value) {
  $pid = $value[0];
  $payment = entity_load_single('payment', $pid);
  if ($payment) {
    $data = array($pid, $payment->currency_code, $payment->totalAmount(TRUE));
  }
  else {
    $data = array('', '', '');
  }

  return $data;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_payment_webform($component, $value) {
  $pid = $value[0];
  $label = t('Payment !pid', array(
    '!pid' => $pid
  ));
  if (payment_access('view')) {
    $label = l($label, 'payment/' . $pid);
  }

  return $label;
}
