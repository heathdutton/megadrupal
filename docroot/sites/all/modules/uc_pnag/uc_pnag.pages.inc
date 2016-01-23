<?php

/**
 * @file
 * Forms for the uc_pnag module.
 */

/**
 * Builds a form array for Drupal's form API which contains all settings
 * for the shop administrator for the payment method.
 *
 * @return array
 */
function uc_pnag_project_form() {
  $form = array();

  $form['uc_pnag_info'] = array(
    '#markup' => '<div>' . sofortbanking_strings(1602, array('!url' => 'https://www.sofort.com/payment/users/register')) . '</div>',
  );

  $form['uc_pnag_config_key'] = array(
    '#type' => 'textfield',
    '#title' => sofortbanking_strings(1606),
    '#default_value' => variable_get('uc_pnag_config_key', ''),
    '#description' => sofortbanking_strings(1607),
  );

  $form['extended'] = array(
    '#type' => 'fieldset',
    '#title' => sofortbanking_strings(1611),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['extended']['uc_pnag_info'] = array(
    '#value' => '<div>' . sofortbanking_strings(1612) . '</div>',
  );
  $form['extended']['uc_pnag_checkout_display_mode'] = array(
    '#type' => 'radios',
    '#title' => sofortbanking_strings(1613),
    '#options' => array(
      'banner' => sofortbanking_strings(1615),
      'description' => sofortbanking_strings(1616),
    ),
    '#default_value' => variable_get('uc_pnag_checkout_display_mode', 'banner'),
    '#description' => sofortbanking_strings(1614),
  );
  $form['extended']['uc_pnag_order_subject_1'] = array(
    '#type' => 'textfield',
    '#title' => sofortbanking_strings(1623),
    '#default_value' => variable_get('uc_pnag_order_subject_1', 'Order {{order_id}}'),
    '#description' => sofortbanking_strings(1624),
  );
  $form['extended']['uc_pnag_order_subject_2'] = array(
    '#type' => 'textfield',
    '#title' => sofortbanking_strings(1625),
    '#default_value' => variable_get('uc_pnag_order_subject_2', ''),
    '#description' => sofortbanking_strings(1624),
  );
  $form['extended']['options'] = array(
    '#type' => 'fieldset',
    '#title' => sofortbanking_strings(9002),
    '#collapsible' => FALSE,
  );
  $form['extended']['options']['uc_pnag_customer_protection'] = array(
    '#type' => 'checkbox',
    '#title' => sofortbanking_strings(1617),
    '#default_value' => variable_get('uc_pnag_customer_protection', FALSE),
    '#description' => sofortbanking_strings(1618),
  );
  $form['extended']['options']['uc_pnag_recommended'] = array(
    '#type' => 'checkbox',
    '#title' => sofortbanking_strings(1619),
    '#default_value' => variable_get('uc_pnag_recommended', TRUE),
    '#description' => sofortbanking_strings(1620),
  );
  $form['extended']['options']['uc_pnag_logging'] = array(
    '#type' => 'checkbox',
    '#title' => sofortbanking_strings(1630),
    '#default_value' => variable_get('uc_pnag_logging', FALSE),
    '#description' => sofortbanking_strings(1631),
  );
  $form['extended']['states'] = array(
    '#type' => 'fieldset',
    '#title' => sofortbanking_strings(9003),
    '#collapsible' => FALSE,
  );
  $form['extended']['states']['uc_pnag_info'] = array(
    '#value' => '<div>' . sofortbanking_strings(1632) . '</div>',
  );
  $options = array();
  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }
  $form['extended']['states']['uc_pnag_order_status_untraceable'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1635),
    '#options' => $options,
    '#default_value' => variable_get('uc_pnag_order_status_untraceable', 'payment_received'),
    '#description' => sofortbanking_strings(1636),
  );
  $form['extended']['states']['uc_pnag_order_status_pending'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(9004),
    '#options' => $options,
    '#default_value' => variable_get('uc_pnag_order_status_pending', 'payment_received'),
    '#description' => sofortbanking_strings(9005),
  );
  $form['extended']['states']['uc_pnag_order_status_payment_received'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1637),
    '#options' => $options,
    '#default_value' => variable_get('uc_pnag_order_status_payment_received', 'payment_received'),
    '#description' => sofortbanking_strings(1638),
  );
  $form['extended']['states']['uc_pnag_order_status_loss'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1641),
    '#options' => $options,
    '#default_value' => variable_get('uc_pnag_order_status_loss', 'pending'),
    '#description' => sofortbanking_strings(1642),
  );
  $form['extended']['states']['uc_pnag_order_status_refunded'] = array(
    '#type' => 'select',
    '#title' => sofortbanking_strings(1645),
    '#options' => $options,
    '#default_value' => variable_get('uc_pnag_order_status_refunded', 'payment_received'),
    '#description' => sofortbanking_strings(1646),
  );

  return $form;
}
