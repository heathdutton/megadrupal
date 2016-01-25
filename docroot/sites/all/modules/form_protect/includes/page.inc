<?php

/**
 * @file
 * Contains the page callback for 'submit/form' route.
 */

/**
 * Provides a page callback to redirect forms when JavaScript is disabled.
 */
function form_protect_nojs() {
  if (variable_get('form_protect_log', FALSE)) {
    // Log the blocked submission.
    $post = '<pre>' . print_r($_POST, TRUE) . '</pre>';
    watchdog('form_protect', "Blocked submission. Post data:$post");
  }

  return array(
    '#type' => 'markup',
    '#markup' => t('JavaScript is not enabled in your browser. This form requires JavaScript to be enabled.'),
  );
}
