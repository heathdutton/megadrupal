<?php

/**
 * @file
 * Coupon checkout panes.
 *
 * Expose a textfield for entering a coupon code during checkout, and show list
 * of coupons applied to current order.
 */

/**
 * Payment pane: form callback.
 */
function commerce_coupon_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  // Allow to replace pane content with ajax calls.
  $pane_form = array(
    '#prefix' => '<div id="commerce-checkout-coupon-ajax-wrapper">',
    '#suffix' => '</div>',
  );

  // Store the payment methods in the form for validation purposes.
  $pane_form['coupon_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon Code'),
    '#description' => t('Enter here your coupon code.'),
  );

  if (variable_get('commerce_coupon_checkout_enable_add_button', TRUE)) {
    $pane_form['coupon_add'] = array(
      '#type' => 'button',
      '#value' => t('Add coupon'),
      '#name' => 'coupon_add',
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'commerce_coupon_add_coupon_callback',
        'wrapper' => 'commerce-checkout-coupon-ajax-wrapper',
      ),
    );
  }

  // Extract the View and display keys from the cart contents pane setting.
  $pane_view = variable_get('commerce_coupon_checkout_pane_view', 'commerce_coupon_review_pane|checkout');
  if ($pane_view <> 'none') {
    list($view_id, $display_id) = explode('|', $pane_view);
    if (!empty($view_id) && !empty($display_id) && views_get_view($view_id)) {
      $pane_form['redeemed_coupons'] = array(
        '#type' => 'markup',
        '#markup' => commerce_embed_view($view_id, $display_id, array($order->order_id)),
      );
    }
  }

  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] == 'coupon_add') {
    if (!empty($form_state['values']['commerce_coupon']['coupon_add'])) {
      list($view_id, $display_id) = explode('|', variable_get('commerce_coupon_review_pane_view', 'commerce_coupon_review_pane|checkout'));
      if (!empty($view_id) && !empty($display_id)) {
        global $_commerce_coupon_validation_error_message;
        $code = $form_state['input']['commerce_coupon']['coupon_code'];
        if (!empty($code) && commerce_coupon_code_is_valid($code, $order)) {
          $coupon = commerce_coupon_load_by_code($code);
          commerce_coupon_redeem_coupon($coupon, $order);

          // Clear the field value so that the coupon code does not get resubmitted
          // causing an error when user uses main "Continue to next step" submit.
          $pane_form['coupon_code']['#value'] = '';

          if (isset($order->order_id) && is_numeric($order->order_id)) {
            // This is to get the summary of the form rerendered.
            $alter_coupon_commands = &drupal_static('alter_coupon_commands');
            $coupon_current_order = &drupal_static('coupon_current_order');
            $alter_coupon_commands = TRUE;
            $coupon_current_order = $order->order_id;
          }
          // Force shopping cart refresh to recalculate all prices there,
          // which is not done automatically in case of percentage coupons.
          commerce_cart_order_refresh($order);
          $pane_form['redeemed_coupons']['#markup'] = commerce_embed_view($view_id, $display_id, array($order->order_id));
        }
        else {
          if (isset($_commerce_coupon_validation_error_message)) {
            $message = $_commerce_coupon_validation_error_message;
          }
          else {
            $message = t('Your coupon code is not valid.');
          }
          form_set_error('commerce_coupon][coupon_code', $message);
        }
      }
    }
  }

  // Display any new status messages added by this pane within the pane's area.
  if (drupal_get_messages(NULL, FALSE)) {
    $pane_form['status_messages'] = array(
      '#type' => 'markup',
      '#markup' => theme('status_messages'),
      '#weight' => -1,
    );
  }

  return $pane_form;
}

/**
 * Check whether the coupon code is valid.
 */
function commerce_coupon_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  global $_commerce_coupon_validation_error_message;

  // Check if coupon code is empty.
  if (empty($form_state['values']['commerce_coupon']['coupon_code'])) {
    return TRUE;
  }

  // Validate the coupon code.
  if (commerce_coupon_code_is_valid($form_state['values']['commerce_coupon']['coupon_code'], $order)) {
    return TRUE;
  }
  else {
    if (isset($_commerce_coupon_validation_error_message)) {
      $message = $_commerce_coupon_validation_error_message;
    }
    else {
      $message = t('Your coupon code is not valid.');
    }

    form_set_error('commerce_coupon][coupon_code', $message);
    return FALSE;
  }
}

