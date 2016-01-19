<?php

/**
 * @file
 * Admin settings form for commerce Order Cleanup module.
 */

/**
 * Commerce order cleanup settings form.
 **/
function commerce_order_cleanup_settings() {
  $form = array();
  $form['commerce_order_cleanup_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Batches size of bulk delete'),
    '#default_value' => variable_get('commerce_order_cleanup_batch_size', 100),
    '#description' => t('Orders will be deleted as a small batches to avoid timeout.'),
  );

  return system_settings_form($form);
}
