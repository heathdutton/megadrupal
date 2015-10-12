<?php

/**
 * @file
 * Forms for the commerce_sofortbanking module.
 */

/**
 * Builds a form array for Drupal's form API which contains all settings
 * for the shop administrator for the payment method.
 *
 * @param array $settings
 *   Includes all the settings about the payment method.
 * @return array
 */
function commerce_sofortbanking_project_form($settings) {
  $form = array();

  $settings = drupal_array_merge_deep(array(
    'config_key' => '',
    'extended' => array(
      'checkout_display_mode' => 'banner',
      'order_subject_1' => 'Order {{order_id}}',
      'order_subject_2' => '',
      'options' => array(
        'customer_protection' => FALSE,
        'recommended' => TRUE,
        'logging' => FALSE,
      ),
      'states' => array(
        'cancel' => 'failure',
        'untraceable' => 'success',
        'pending' => 'success',
        'received' => 'success',
        'loss' => 'pending',
        'refunded' => 'success',
      ),
    ),
  ), $settings);

  $form['commerce_sofortbanking_info'] = array(
    '#markup' => '<div>' . sofortbanking_strings(1602, array('!url' => 'https://www.sofort.com/payment/users/register')) . '</div>',
  );

  $form['config_key'] = array(
    '#type' => 'textfield',
    '#title' => sofortbanking_strings(1606),
    '#default_value' => $settings['config_key'],
    '#description' => sofortbanking_strings(1607),
  );

  $form['extended'] = array(
    '#type' => 'fieldset',
    '#title' => sofortbanking_strings(1611),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['extended']['commerce_sofortbanking_info'] = array(
    '#value' => '<div>' . sofortbanking_strings(1612) . '</div>',
  );
  $form['extended']['checkout_display_mode'] = array(
    '#type' => 'radios',
    '#title' => sofortbanking_strings(1613),
    '#options' => array(
      'banner' => sofortbanking_strings(1615),
      'description' => sofortbanking_strings(1616),
    ),
    '#default_value' => $settings['extended']['checkout_display_mode'],
    '#description' => sofortbanking_strings(1614),
  );
  $form['extended']['order_subject_1'] = array(
    '#type' => 'textfield',
    '#title' => sofortbanking_strings(1623),
    '#default_value' => $settings['extended']['order_subject_1'],
    '#description' => sofortbanking_strings(1624),
  );
  $form['extended']['order_subject_2'] = array(
    '#type' => 'textfield',
    '#title' => sofortbanking_strings(1625),
    '#default_value' => $settings['extended']['order_subject_2'],
    '#description' => sofortbanking_strings(1624),
  );
  $form['extended']['options'] = array(
    '#type' => 'fieldset',
    '#title' => sofortbanking_strings(9002),
    '#collapsible' => FALSE,
  );
  $form['extended']['options']['customer_protection'] = array(
    '#type' => 'checkbox',
    '#title' => sofortbanking_strings(1617),
    '#default_value' => $settings['extended']['options']['customer_protection'],
    '#description' => sofortbanking_strings(1618),
  );
  $form['extended']['options']['recommended'] = array(
    '#type' => 'checkbox',
    '#title' => sofortbanking_strings(1619),
    '#default_value' => $settings['extended']['options']['recommended'],
    '#description' => sofortbanking_strings(1620),
  );
  $form['extended']['options']['logging'] = array(
    '#type' => 'checkbox',
    '#title' => sofortbanking_strings(1630),
    '#default_value' => $settings['extended']['options']['logging'],
    '#description' => sofortbanking_strings(1631),
  );
  $form['extended']['states'] = array(
    '#type' => 'fieldset',
    '#title' => sofortbanking_strings(9003),
    '#collapsible' => FALSE,
  );
  $form['extended']['states']['commerce_sofortbanking_info'] = array(
    '#value' => '<div>' . sofortbanking_strings(1632) . '</div>',
  );
  $options = commerce_payment_transaction_status_options_list();
  $form['extended']['states']['cancel'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1639),
    '#options' => $options,
    '#default_value' => $settings['extended']['states']['cancel'],
    '#description' => sofortbanking_strings(1640),
  );
  $form['extended']['states']['untraceable'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1635),
    '#options' => $options,
    '#default_value' => $settings['extended']['states']['untraceable'],
    '#description' => sofortbanking_strings(1636),
  );
  $form['extended']['states']['pending'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(9004),
    '#options' => $options,
    '#default_value' => $settings['extended']['states']['pending'],
    '#description' => sofortbanking_strings(9005),
  );
  $form['extended']['states']['received'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1637),
    '#options' => $options,
    '#default_value' => $settings['extended']['states']['received'],
    '#description' => sofortbanking_strings(1638),
  );
  $form['extended']['states']['loss'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1641),
    '#options' => $options,
    '#default_value' => $settings['extended']['states']['loss'],
    '#description' => sofortbanking_strings(1642),
  );
  $form['extended']['states']['refunded'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1645),
    '#options' => $options,
    '#default_value' => $settings['extended']['states']['refunded'],
    '#description' => sofortbanking_strings(1646),
  );

  return $form;
}
