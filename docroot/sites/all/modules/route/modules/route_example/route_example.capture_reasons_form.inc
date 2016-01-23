<?php

/**
 * Builds an example form that captures reasons.
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_reasons_form($form, &$form_state) {
  $route = $form_state['route'];

  $form['reasons'] = array(
    '#type' => 'checkboxes',
    '#title' => _route_example_get_property_title('reasons'),
    '#options' => _route_example_get_reasons(),
    '#default_value' => isset($route->reasons) ? $route->reasons : array(),
  );

  return $form;
}

/**
 * Submit handler for an example form that captures reasons.
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_reasons_form_submit($form, &$form_state) {
  $route = $form_state['route'];

  // Now, set your form values into the route object. In real life scenario you can do some further processing too, like sanitizing entered values, calling web services or whatever...
  // Properties set on $route object this way are accessible inside all later step.
  $route->reasons = $form_state['values']['reasons'];
}