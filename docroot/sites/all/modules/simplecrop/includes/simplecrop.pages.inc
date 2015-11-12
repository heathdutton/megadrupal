<?php

/**
 * @file
 * Contains page callbacks for SimpleCrop module.
 */

/**
 * Ajax page callbacks for "apply crop" / "edit crop" buttons submittion.
 */
function simplecrop_ajax_rebuild_image() {
  $form_parents = func_get_args();
  $form_build_id = (string) array_pop($form_parents);

  if (empty($_POST['form_build_id']) || $form_build_id != $_POST['form_build_id']) {
    // Invalid request.
    drupal_set_message(t('An unrecoverable error occurred.'), 'error');
    $commands = array();
    $commands[] = ajax_command_replace(NULL, theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
  }

  list($form, $form_state) = ajax_get_form();

  if (!$form) {
    // Invalid form_build_id.
    drupal_set_message(t('An unrecoverable error occurred. Try reloading the page and submitting again.'), 'error');
    $commands = array();
    $commands[] = ajax_command_replace(NULL, theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
  }

  // Process user input. $form and $form_state are modified in the process.
  drupal_process_form($form['#form_id'], $form, $form_state);

  // Retrieve the element to be rendered.
  foreach ($form_parents as $parent) {
    $form = $form[$parent];
  }

  $output = theme('status_messages') . drupal_render($form);
  $js = drupal_add_js();
  $settings = call_user_func_array('array_merge_recursive', $js['settings']['data']);

  $commands = array();
  $commands[] = ajax_command_replace(NULL, $output, $settings);
  return array('#type' => 'ajax', '#commands' => $commands);
}
