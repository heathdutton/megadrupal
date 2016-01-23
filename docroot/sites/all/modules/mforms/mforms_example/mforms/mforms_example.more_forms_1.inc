<?php

/**
 * @file
 * Provides example of multi forms at one page.
 */


/**
 * First form step.
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
function _mforms_example_more_forms_1_s1(&$form_state, &$next_step, $params) {
  $next_step = '_mforms_example_more_forms_1_s2';

  $controls = mforms_controls_get('more_forms_1');
  $controls->setControlsLabels(t('Edit form 1'));

  $form['field1'] = array(
    '#title' => t('Field 1'),
    '#markup' => isset($_SESSION['field1']) ? $_SESSION['field1'] : NULL,
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
function _mforms_example_more_forms_1_s2(&$form_state, &$next_step, $params) {
  $next_step = '_mforms_example_more_forms_1_s3';

  $controls = mforms_controls_get('more_forms_1');
  $controls->setControlsLabels(t('Submit'));
  $controls->setCancelPath('mforms/example/more_forms');

  $form['field1'] = array(
    '#type' => 'textfield',
    '#title' => t('Field 1'),
    '#required' => FALSE,
    '#default_value' => isset($_SESSION['field1']) ? $_SESSION['field1'] : NULL,
  );

  return $form;
}

/**
 * Submit callback of second form step.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function _mforms_example_more_forms_1_s2_submit($form, &$form_state) {
  $_SESSION['field1'] = $form_state['values']['field1'];
  drupal_set_message(t('Information has been updated'));

  // Override next, so that after submitting of edit mode first step gets
  // loaded.
  $steps = MformsSteps::getInstance(mforms_store_get('more_forms_1'));
  $steps->overrideNext('_mforms_example_more_forms_1_s1');
}

/**
 * Form build callback that simulates return to the read state.
 *
 * @param array $form_state
 *   Drupal form_state array.
 * @param string $next_step
 *   Next step.
 * @param mixed $params
 *   Additional params passed into form.
 *
 * @return array
 *   Drupal form.
 */
function _mforms_example_more_forms_1_s3(&$form_state, &$next_step, $params) {
  return _mforms_example_more_forms_1_s1($form_state, $next_step, $params);
}
