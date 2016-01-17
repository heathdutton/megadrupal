<?php

/**
 * @file   commerce_order_counter.admin.inc
 * @author Augustin Delaporte <guss.tissier@gmail.com>
 *         António P. P. Almeida <appa@perusio.net>
 * @date   Thu Mar  7 10:51:59 2013
 *
 * @brief  Settings form handling for the commerce_order_counter.
 *
 */

/**
 * Settings form definition page callback.
 */
function commerce_order_counter_settings_form() {
  $form = array();

  $form['commerce_order_counter_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Order Counter Base'),
    '#description' => t('The value being used as the base for the commerce order numbering: assumed to be a <strong>positive integer</strong>.'),
    '#default_value' => variable_get('commerce_order_counter_value',
                                     COMMERCE_ORDER_COUNTER_DEFAULT_BASE_VALUE),
    '#required' => TRUE,
    '#maxlength' => 60,
    '#size' => 20,
    '#element_validate' => array(
      'element_validate_integer_positive',
    ),
  );

  return system_settings_form($form);
} // commerce_order_counter_settings_form
