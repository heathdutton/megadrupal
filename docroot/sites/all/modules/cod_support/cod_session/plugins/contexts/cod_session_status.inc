<?php

/**
 * @file
 *
 * Plugin to provide a node context. A node context is a node wrapped in a
 * context object that can be utilized by anything that accepts contexts.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Session Status"),
  'description' => t('Status (Proposed, Accepted, etc.'),
  'context' => 'cod_session_context_create_status',
  'edit form' => 'cod_session_context_status_settings_form',
  'defaults' => '',
  'no ui' => FALSE,
  'keyword' => 'cod_session_status',
  'context name' => 'cod_session_status',
    // Provides a list of items which are exposed as keywords.
  'convert list' => 'cod_session_context_convert_list',
  'convert' => 'cod_session_context_status_convert',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the status machine name for this context.'),
  ),
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function cod_session_context_create_status($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('cod_session_status');
  $context->plugin = 'cod_session_status';

  // If an argument is being passed, convert it to an array.
  if (!empty($data) && !is_array($data)) {
    $value = $data;
    $data = array();
    $data['status'] = $value;
  }

  // We don't know the session status in the argument, lets try to create one
  if ($empty) {
    return $context;
  }

  if (!empty($data)) {
    // Support the array storage from the settings form but also handle direct input from arguments.
    $context->data = $data;
    $output = check_plain($data['status']);
    $context->title = ($conf) ? check_plain($data['identifier']) : drupal_strtoupper(drupal_substr($output, 0, 1)) . drupal_substr($output, 1);
    return $context;
  }
}

function cod_session_context_status_settings_form($form, &$form_state) {
  $conf = &$form_state['conf'];

  $form['status'] = array(
    '#title' => t('Enter the status for the event'),
    '#type' => 'textfield',
    '#maxlength' => 64,
    '#weight' => -10,
  );

  if (!empty($conf['status'])) {
    $form['status']['#description'] = t('Currently set to !link', array('!link' => $conf['status']));
  }

  return $form;
}

/**
 * Validate a node.
 */
function cod_session_context_status_settings_form_validate($form, &$form_state) {
  // Validate the autocomplete
  // We should do something here!
  /*
  if (!$node || (empty($node->status) && !(user_access('administer nodes')))) {
    form_error($form['node'], t('Invalid node selected.'));
  }
  else {
    form_set_value($form['nid'], $node->nid, $form_state);
  }
  */
}

function cod_session_context_status_settings_form_submit($form, &$form_state) {
  // This will either be the value set previously or a value set by the
  // validator.
  $form_state['conf']['status'] = $form_state['values']['status'];
}

/**
 * Provide a list of sub-keywords.
 *
 * This is used to provide keywords from the context for use in a content type,
 * pane, etc.
 */
function cod_session_context_convert_list() {
  return array(
    'status' => t('Status'),
  );
}

/**
 * Convert a context into a string.
 */
function cod_session_context_status_convert($context, $type) {
  switch ($type) {
    // Convert a vocabulary keyword into the data.
    case 'status':
      return $context->data['status'];
  }
}
