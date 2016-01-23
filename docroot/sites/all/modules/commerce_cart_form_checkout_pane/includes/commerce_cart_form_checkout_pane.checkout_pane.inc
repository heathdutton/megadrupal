<?php

/**
 * @file
 * Checkout pane callback functions for the cart module.
 */

/**
 * Checkout pane callback: returns the cart contents pane's settings form.
 */
function commerce_cart_form_checkout_pane_settings_form($checkout_pane) {
  $form = array();

  // Build an options array of Views available for the cart contents pane.
  $options = array();

  // Generate an option list from all user defined and module defined Views,
  // excluding the core Commerce cart block / form and order listing Views.
  $exclude = array(
    'commerce_cart_block',
    'commerce_cart_summary',
    'commerce_orders',
    'commerce_user_orders',
  );
  $default = variable_get('commerce_cart_form_contents_pane_view', 'commerce_cart_form|default');

  foreach (views_get_all_views() as $view_id => $view_value) {
    // Only include line item Views, including a View that may be excluded but
    // has already been set to be the selected View some other way. The list of
    // excluded Views was added in as of Commerce 1.5, so we want to preserve
    // existing selections much like we do for Price fields with currency select
    // lists whose currency may have been disabled since the price was entered.
    if ($view_value->base_table == 'commerce_order') {
      foreach ($view_value->display as $display_id => $display_value) {
        $key = $view_id . '|' . $display_id;

        if (!in_array($view_id, $exclude) || $key == $default) {
          $options[$view_id][$view_id . '|' . $display_id] = $display_value->display_title;
        }
      }
    }
  }

  $form['commerce_cart_form_contents_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Cart contents View Form'),
    '#description' => t('Specify the line item listing View to use in the cart contents pane.'),
    '#options' => $options,
    '#default_value' => $default,
  );

  return $form;
}

/**
 * Checkout pane callback: returns a customer profile edit form.
 */
function commerce_cart_form_checkout_pane_checkout_form($form, &$form_state, &$checkout_pane, $order) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_cart_form_contents_pane') . '/includes/commerce_cart_form_contents_pane.checkout_pane.inc';

  // Extract the View and display keys from the cart contents pane setting.
  list($view_id, $display_id) = explode('|', variable_get('commerce_cart_form_contents_pane_view', 'commerce_cart_form|default'));
  $view_args = array(
    $order->order_id,
  );

  $pane_form = array(
    '#parents' => array(
      $checkout_pane['pane_id'],
    ),
  );
  $pane_form['#attached']['css'][] = drupal_get_path('module', 'commerce_cart') . '/theme/commerce_cart.theme.css';
  // Load the specified View.
  $view = views_get_view($view_id);
  $view->set_display($display_id);

  // Check access.
  if (!$view || !$view->access($display_id)) {
    $view->destroy();
    return $output;
  }

  $view->override_url = url('checkout/' . $order->order_id . '/' . $form_state['checkout_page']['page_id']);
  $view->set_arguments(array(
    $order->order_id,
  ));

  // Prepare and execute the View query.
  $view->pre_execute();
  $view->execute();
  // Output of the view, needed for building the view form.
  $view->preview();
  $output = (!empty($view->result) || $view->style_plugin->even_empty()) ? $view->style_plugin->render($view->result) : '';

  // Get the view form.
  $view_form = views_form($pane_form, $form_state, $view, $output);

  $view_form['#view'] = $view;

  $view_form['actions']['submit']['#value'] = t('Update cart');
  $view_form['actions']['submit']['#limit_validation_errors'] = array(
    array($checkout_pane['pane_id']),
  );
  $view_form['actions']['submit']['#submit'] = array(
    'commerce_cart_form_checkout_pane_checkout_form_update_submit',
  );

  // Change any Delete buttons to say Remove.
  if (!empty($view_form['edit_delete'])) {
    foreach (element_children($view_form['edit_delete']) as $key) {
      // Load and wrap the line item to have the title in the submit phase.
      if (!empty($view_form['edit_delete'][$key]['#line_item_id'])) {
        $line_item_id = $view_form['edit_delete'][$key]['#line_item_id'];
        $form_state['line_items'][$line_item_id] = commerce_line_item_load($line_item_id);
        $view_form['edit_delete'][$key]['#limit_validation_errors'] = array(
          array($checkout_pane['pane_id']),
        );
        $view_form['edit_delete'][$key]['#submit'] = array(
          'commerce_cart_line_item_delete_form_submit',
          'commerce_cart_form_checkout_pane_checkout_form_delete_submit',
        );
      }
    }
  }

  if ($view->use_ajax && module_exists('commerce_checkout_ajax')) {
    $wrapper_id = $checkout_pane['ajax_wrapper'];
    $pane_form['#prefix'] = '<div id="' . $wrapper_id . '">';
    $pane_form['#suffix'] = '</div>';

    $ajax = array(
      'callback' => 'commerce_checkout_ajax_callback',
      'wrapper' => $wrapper_id,
      'method' => 'replace',
    );

    $view_form['actions']['submit']['#ajax'] = $ajax;

    if (!empty($view_form['edit_delete'])) {
      foreach (element_children($view_form['edit_delete']) as $key) {
        $view_form['edit_delete'][$key]['#ajax'] = $ajax;
      }
    }
  }

  $pane_form['cart_contents_form_view'] = $view_form;

  $view->destroy();

  return $pane_form;
}

function commerce_cart_form_checkout_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  return TRUE;
}

function commerce_cart_form_checkout_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
}
