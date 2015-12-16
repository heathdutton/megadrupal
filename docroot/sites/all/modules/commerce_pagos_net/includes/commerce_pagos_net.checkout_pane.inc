<?php


/**
 * Checkout pane callback: returns the settings form elements for the checkout
 * completion message.
 */


function commerce_pagos_net_pane_settings_form($checkout_pane) {
  $form = array();

  $message = variable_get('pagos_net_completion_message', commerce_checkout_completion_message_default());

  $form['container'] = array(
    '#type' => 'container',
    '#access' => filter_access(filter_format_load($message['format'])),
  );
  $form['container']['pagos_net_completion_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Pagos Net Checkout completion message'),
    '#default_value' => $message['value'],
    '#format' => $message['format'],
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
 * Checkout pane callback: presents a completion message on the complete page.
 */
function commerce_pagos_net_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  // Load the completion message.
  $message = variable_get('pagos_net_completion_message', commerce_checkout_completion_message_default());

  // Perform translation.
  $message['value'] = commerce_i18n_string('commerce:checkout:complete:message', $message['value'], array('sanitize' => FALSE));

  // Perform token replacement.
  $message['value'] = token_replace($message['value'], array('commerce-order' => $order), array('clear' => TRUE));

  // Apply the proper text format.
  $message['value'] = check_markup($message['value'], $message['format']);

  $pane_form['message'] = array(
    '#markup' => '<div class="checkout-completion-message">' . $message['value'] . '</div>',
  );

  return $pane_form;
}





/**
Implements base_settings_form()
 */
//function commerce_pagos_net_base_settings_form($checkout_pane) {
//  $form['pagos_net_pane_field'] = array(
//    '#type' => 'textfield',
//    '#title' => t('My Module Pane Field'),
//    '#default_value' => variable_get('pagos_net_pane_field', ''),
//  );
//  return $form;
//}
