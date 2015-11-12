<?php

/**
 * @file
 * Checkout pane callback functions for CAB.
 */

/**
 * Returns a summary text (as a form) for the checkout panes.
 */
function commerce_account_balance_checkout_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  $message = variable_get('commerce_account_balance_checkout_summary', commerce_account_balance_checkout_summary_default());

  // Perform translation.
  $message['value'] = commerce_i18n_string('commerce:account:balance:checkout:summary', $message['value'], array('sanitize' => FALSE));

  // Perform token replacement.
  $message['value'] = token_replace($message['value'], array('commerce-order' => $order), array('clear' => TRUE));

  // Apply the proper text format.
  $message['value'] = check_markup($message['value'], $message['format']);

  $pane_form['message'] = array(
    '#markup' => '<div class="checkout-account-balance-message">' . $message['value'] . '</div>',
  );

  return $pane_form;
}

/**
 * Returns a summary text for the checkout review pane.
 */
function commerce_account_balance_checkout_review($form, &$form_state, $checkout_pane, $order) {
  return commerce_account_balance_checkout_checkout_form($form, $form_state, $checkout_pane, $order);
}

/**
 * Callback for summary text on the checkout and review pages.
 */
function _commerce_account_balance_checkout_summary_text($order) {
  $pane_form = array();

  $message = variable_get('commerce_account_balance_checkout_summary', commerce_account_balance_checkout_summary_default());

  // Perform translation.
  $message['value'] = commerce_i18n_string('commerce:account:balance:checkout:summary', $message['value'], array('sanitize' => FALSE));

  // Perform token replacement.
  $message['value'] = token_replace($message['value'], array('commerce-order' => $order), array('clear' => TRUE));

  // Apply the proper text format.
  $message['value'] = check_markup($message['value'], $message['format']);

  $pane_form['message'] = array(
    '#markup' => '<div class="checkout-account-balance-message">' . $message['value'] . '</div>',
  );

  return $pane_form;
}

/**
 * Checkout pane callback: returns the shipping service pane's settings form.
 */
function commerce_account_balance_checkout_settings_form($checkout_pane) {
  $form = array();

  $default_message = variable_get('commerce_account_balance_checkout_summary',
    commerce_account_balance_checkout_summary_default()
  );

  $form['container'] = array(
    '#type' => 'container',
    '#access' => filter_access(filter_format_load($default_message['format'])),
  );

  $form['container']['commerce_account_balance_checkout_summary'] = array(
    '#type' => 'text_format',
    '#title' => t('Desposit summary message'),
    '#default_value' => $default_message['value'],
    '#format' => $default_message['format'],
  );

  $var_info = array(
    'site' => array(
      'type' => 'site',
      'label' => t('Site information'),
      'description' => t('Site-wide settings and other global information.'),
    ),
    'commerce_order' => array(
      'label' => t('Order'),
      'type' => 'commerce_order',
    ),
  );

  $form['container']['commerce_checkout_completion_message_help'] = RulesTokenEvaluator::help($var_info);

  return $form;
}

/**
 * Callback for the default value of the CAB checkout summary.
 */
function commerce_account_balance_checkout_summary_default() {
  $value = t('Your current balance is [commerce-order:balance-current].
    After a deposit of [commerce-order:deposit] your balance will be
    [commerce-order:balance-updated].');

  if (filter_format_load('filtered_html')) {
    $format = 'filtered_html';
  }
  else {
    $format = filter_fallback_format();
  }
  return array('value' => $value, 'format' => $format);
}
