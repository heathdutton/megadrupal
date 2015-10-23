<?php

/**
 * @file
 * Checkout pane callback functions for the cart module.
 */


/**
 * Checkout pane callback: returns the cart contents pane's settings form.
 */
function commerce_views_pane_checkout_pane_settings_form($checkout_pane) {
  $form = array();

  // Extract the View and display keys from the pane id.
  $pane_info = commerce_views_display_parse_pane_id($checkout_pane['pane_id']);
  if (empty($pane_info['view_id']) || empty($pane_info['display_id'])) {
    return $form;
  }
  $view = views_get_view($pane_info['view_id']);

  $args = $view->get_items('argument', $pane_info['display_id']);
  if (!empty($args) && is_array($args)) {
    foreach ($args as $field_name => $arg_data) {
      if (!isset($arg_data['field'])) {
        continue;
      }

      $var_name = implode('_', $pane_info + array($field_name));
      $form[$var_name] = array(
        '#title' => t('Value for argument %field_name', array('%field_name' => $field_name)),
        '#type' => 'textfield',
        '#default_value' => variable_get($var_name),
      );
      if ($arg_data['field'] == 'order_id' && empty($form[$var_name]['#default_value'])) {
        $form[$var_name]['#default_value'] = '[commerce-order:order-id]';
      }
    }
  }
  if(!empty($form)) {
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

    $form['cvp_argument_suggestions'] = RulesTokenEvaluator::help($var_info);
  }

/** @todo: any pane options? hide view title, etc ? **/
  return $form;
}

/**
 * Checkout pane callback: returns the cart contents View for inclusion in the
 *   checkout form.
 */
function commerce_views_pane_checkout_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  // Extract the View and display keys from the pane id.
  $view_info = commerce_views_display_parse_pane_id($checkout_pane['pane_id']);
  if (empty($view_info['view_id']) || empty($view_info['display_id'])) {
    return $pane_form;
  }

  // Render the view
  $view_args = _commerce_views_pane_view_build_order_arguments($view_info, $order);
  $view_output = commerce_views_pane_execute_display($view_info['view_id'], $view_info['display_id'], $view_args);
  $pane_form['views_content'] = array(
    '#markup' => theme('commerce_views_pane_content', $view_output + array('order' => $order)),
  );

  return $pane_form;
}

/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 */
function commerce_views_pane_checkout_pane_review($form, $form_state, $checkout_pane, $order) {

  // Extract the View and display keys from the pane id.
  $view_info = commerce_views_display_parse_pane_id($checkout_pane['pane_id']);
  if (empty($view_info['view_id']) || empty($view_info['display_id'])) {
    return '';
  }

  // Render the view
  $view_args = _commerce_views_pane_view_build_order_arguments($view_info, $order);
  $view_output = commerce_views_pane_execute_display($view_info['view_id'], $view_info['display_id'], $view_args);
  if (empty($view_output['content'])) {
    return '';
  }

  return theme('commerce_views_pane_content', $view_output + array('order' => $order));
}
