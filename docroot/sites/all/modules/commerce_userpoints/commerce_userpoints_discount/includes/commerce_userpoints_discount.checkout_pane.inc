<?php
/**
 * @file
 * Implements the different hook required by Drupal Commerce module to display specific Checkout Panes
 */


/**
 * Form implementation set in commerce_userpoints_discount_commerce_checkout_pane_info().
 */
function commerce_userpoints_discount_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $currencies = commerce_userpoints_currencies();

  if (empty($currencies)) {
    drupal_set_message(t('No !points currencies have been set up.', userpoints_translation()), 'error');
    return array();
  }

  $default_currency = reset($currencies);
  if (count($currencies) > 1) {
    $options = array();
    foreach ($currencies as $currency) {
      $options[$currency['code']] = $currency['name'];
    }

    $pane_form['currency'] = array(
      '#type' => 'select',
      '#title' => t('!Points currency', userpoints_translation()),
      '#options' => $options,
      '#default_value' => $default_currency['code'],
      '#ajax' => array(
        'callback' => '_commerce_userpoints_discount_ajax_update_info',
        'wrapper' => 'commerce-userpoints-widget-info',
      ),
    );
  }
  else {
    $pane_form['currency'] = array(
      '#type' => 'value',
      '#value' => $default_currency['code'],
    );
  }

  // Get current balance detils.
  if (isset($form_state['values']['commerce_userpoints_discount']['currency'])) {
    $current_code = $form_state['values']['commerce_userpoints_discount']['currency'];
  }
  else {
    $current_code = $default_currency['code'];
  }
  $arguments = _commerce_userpoints_discount_get_info($order, $current_code);

  drupal_add_library('system', 'drupal.ajax');

  $pane_form['info'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'commerce-userpoints-widget-info',
    ),
  );
  $pane_form['info']['available'] = array(
    '#type' => 'item',
    '#title' => t('Available'),
    '#prefix' => '<div class="commerce-userpoints-available">',
    '#markup' => t('You have @count_available !points.', $arguments),
    '#suffix' => '</div>',
  );
  $pane_form['info']['exchange'] = array(
    '#type' => 'item',
    '#title' => t('Exchange'),
    '#prefix' => '<div class="commerce-userpoints-exchange-rate">',
    '#markup' => t('One !point is worth @order_currency.', $arguments),
    '#suffix' => '</div>',
  );
  $pane_form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#description' => t('@total_count_needed will pay this order in full.', $arguments) . '<div id="commerce-userpoints-use-links">' . _commerce_userpoints_discount_update_use_links($order, $current_code) . '</div>',
    '#default_value' => $arguments['@count_applied'],
    '#size' => 10,
  );
  $pane_form['cup_submit'] = array(
    '#type' => 'submit',
    '#access' => TRUE,
    '#value' => t('Apply credits'),
    '#limit_validation_errors' => array(array('commerce_userpoints_discount')),
    '#executes_submit_callback' => FALSE,
    '#name' => 'cup_submit',
    '#ajax' => array(
      'callback' => 'commerce_userpoints_discount_use_credit_callback_input',
    ),
  );

  // work around for client side validation 
  // watch patch on http://drupal.org/node/1707032 to drop.
  $pane_form['cup_submit']['#attributes']['class'][] = 'cancel';

  // Give the form an ID so that we can easily target it with ajax callbacks.  
  $pane_form['#prefix'] = '<div id="commerce-userpoints-discount">';
  $pane_form['#suffix'] = '</div>';

  return $pane_form;
}

/**
 * Implements the callback for the checkout pane review form.
 */
function commerce_userpoints_discount_pane_review($form, $form_state, $checkout_pane, $order) {
  // Extract the View and display keys from the cart contents pane setting.
  list($view_id, $display_id) = explode('|', variable_get('commerce_userpoints_discount_cart_summary', 'commerce_userpoints_discount_cart_summary|default'));

  return commerce_embed_view($view_id, $display_id, array($order->order_id));
}

/**
 * Checkout pane callback: returns the cart contents pane's settings form.
 */
function commerce_userpoints_discount_pane_settings_form($checkout_pane) {
  $form = array();

  // Build an options array of Views available for the cart contents pane.
  $options = array();

  // Generate an option list from all user defined and module defined views.
  foreach (views_get_all_views() as $view_id => $view_value) {
  // Only include line item Views.
    if ($view_value->base_table == 'commerce_order') {
      foreach ($view_value->display as $display_id => $display_value) {
        $options[check_plain($view_id)][$view_id . '|' . $display_id] = check_plain($display_value->display_title);
      }
    }
  }

  $form['commerce_userpoints_discount_review_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Cart contents View'),
    '#description' => t('Specify the View to use in the review pane to display the coupons.'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_userpoints_discount_cart_summary', 'commerce_userpoints_discount_cart_summary|default'),
  );

  return $form;
}
