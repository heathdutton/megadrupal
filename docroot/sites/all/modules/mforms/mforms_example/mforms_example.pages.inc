<?php

/**
 * @file
 *   Mforms examples.
 */

// /////////////////////////////////////////////////////////////////////////////
// Form state store example
/// ////////////////////////////////////////////////////////////////////////////

/**
 * Entry page for the multi-step form.
 *
 * @return array
 *   Drupal renderable array.
 */
function mforms_example_fs_store_page() {

  $description = '<div class="messages status">' . t("Demonstrates multistep form which uses Drupal's form_state array as storage. If you refresh the page all filled in data get lost. Same happends if you load the page again. Such scenario is fine for two step forms (i.e. enter data than confirm) or for forms where users do not expect the data they filled in are persisted for longer time.") . '</div>';

  // Just an example of how to pass additional parameters into mforms.
  $params = array('param1', 'objectN');

  // Return Drupal renderable array.
  return array(
    'message' => array(
      '#markup' => $description,
    ),
    'mform' => drupal_get_form('mforms_example_fs_form', $params),
  );
}

/**
 * Callback function to generate form.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 * @param array $params
 *   Optional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function mforms_example_fs_form($form, &$form_state, $params) {

  // Call mforms_build(); function which is a wrapper around the Mform state
  // machine. We pass in following parameters:
  // - store key under which all data is stored
  // - first step, which is a callback that returns the form array for the first
  //   screen of the form.
  // - $form_state - regular Drupal's form state array
  // - $params - additional parameters that we need inside the form building
  //   callbacks.
  return mforms_build('fs_store_example', '_mforms_example_fs_store_step1', $form_state);
}

/**
 * Callback function to validate form inputs
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_fs_form_validate($form, &$form_state) {

  // Call mforms_validate(); function that runs the validate callback for
  // current step. Params are as follows:
  // - store key
  // - $form - Drupal's form array
  // - $form_state - Drupal's form state array.
  mforms_validate('fs_store_example', $form, $form_state);
}

/**
 * Callback function to process the form inputs.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_fs_form_submit($form, &$form_state) {
  // Call mforms_submit(); function that runs the submit callback for
  // current step. Params are the same as for validate action.
  mforms_submit('fs_store_example', $form, $form_state);
}

// /////////////////////////////////////////////////////////////////////////////
// Session store example
/// ////////////////////////////////////////////////////////////////////////////

/**
 * Entry page for the multi-step form.
 *
 * @return array
 *   Drupal renderable array.
 */
function mforms_example_session_store_page() {

  $description = '<div class="messages status">' . t('Demonstrates multistep form which uses $_SESSION object as storage. If you refresh the page all filled in data will remain in fields. Same happends if you load the page again. Moreover, the last step you were at is displayed. This approach is suitable for complex forms, where it is important to provide users with more time to fill in all data and prevent data loss in case of page reload or accidental window close, etc.') . '</div>';

  // Just an example how to pass additional parameters into mforms.
  $params = array('param1', 'objectN');

  // Return Drupal renderable array.
  return array(
    'message' => array(
      '#markup' => $description,
    ),
    'mform' => drupal_get_form('mforms_example_session_form', $params),
  );
}

/**
 * Callback function to generate form.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 * @param array $params
 *   Optional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function mforms_example_session_form($form, &$form_state, $params) {
  return mforms_build('session_store_example', '_mforms_example_step1', $form_state);
}

/**
 * Callback function to validate form inputs
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_session_form_validate($form, &$form_state) {
  mforms_validate('session_store_example', $form, $form_state);
}

/**
 * Callback function to process the form inputs.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_session_form_submit($form, &$form_state) {
  mforms_submit('session_store_example', $form, $form_state);
}

// /////////////////////////////////////////////////////////////////////////////
// More forms on one page example
/// ////////////////////////////////////////////////////////////////////////////

/**
 * This is a callback to display several forms on one page.
 *
 * @return array
 *   Drupal renderable array.
 */
function mforms_example_more_forms_page() {
  global $user;

  $description = '<div class="messages status">' . t('This example demonstrates how several forms can live on one page.') . '</div>';

  return array(
    'message' => array(
      '#markup' => $description,
    ),
    'mform1' => drupal_get_form('mforms_example_more_forms_1_form', $user),
    'mform2' => drupal_get_form('mforms_example_more_forms_2_form', $user),
  );
}

/**
 * Form build callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 * @param mixed $params
 *   Additional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function mforms_example_more_forms_1_form($form, &$form_state, $params) {
  return mforms_build('more_forms_1', '_mforms_example_more_forms_1_s1', $form_state, $params);
}

/**
 * Form validate callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_more_forms_1_form_validate($form, &$form_state) {
  mforms_validate('more_forms_1', $form, $form_state);
}

/**
 * Form submit callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_more_forms_1_form_submit($form, &$form_state) {
  mforms_submit('more_forms_1', $form, $form_state);
}

/**
 * Form build callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 * @param mixed $params
 *   Additional params passed into form.
 *
 * @return array
 *   Drupal form array.
 */
function mforms_example_more_forms_2_form($form, &$form_state, $params) {
  return mforms_build('more_forms_2', '_mforms_example_more_forms_2_s1', $form_state, $params);
}

/**
 * Form validate callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_more_forms_2_form_validate($form, &$form_state) {
  mforms_validate('more_forms_2', $form, $form_state);
}

/**
 * Form submit callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_more_forms_2_form_submit($form, &$form_state) {
  mforms_submit('more_forms_2', $form, $form_state);
}

// /////////////////////////////////////////////////////////////////////////////
// Multi step forms with ajax example
/// ////////////////////////////////////////////////////////////////////////////

/**
 * Page callback for multistep form with ajax example.
 *
 * @return array
 *   Drupal renderable array.
 */
function mforms_example_ajax_forms_page() {
  return drupal_get_form('mforms_example_ajax_form');
}

/**
 * Form build callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 *
 * @return array
 *   Drupal form array.
 */
function mforms_example_ajax_form($form, &$form_state) {
  return mforms_build('ajax_forms', '_mforms_example_ajax_forms_s1', $form_state);
}

/**
 * Form validate callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_ajax_form_validate($form, &$form_state) {
  mforms_validate('ajax_forms', $form, $form_state);
}

/**
 * Form submit callback.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function mforms_example_ajax_form_submit($form, &$form_state) {
  mforms_submit('ajax_forms', $form, $form_state);
}
