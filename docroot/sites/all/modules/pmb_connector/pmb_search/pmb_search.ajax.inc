<?php

/**
 * @file
 * Ajax functions for search.
 */

// TODO Ajax.
function pmb_search_ajax_get_field($form, &$form_state) {
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $output = '';
  $form_state['input'] = $form['input'] = $_POST;
  // Enable the submit/validate handlers to determine whether AHAH-submittted.
  $form_state['ajax_submission'] = TRUE;
  $form['#programmed'] = $form_state['redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  $fields = $form['fields'];
  $header = array();
  $row = array();
  foreach (element_children($fields) as $field) {
    $ligne = array();
    foreach (element_children($fields[$field]) as $elem) {
      $ligne[] = drupal_render($form['fields'][$field][$elem]);
    }
    $row[] = $ligne;
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  drupal_json(array('status' => TRUE, 'data' => $output));
  exit();
}
