<?php

/**
 * Builds an example form that shows summary. This doesn't sound like a form initially, but we actually still want to display buttons to go back or start again.
 *
 * @param $form
 * @param $form_state
 */
function route_example_show_summary_form($form, &$form_state) {
  $route = $form_state['route'];
  $captured_data = $route->get_data();

  // Simply list all key=>value pairs. We need proper formatters on some point.
  foreach($captured_data as $key => $value) {
    $form[$key] = array(
      '#type' => 'item',
      '#title' => _route_example_get_property_title($key),
      '#markup' => $route->format($key)
    );
  }

  return $form;
}

/**
 * Submit handler for an example form that captures reasons.
 *
 * @param $form
 * @param $form_state
 */
function route_example_show_summary_form_submit($form, &$form_state) {
  drupal_set_message(t('Thank you for completing our example journey! Now you can fully appreciate how great Route module by !link.', array('!link' => l(t('looking at the configuration page of this panel'), 'admin/structure/pages/edit/page-route_example'))));
}