<?php

/**
 * @file
 * The default format for adresses.
 */

$plugin = array(
  'title' => t('Payumoney Phone Field'),
  'format callback' => 'commerce_payumoney_pmphone_plugins_format_pmphone_addressfield_format_generate',
  'type' => 'pmphone',
  'weight' => 99,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function commerce_payumoney_pmphone_plugins_format_pmphone_addressfield_format_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'form') {
    $format['pmphone_block'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('custom-block')),
      '#weight' => 200,
    );
    $format['pmphone_block']['pm_phone'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('addressfield-container-inline')),
    );
    $format['pmphone_block']['pm_phone'] = array(
      '#title' => t('Phone'),
      '#size' => 15,
      '#required' => TRUE,
      '#attributes' => array('class' => array('pm-phone')),
      '#type' => 'textfield',
      '#tag' => 'span',
      '#default_value' => isset($address['pm_phone']) ? $address['pm_phone'] : '',
    );
  }
  else {
    // Add our own render callback for the format view.
    $format['#pre_render'][] = '_addressfield_pmphone_render_address';
  }
}
