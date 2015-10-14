<?php

/**
 * @file
 * Form validation through API as if submitted by web browser.
 */

/**
 * Validates a form with values as they were submitted by a browser.
 */
function wf_required_validate_form($form_id) {
  $args = func_get_args();
  array_shift($args);

  // First, build the form as presented to the user.
  $form_state = wf_required_form_state_defaults($args);
  $form = wf_required_build_form($form_id, $form_state, array());

  // Extract values.
  $input = array();
  wf_required_build_form_values($form, $input);

  // Build the form all over again with $_POST-alike values.
  $form_state = wf_required_form_state_defaults($args);
  $form = wf_required_build_form($form_id, $form_state, $input);

  // Fix for file.module validation.
  if (!isset($form_state['triggering_element']) && !empty($form_state['buttons'])) {
    $form_state['triggering_element'] = $form_state['buttons'][0];
  }
  if ($form_state['process_input']) {
    drupal_validate_form($form_id, $form, $form_state);
  }
}

/**
 * Build a default $form_state array for this module.
 */
function wf_required_form_state_defaults($args = array()) {
  $form_state = array('build_info' => array('args' => $args));
  $form_state += form_state_defaults();

  // This will only be called from outside node edit context, so let's trigger
  // normal node validation.
  $form_state['wf_required_validate'] = TRUE;
  return $form_state;
}

/**
 * Build a form from scratch up to and including form_builder().
 */
function wf_required_build_form($form_id, &$form_state, $input) {
  $form_state['input'] = $input;
  $form_state['programmed'] = TRUE;
  $form = drupal_retrieve_form($form_id, $form_state);
  $form_state['submitted'] = TRUE;

  $form_state['must_validate'] = TRUE;
  form_clear_error();

  drupal_prepare_form($form_id, $form, $form_state);

  $form_state['values'] = array();

  $form = form_builder($form_id, $form, $form_state);
  return $form;
}

/**
 * Build form values as if they were submitted by web browsers.
 *
 * Semi-equivalent to $_POST.
 *
 * @param array $element
 *   Form element.
 * @param array $values
 *   Where to store the values.
 */
function wf_required_build_form_values(array $element, array &$values) {
  if (isset($element['#input']) && isset($element['#value']) && !empty($element['#value'])) {
    drupal_array_set_nested_value($values, $element['#parents'], $element['#value']);
  }
  foreach (element_get_visible_children($element) as $key) {
    wf_required_build_form_values($element[$key], $values);
  }
}
