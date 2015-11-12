<?php

/**
 * @file
 * Provides example of form steps for session store implemtation.
 */


/**
 * First step called by the Mforms state machine.
 *
 * @param array $form_state
 *   Drupal form_state array.
 * @param string $next_step
 *   Mforms next step callback name.
 * @param mixed $params
 *   Optional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function _mforms_example_step1(&$form_state, &$next_step, $params) {
  // Define following step callback. If none set, that implies it is
  // the last step.
  $next_step = '_mforms_example_step2';

  // Retrieve submitted values. This comes in handy when back action
  // occured and we need to display values that were originaly submitted.
  $data = mforms_get_vals('session_store_example');

  // If we have the data it means we arrived here from back action, so show
  // them in form as default vals.
  if (!empty($data)) {
    $vals = $data;
  }
  elseif (isset($form_state['values'])) {
    $vals = $form_state['values'];
  }

  // Define form array and return it.
  $form = array();
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($vals['name']) ? $vals['name'] : NULL,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => isset($vals['email']) ? $vals['email'] : NULL,
    '#required' => TRUE,
  );
  $form['www'] = array(
    '#type' => 'textfield',
    '#title' => t('Your web site'),
    '#default_value' => isset($vals['www']) ? $vals['www'] : NULL,
  );

  return $form;
}

/**
 * Validate callback - validates email address.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function _mforms_example_step1_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Invalid email.'));
  }
}

/**
 * Step two.
 *
 * @param array $form_state
 *   Drupal form_state array.
 * @param string $next_step
 *   Mforms next step callback name.
 * @param mixed $params
 *   Optional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function _mforms_example_step2(&$form_state, &$next_step, $params) {
  $next_step = '_mforms_example_step3';
  $form = array();

  $data = mforms_get_vals('session_store_example');

  if (!empty($data)) {
    $vals = $data;
  }
  elseif (isset($form_state['values'])) {
    $vals = $form_state['values'];
  }

  $form['hobby'] = array(
    '#type' => 'textarea',
    '#title' => t('Hobby'),
    '#default_value' => isset($vals['hobby']) ? $vals['hobby'] : NULL,
  );
  $form['education'] = array(
    '#type' => 'textarea',
    '#title' => t('Education'),
    '#default_value' => isset($vals['education']) ? $vals['education'] : NULL,
  );
  $form['employment'] = array(
    '#type' => 'textarea',
    '#title' => t('Employment'),
    '#default_value' => isset($vals['employment']) ? $vals['employment'] : NULL,
  );

  return $form;
}

/**
 * Third step.
 *
 * @param array $form_state
 *   Drupal form_state array.
 * @param string $next_step
 *   Mforms next step callback name.
 * @param mixed $params
 *   Optional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function _mforms_example_step3(&$form_state, &$next_step, $params) {
  $next_step = '_mforms_example_step4';
  $form = array();

  $data = mforms_get_vals('session_store_example');

  if (!empty($data)) {
    $vals = $data;
  }
  elseif (isset($form_state['values'])) {
    $vals = $form_state['values'];
  }

  $form['anythingelse'] = array(
    '#type' => 'textarea',
    '#title' => t('Anything else'),
    '#default_value' => isset($vals['anythingelse']) ? $vals['anythingelse'] : NULL,
  );

  return $form;
}

/**
 * Step four - here we create an overview page.
 *
 * @param array $form_state
 *   Drupal form_state array.
 * @param string $next_step
 *   Mforms next step callback name.
 * @param mixed $params
 *   Optional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function _mforms_example_step4(&$form_state, &$next_step, $params) {

  // Get the collected values submited at each step.
  // Here is one difference - the third parameter that defines the step
  // from which we want to retrieve the data.
  $vals1 = mforms_get_vals('session_store_example', '_mforms_example_step1');
  $vals2 = mforms_get_vals('session_store_example', '_mforms_example_step2');
  $vals3 = mforms_get_vals('session_store_example', '_mforms_example_step3');

  // Build an overview form and return it.
  $form['name'] = array(
    '#type' => 'item',
    '#title' => t('Name'),
    '#description' => $vals1['name'],
  );
  $form['email'] = array(
    '#type' => 'item',
    '#title' => t('Email'),
    '#description' => $vals1['email'],
  );
  $form['www'] = array(
    '#type' => 'item',
    '#title' => t('Web site'),
    '#description' => $vals1['www'],
  );
  $form['hobby'] = array(
    '#type' => 'item',
    '#title' => t('Hobby'),
    '#description' => $vals2['hobby'],
  );
  $form['education'] = array(
    '#type' => 'item',
    '#title' => t('Education'),
    '#description' => $vals2['education'],
  );
  $form['employment'] = array(
    '#type' => 'item',
    '#title' => t('Employment'),
    '#description' => $vals2['employment'],
  );
  $form['anythingelse'] = array(
    '#type' => 'item',
    '#title' => t('Anything else'),
    '#description' => $vals3['anythingelse'],
  );
  $form['confirm'] = array(
    '#type' => 'checkbox',
    '#description' => t('Confirm the data entered is correct'),
  );
  return $form;
}

/**
 * Validate callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function _mforms_example_step4_validate($form, &$form_state) {
  if (!$form_state['values']['confirm']) {
    form_set_error('confirm', t('You have to confirm...'));
  }
}

/**
 * Implement submit callback for the last step to process all data submitted.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function _mforms_example_step4_submit($form, &$form_state) {
  // Code that does all the data processing we need....

  // Call mforms_clean(); to dump all data from store - that will rest the
  // state machine.
  mforms_clean('session_store_example');

  drupal_set_message(t('all went fine, enjoy!'));
}
