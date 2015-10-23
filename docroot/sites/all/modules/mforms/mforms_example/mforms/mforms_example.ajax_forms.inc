<?php

/**
 * @file
 *    Mforms ajax example.
 */

/**
 * Builds first form step with form element loaded by ajax call.
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
function _mforms_example_ajax_forms_s1(&$form_state, &$next_step, $params) {
  $next_step = '_mforms_example_ajax_forms_s2';

  $form = array();
  $values = mforms_get_vals('ajax_forms', '_mforms_example_ajax_forms_s1');

  $form['switch'] = array(
    '#type' => 'select',
    '#title' => t('Get me form element of type') . ':',
    '#options' => array(
      'textfield' => t('Textfield'),
      'textarea' => t('Textarea'),
    ),
    '#default_value' => isset($values['switch']) ? $values['switch'] : NULL,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_mforms_example_ajax_forms_load_element',
      'wrapper' => 'new-element-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['new-element-wrapper'] = array(
    '#markup' => '',
    '#suffix' => '</div>',
    '#prefix' => '<div id="new-element-wrapper">',
  );

  if (isset($values['switch']) || isset($form_state['values']['switch'])) {
    $form['new-element-wrapper']['ajax_field'] = array(
      '#type' => isset($form_state['values']['switch']) ? $form_state['values']['switch'] : $values['switch'],
      '#title' => t('Element loaded by ajax'),
      '#default_value' => isset($values['ajax_field']) ? $values['ajax_field'] : NULL,
    );
  }

  return $form;
}

/**
 * Loads built form element as a result of ajax call.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 *
 * @return array
 *   Drupal form array.
 */
function _mforms_example_ajax_forms_load_element($form, &$form_state) {
  return $form['new-element-wrapper'];
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
function _mforms_example_ajax_forms_s2(&$form_state, &$next_step, $params) {
  $values = mforms_get_vals('ajax_forms', '_mforms_example_ajax_forms_s1');

  $form = array();

  $form['info_chosen_element'] = array(
    '#title' => t('Chosen form element'),
    '#markup' => $values['switch'],
    '#type' => 'item',
  );

  $form['info_value'] = array(
    '#title' => t('Submitted value'),
    '#markup' => $values['ajax_field'],
    '#type' => 'item',
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
function _mforms_example_ajax_forms_s2_submit($form, &$form_state) {
  drupal_set_message(t('All went fine, enjoy!'));
  mforms_clean('ajax_forms');
  $form_state['redirect'] = 'mforms';
}
