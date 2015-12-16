<?php

/**
 * @file
 * Provides an example of how to create form steps for
 * FormState store implementation.
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
function _mforms_example_fs_store_step1(&$form_state, &$next_step, $params) {
  // Define following step callback. If none set, that implies it is
  // the last step.
  $next_step = '_mforms_example_fs_store_step2';

  // Retrieve submitted values. This comes in handy when back action occured
  // and we need to display values that were originaly submitted.
  $data = mforms_get_vals('fs_store_example');

  // If we have the data it means we arrived here from back action,
  // so show them in form as default vals.
  $vals = array();
  if (!empty($data)) {
    $vals = $data;
  }
  elseif (!empty($form_state['values'])) {
    $vals = $form_state['values'];
  }

  // Define form array and return it.
  $form = array();
  $form['field1'] = array(
    '#type' => 'textfield',
    '#title' => t('Your name'),
    '#default_value' => isset($vals['field1']) ? $vals['field1'] : NULL,
  );
  $form['field2'] = array(
    '#type' => 'textarea',
    '#title' => t('About you'),
    '#default_value' => isset($vals['field2']) ? $vals['field2'] : NULL,
  );

  return $form;
}

/**
 * Second form step.
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
function _mforms_example_fs_store_step2(&$form_state, &$next_step, $params) {

  // Get the collected values submited at each step.
  // Here is one difference - the third parameter that defines the step from
  // which we want to retrieve the data.
  $vals1 = mforms_get_vals('fs_store_example', '_mforms_example_fs_store_step1');

  // Build an overview form and return it.
  $form['field1'] = array(
    '#type' => 'item',
    '#title' => t('Your name'),
    '#description' => $vals1['field1'],
  );
  $form['field2'] = array(
    '#type' => 'item',
    '#title' => t('About you'),
    '#description' => $vals1['field2'],
  );
  $form['confirm'] = array(
    '#type' => 'checkbox',
    '#required' => TRUE,
    '#title' => t('Confirm the data entered is correct'),
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
function _mforms_example_fs_store_step2_validate($form, &$form_state) {
  // Own validation logic.
}

/**
 * Implement submit callback for the last step to process all data submitted.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function _mforms_example_fs_store_step2_submit($form, &$form_state) {
  // Code that does all the data processing we need....

  // Clean storage.
  mforms_clean('fs_store_example');

  drupal_set_message(t('all went fine, enjoy!'));

  $form_state['redirect'] = 'mforms';
}
