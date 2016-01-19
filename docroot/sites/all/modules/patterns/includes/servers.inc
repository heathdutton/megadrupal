<?php
/**
 * @file
 * Functions related to importing and publishing patterns on the web.
 */

/**
 * Callback function for route /admin/patterns/publish/
 *
 * Sets the flag public to TRUE in the database and invokes the
 * hook_patterns_publish in all the modules implementing it.
 *
 * @param mixed $pid The ID of the Pattern to enable.
 *
 */
function patterns_publish_pattern($form, &$form_state, $pid = NULL) {
  $pattern = patterns_utils_if_invalid_go_back($pid);
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => @$pattern->name,
  );
  $disclaimer = t('Published patterns can be unpublished again later on.');
  return confirm_form($form, t('Proceed with publishing pattern %pattern', array('%pattern' => $pattern->title)), 'admin/patterns', $disclaimer);
}

/**
 * Executes patterns_db_publish_pattern($pid) and display a message.
 *
 * @see patterns_db_restore_pattern()
 */
function patterns_publish_pattern_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];
  $pattern = patterns_utils_if_invalid_go_back($pid);
  $result = patterns_db_publish_pattern($pattern);
  if ($result) {
    drupal_set_message(t('Pattern %pattern was marked as public.', array('%pattern' => @$pattern->name)));
  }
  else {
    drupal_set_message(t('An error occurred. Unable to mark pattern %pattern as public.', array('%pattern' => @$pattern->name)), 'error');
  }
  // Patterns API: Invoke all the modules who wants to do something with the published pattern
  module_invoke_all('patterns_publish', $pattern);
  $form_state['redirect'] = 'admin/patterns';
}

/**
 * Callback function for route /admin/patterns/publish/
 *
 * Sets the flag public to FALSE in the database and invokes the
 * hook_patterns_unpublish in all the modules implementing it.
 *
 * @param mixed $pid The ID of the Pattern to enable.
 *
 */
function patterns_unpublish_pattern($form, &$form_state, $pid = NULL) {
  $pattern = patterns_utils_if_invalid_go_back($pid);
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => @$pattern->name,
  );
  $disclaimer = t('Unpublished patterns can be published again later on.');
  return confirm_form($form, t('Proceed with unpublishing pattern %pattern', array('%pattern' => $pattern->title)), 'admin/patterns', $disclaimer);
}

/**
 * Executes patterns_db_unpublish_pattern($pid) and display a message.
 *
 * @see patterns_db_restore_pattern()
 */
function patterns_unpublish_pattern_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];
  $pattern = patterns_utils_if_invalid_go_back($pid);
  $result = patterns_db_unpublish_pattern($pattern);
  if ($result) {
    drupal_set_message(t('Pattern %pattern was marked as private.', array('%pattern' => @$pattern->name)));
  }
  else {
    drupal_set_message(t('An error occurred. Unable to mark pattern %pattern as private.', array('%pattern' => @$pattern->name)), 'error');
  }
  // Patterns API: Invoke all the modules who wants to do something with the unpublished pattern
  module_invoke_all('patterns_unpublish', $pattern);
  $form_state['redirect'] = 'admin/patterns';
}