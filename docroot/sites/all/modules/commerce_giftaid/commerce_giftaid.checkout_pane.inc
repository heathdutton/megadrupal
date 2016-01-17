<?php

/**
 * @file
 * Checkout pane callback functions for the gift aid module.
 */

/**
 * Checkout pane callback: returns the customer profile pane's settings form.
 */
function commerce_giftaid_pane_settings_form($checkout_pane) {
  $form = array();

  $form['commerce_giftaid_pane_accept'] = array(
    '#type' => 'textarea',
    '#title' => t('Giftaid accept declaration'),
    '#description' => t('Specify the gift aid declaration the user can tick to agree to.'),
    '#default_value' => variable_get('commerce_giftaid_pane_accept', t('I confirm I have paid or will pay an amount of Income Tax and/or Capital
    Gains Tax for the current tax year (6 April to 5 April) that is at least equal to
    the amount of tax that all the charities and Community Amateur Sports Clubs
    (CASCs) that I donate to will reclaim on my gifts for the current tax year. I
    understand that other taxes such as VAT and Council Tax do not qualify. I
    understand the charity will reclaim 25p of tax on every £1 that I have given.')),
  );

  $form['commerce_giftaid_pane_decline'] = array(
    '#type' => 'textarea',
    '#title' => t('Giftaid decline declaration'),
    '#description' => t('Specify the gift aid declaration the user can tick to agree to.'),
    '#default_value' => variable_get('commerce_giftaid_pane_decline', t('I do not meet the gift aid criteria.')),
  );

  $form['commerce_giftaid_pane_prefix'] = array(
    '#type' => 'textarea',
    '#title' => t('Giftaid introduction'),
    '#description' => t('Specify a paragraph of text to appear above the gift aid choices.'),
    '#default_value' => variable_get('commerce_giftaid_pane_prefix', ''),
  );

  $form['commerce_giftaid_pane_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Giftaid description'),
    '#description' => t('Specify some description text which will appear below the gift aid declaration.'),
    '#default_value' => variable_get('commerce_giftaid_pane_description', t('For more information about giftaid please !url', array('!url' => l('read this page', 'http://www.hmrc.gov.uk/individuals/giving/gift-aid.htm')))),
  );

  $form['commerce_giftaid_pane_error'] = array(
    '#type' => 'textarea',
    '#title' => t('Giftaid error message'),
    '#description' => t('Specify an error message to display if no choice is made.'),
    '#default_value' => _commerce_giftaid_pane_error_message(),
  );

  return $form;
}

/**
 * Checkout pane callback: returns a giftaid edit form if the order
 * has items which are eligable for gift aid.
 */
function commerce_giftaid_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $default = 0;
  if (isset($order->data['commerce_giftaid'])) {
    $default = $order->data['commerce_giftaid'];
  }
  else {
    $defaults = module_invoke_all('commerce_giftaid_user_default', $order->uid);
    if (!empty($defaults)) {
      $default = reset($defaults);
    }
  }

  if (commerce_giftaid_order_contains_eligible_items($order)) {
    $def = commerce_giftaid_pane_settings_form($checkout_pane);
    $giftaid_form['commerce_giftaid'] = array(
      '#type' => 'radios',
      '#options' => array(
        1 => variable_get('commerce_giftaid_pane_accept', $def['commerce_giftaid_pane_accept']['#default_value']),
        0 => variable_get('commerce_giftaid_pane_decline', $def['commerce_giftaid_pane_decline']['#default_value']),
      ),
      '#default_value' => $default,
      '#prefix' => variable_get('commerce_giftaid_pane_prefix', $def['commerce_giftaid_pane_prefix']['#default_value']),
      '#description' => variable_get('commerce_giftaid_pane_description', $def['commerce_giftaid_pane_description']['#default_value']),
    );

    return $giftaid_form;
  }
}

/**
 * Get the pane error message.
 *
 * @return string
 */
function _commerce_giftaid_pane_error_message() {
  return variable_get('commerce_giftaid_pane_error', t('Please choose whether we can claim Gift aid.'));
}

/**
 * Checkout pane callback: validates a giftaid edit form.
 */
function commerce_giftaid_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  if (isset($form_state['values']['commerce_giftaid_pane'])) {
    $value = $form_state['values']['commerce_giftaid_pane']['commerce_giftaid'];

    if (!in_array($value, array('0', '1'), TRUE)) {
      form_set_error('commerce_giftaid', _commerce_giftaid_pane_error_message());
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Checkout pane callback: submits a giftaid edit form.
 */
function commerce_giftaid_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $values = $form_state['values'];
  if (isset($values['commerce_giftaid_pane'])) {
    $order->data['commerce_giftaid'] = $values['commerce_giftaid_pane']['commerce_giftaid'];
  }
  else {
    $order->data['commerce_giftaid'] = 0;
  }
}

/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 */
function commerce_giftaid_pane_review($form, $form_state, $checkout_pane, $order) {
  if (commerce_giftaid_order_contains_eligible_items($order)) {
    return $order->data['commerce_giftaid'] ? t('Gift aid declaration accepted') : t('Gift aid declaration not accepted');
  }
}
