<?php
/**
 * @file
 * The commerce rules checkout pane.
 */

/**
 * Checkout pane callback: returns the checkout rules pane's settings form.
 */
function commerce_rules_pane_settings_form($checkout_pane) {
  $form = array();

  $form['commerce_rules_pane_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Pane title.'),
    '#default_value' => variable_get('commerce_rules_pane_title', t('Rules checkout pane')),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Checkout pane callback: presents pane which content is set via rules.
 */
function commerce_rules_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {

  // If the pane is empty, disable it.
  if (empty($order->data['checkout_messages'])) {
    $checkout_pane['enabled'] = FALSE;
  }

  // Invoke the determine rules checkout pane content event that populate the
  // pane with content.
  rules_invoke_all('determine_rules_checkout_pane_content', $order);

  // Create the pane form.
  $pane_form = array();
  if (!empty($order->data['checkout_messages'])) {
    foreach ($order->data['checkout_messages'] as $message) {
      $pane_form['content'][] = array(
        '#tag' => 'div',
        '#theme' => 'html_tag',
        '#attributes' => array(
          'class' => 'commerce-rules-pane-message',
        ),
        '#value' => drupal_render($message),
      );
    }
  }

  return $pane_form;
}
