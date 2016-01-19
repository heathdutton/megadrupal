<?php

/**
 * Payment method callback: settings form.
 */
function commerce_robokassa_settings_form($settings = NULL) {
  $form = array();

  $settings = (array) $settings + array(
    'login' => '',
    'pass1' => '',
    'pass2' => '',
    'status' => array('pending'),
    'success_message' => '',
    'fail_message' => '',
  );

  $form['login'] = array(
    '#type' => 'textfield',
    '#title' => t('login'),
    '#description' => t(' Your robokassa login'),
    '#default_value' => $settings['login'],
  );

  $form['pass1'] = array(
    '#type' => 'textfield',
    '#title' => t('First password'),
    '#description' => t(' Password 1'),
    '#default_value' => $settings['pass1'],
  );

  $form['pass2'] = array(
    '#type' => 'textfield',
    '#title' => t('Second password'),
    '#description' => t(' Password 2'),
    '#default_value' => $settings['pass2'],
  );

  $form['server'] = array(
    '#type' => 'radios',
    '#title' => t('Robokassa server'),
    '#options' => array(
      'test' => ('Test - use for testing.'),
      'live' => ('Live - use for processing real transactions'),
    ),
    '#default_value' => $settings['server'],
  );

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => t('Choose order status after customer sent the order'),
    '#options' => commerce_robokassa_statuses(),
    '#default_value' => $settings['status'],
  );

   $form['success_message'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#title' => t('Success message'),
    '#description' => t('Enter optional text that will be displayed when payment succesful'),
    '#default_value' => $settings['success_message'],
  );

  $form['fail_message'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#title' => t('Fail message'),
    '#description' => t('Enter optional text that will be displayed when payment fail'),
    '#default_value' => $settings['fail_message'],
  );

  $form['#submit'][] = variable_set('commerce_robokassa_settings', $form);

  return $form;
}