/**
 * We have a valid coupon code, so submit it.
 */
function commerce_coupon_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  // Only redeem coupon if no validation errors on form and coupon code is set
  // This prevents trying to add the same coupon twice on validation error in another pane
  if (!form_get_errors() && $code = $form_state['values']['commerce_coupon']['coupon_code']) {
    $commerce_coupon = commerce_coupon_load_by_code($code);
    commerce_coupon_redeem_coupon($commerce_coupon, $order);
    // Rebuild the form only if "Add coupon" ajax submit was used
    // and we're still staying on the same page.
    if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] == 'coupon_add') {
      $form_state['rebuild'] = TRUE;
    }
  }
}

/**
 * Callback for the ajax button coupon_add.
 */
function commerce_coupon_add_coupon_callback($form, &$form_state) {
  return $form['commerce_coupon'];
}

/**
 * Implements hook_ajax_render_alter().
 *
 * This is to get the summary of the cart to rerender on the add coupon button.
 */
function commerce_coupon_ajax_render_alter(&$commands) {
  $alter_coupon_commands = &drupal_static('alter_coupon_commands');
  $coupon_current_order = &drupal_static('coupon_current_order');

  if ($alter_coupon_commands) {
    list($view_id, $display_id) = explode('|', variable_get('commerce_cart_contents_pane_view', 'commerce_cart_summary|default'));
    $contents = array();
    $contents['cart_contents_view'] = array(
      '#markup' => commerce_embed_view($view_id, $display_id, array($coupon_current_order)),
    );
    $contents = drupal_render($contents);
    $commands[] = ajax_command_replace('.view-commerce-cart-summary', $contents);
  }
}

/**
 * Implements the callback for the checkout pane review form
 */
function commerce_coupon_pane_review($form, $form_state, $checkout_pane, $order) {
  if (!empty($order->commerce_coupon_order_reference)) {
    // Extract the View and display keys from the cart contents pane setting.
    list($view_id, $display_id) = explode('|', variable_get('commerce_coupon_review_pane_view', 'commerce_coupon_review_pane|checkout'));

    return commerce_embed_view($view_id, $display_id, array($order->order_id));
  }
}

/**
 * Checkout pane callback: returns the cart contents pane's settings form.
 */
function commerce_coupon_pane_settings_form($checkout_pane) {
  $form = array();

  // Build an options array of Views available for the cart contents pane.
  $options = array();

  // Generate an option list from all user defined and module defined views.
  foreach (views_get_all_views() as $view_id => $view_value) {
    // Only include line item Views.
    if ($view_value->base_table == 'commerce_coupon') {
      foreach ($view_value->display as $display_id => $display_value) {
        $options[check_plain($view_id)][$view_id . '|' . $display_id] = check_plain($display_value->display_title);
      }
    }
  }

  $form['commerce_coupon_checkout_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Coupons Checkout Pane View'),
    '#description' => t('Specify the View to use in the checkout pane under the coupon form. Select \'none\' to not display anything.'),
    '#options' => array('none' => t('None')) + $options,
    '#default_value' => variable_get('commerce_coupon_checkout_pane_view', 'commerce_coupon_review_pane|checkout'),
  );

  $form['commerce_coupon_review_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Coupons Review Pane View'),
    '#description' => t('Specify the View to use in the review pane to display the coupons.'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_coupon_review_pane_view', 'commerce_coupon_review_pane|checkout'),
  );

  $form['commerce_coupon_checkout_enable_add_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Add coupon</em> button'),
    '#description' => t('Enabling <em>Add coupon</em> button provides option to add multiple coupon codes from <em>Coupons</em> checkout pane.'),
    '#default_value' => variable_get('commerce_coupon_checkout_enable_add_button', TRUE),
  );

  return $form;
}
