<?php

/**
 * @file
 * Pages for query_coder module.
 */

/**
 * Get coder page form.
 */
function query_coder_coder_form($form, &$form_state) {
  drupal_add_library('system', 'drupal.ajax');
  $form = array();

  $form['query'] = array(
    '#title' => t('SQL query'),
    '#type' => 'textarea',
    '#rows' => 10,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#ajax' => array(
      'callback' => 'query_coder_ajax_get_coder_form_result',
      'wrapper' => 'query-coder-form-result',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['result'] = array(
    '#type' => 'item',
    '#title' => t('Result:'),
    '#markup' => t('Please enter your query and press "Submit"'),
    '#prefix' => '<div id="query-coder-form-result">',
    '#suffix' => '</div>',
  );

  $form['#submit'] = array('query_coder_coder_form_submit');
  return $form;
}

/**
 * Ajax result markup update.
 */
function query_coder_ajax_get_coder_form_result($form, $form_state) {
  drupal_get_messages(NULL, TRUE);
  $form['result']['#markup'] = $form_state['values']['result'];
  return $form['result'];
}

/**
 * Submit function for coder form.
 */
function query_coder_coder_form_submit($form, &$form_state) {
  if (isset($form_state['values']['query']) && !empty($form_state['values']['query'])) {
    $query = $form_state['values']['query'];
    $result = query_coder_parse_query($query);
    $form_state['values']['result'] = $result;
  }
  else {
    $form_state['values']['result'] = t('Please enter your query and press "Submit"');
  }
}
