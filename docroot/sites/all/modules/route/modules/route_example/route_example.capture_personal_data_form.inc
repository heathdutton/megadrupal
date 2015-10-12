<?php

/**
 * Builds an example form that captures personal data.
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_personal_data_form($form, &$form_state) {
  $route = $form_state['route'];

  $form['gender'] = array(
    '#type' => 'select',
    '#title' => _route_example_get_property_title('gender'),
    '#required' => TRUE,
    '#options' => _route_example_get_genders(),
    '#default_value' => $route->gender,
    '#ajax' => array(
      'callback' => 'route_example_capture_personal_data_form_js',
      'wrapper' => 'full-name',
      'method' => 'replace',
      'effect' => 'fade',
    )
  );

  $form['full_name'] = array(
    '#type' => 'item',
    '#id' => 'full-name',
  );

  $form['full_name']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => _route_example_get_property_title('first_name'),
    '#required' => TRUE,
    '#default_value' => $route->first_name,
  );

  $form['full_name']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => _route_example_get_property_title('last_name'),
    '#required' => TRUE,
    '#default_value' => $route->last_name,
  );

  if ((!empty($form_state['values']['gender']) && $form_state['values']['gender'] == 'female')
  || (empty($form_state['values']['gender']) && isset($route->gender) && $route->gender == 'female')) {
    $form['full_name']['maiden_name'] = array(
      '#type' => 'textfield',
      '#title' => _route_example_get_property_title('maiden_name'),
      '#default_value' => $route->maiden_name,
    );
  }

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' =>  _route_example_get_property_title('email'),
    '#required' => TRUE,
    '#default_value' => $route->email,
  );

  return $form;
}

/**
 * Validate handler for an example form that captures personal data.
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_personal_data_form_validate($form, &$form_state) {
  if(!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Email address is invalid.'));
  }
}

/**
 * Submit handler for an example form that captures personal data.
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_personal_data_form_submit($form, &$form_state) {
  $route = $form_state['route'];

  // Now, set your form values into the route object. In real life scenario you can do some further processing too, like sanitizing entered values, calling web services or whatever...
  // Properties set on $route object this way are accessible inside all later step.
  $route->gender = $form_state['values']['gender'];
  $route->first_name = $form_state['values']['first_name'];
  $route->last_name = $form_state['values']['last_name'];
  if (!empty($form_state['values']['maiden_name'])) {
    $route->maiden_name = $form_state['values']['maiden_name'];
  }
  $route->email = $form_state['values']['email'];
}

/**
 * JS callback. Updates part of the form usin AJAX
 *
 * @param $form
 * @param $form_state
 */
function route_example_capture_personal_data_form_js($form, $form_state) {
  return $form['full_name'];
}