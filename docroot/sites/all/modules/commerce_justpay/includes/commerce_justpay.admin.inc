<?php

/**
 * @file
 * Administrative callbacks for the Commerce Justpay module.
 */

/**
 * Callback for config form.
 */
function commerce_justpay_admin_form($form, &$form_state) {

  $form['commerce_justpay_sku'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_justpay_sku', 'JUSTPAYMENT'),
    '#title' => t('Dummy SKU'),
    '#description' => t('Enter the SKU of a product for which to base the one-off payments on'),
  );

  $form['commerce_justpay_submit_text'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_justpay_submit_text', t('Pay')),
    '#title' => t('Form submit button text'),
  );

  foreach (field_info_instances('commerce_order', 'commerce_order') as $field_name => $info) {
    $options[$field_name] = $info['label'];
  }

  $form['commerce_justpay_exlude_order_fields'] = array(
    '#title' => t('Exclude these fields from the Justpay order form'),
    '#description' => t('NOTE: email and amount fields are always required, line items and order total should always remain excluded, some payment methods require the billing information'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('commerce_justpay_exlude_order_fields', array('commerce_line_items', 'commerce_order_total')),
  );

  return system_settings_form($form);
}
