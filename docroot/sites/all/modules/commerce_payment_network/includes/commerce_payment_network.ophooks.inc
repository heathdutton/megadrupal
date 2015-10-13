<?php
/**
 * @file
 * Callback functions for Payment Network payment method operations.
 */

/**
 * Payment method callback: settings form.
 *
 * Returns form elements for the payment method settings form included
 * as part of the payment methods enabling action in Rules.
 */
function commerce_payment_network_settings_form($settings = NULL) {
  $form = array();

  // German URL: 'https://www.sofortueberweisung.de/payment/start'
  $soforturl = 'https://www.sofort.com/payment/start';

  $settings = (array) $settings + array(
    'soforturl' => '',
    'userid' => '',
    'projectid' => '',
    'project_pass' => '',
    'notification_pass' => '',
    'checkout_inform_image' => ''
  );

  // BASIC settings.
  $form['soforturl'] = array(
    '#type' => 'textfield',
    '#title' => t('!payment_network URL', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)),
    '#description' => t('The redirect URL to !payment_network. Leave blank to enter the default URL.', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)),
    '#default_value' => !empty($settings['soforturl']) ? $settings['soforturl'] : $soforturl
  );
  $form['userid'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#description' => t('Your user ID from !payment_network, find in your !payment_network backoffice.', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)),
    '#default_value' => $settings['userid']
  );
  $form['projectid'] = array(
    '#type' => 'textfield',
    '#title' => t('Project ID'),
    '#description' => t('The ID from your !payment_network project, find in your !payment_network backoffice.', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)),
    '#default_value' => $settings['projectid']
  );

  // SECURITY settings.
  $form['project_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Project password'),
    '#description' => t('Your !payment_network project password, find in your !payment_network backoffice', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)),
    '#default_value' => $settings['project_pass']
  );
  $form['notification_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification password'),
    '#description' => t('The !payment_network notification password, find in your !payment_network backoffice.', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)),
    '#default_value' => $settings['notification_pass']
  );

  $form['checkout_inform_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the information image'),
    '#description' => t('Hide the !payment_network image on the checkout pages. If enabled, will displayed only a text information.', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)),
    '#default_value' => $settings['checkout_inform_image']
  );

  return $form;
}

/**
 * Payment method callback: checkout form.
 */
function commerce_payment_network_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  $form['bank_details'] = array(
    '#markup' => '<div class="commerce-payment-network-checkout-info">' . t('(Continue with checkout to complete payment via !payment_network.)', array('!payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)) . '</div>'
  );

  return $form;
}

/**
 * Payment method callback: redirect form
 *
 * Returns form elements that should be submitted to the redirected
 * payment service.
 */
function commerce_payment_network_redirect_form($form, &$form_state, $order, $payment_method) {
  // Return an error if the enabling action's settings haven't been configured.
  if (empty($payment_method['settings']['userid']) || empty($payment_method['settings']['projectid'])) {
    drupal_set_message(t('@payment_network is not configured for use. No user or project ID has been specified.', array('@payment_network' => COMMERCE_PAYMENT_NETWORK_NAME)), 'error');
    watchdog('Commerce Payment Network', '%payment_network is not configured for use. No user or project ID has been specified.', array('%payment_network' => COMMERCE_PAYMENT_NETWORK_NAME), WATCHDOG_ERROR);

    return array();
  }

  return commerce_payment_network_build_redirect_form($form, $form_state, $order, $payment_method['settings']);
}

