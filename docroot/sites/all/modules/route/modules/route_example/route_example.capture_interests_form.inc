<?php

/**
 * Builds an example form that captures interests.
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_interests_form($form, &$form_state) {
  $route = $form_state['route'];

  $form['interests'] = array(
    '#type' => 'checkboxes',
    '#title' => _route_example_get_property_title('interests'),
    '#options' => _route_example_get_interests(),
    '#default_value' => isset($route->interests) ? $route->interests : array(),
  );

  return $form;
}

/**
 * Submit handler for an example form that captures interests.
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_interests_form_submit($form, &$form_state) {
  $route = $form_state['route'];

  // Now, set your form values into the route object. In real life scenario you can do some further processing too, like sanitizing entered values, calling web services or whatever...
  // Properties set on $route object this way are accessible inside all later step.
  $route->interests = $form_state['values']['interests'];
}