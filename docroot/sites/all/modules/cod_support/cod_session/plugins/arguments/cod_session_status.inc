<?php

/**
 * @file
 *
 * Plugin to provide an argument handler for a raw string
 */
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Session Status"),
  // keyword to use for %substitution
  'keyword' => 'cod_session_status',
  'description' => t('Creates a context from a session status argument.'),
  'settings form' => 'cod_session_argument_status_settings_form',
  'context' => 'cod_session_argument_status_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter a value for this argument'),
  ),
  'path placeholder' => 'cod_session_argument_status_path_placeholder', // This is in pagemanager.
);

/**
 * Discover if this argument gives us the term we crave.
 */
function cod_session_argument_status_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('cod_session_status');
  }

  if (!$arg) {
    return FALSE;
  }
  $context = ctools_context_create('cod_session_status', $arg);
  $context->original_argument = $arg;
  return $context;
}

/**
 * Settings form for the argument
 */
function cod_session_argument_status_settings_form(&$form, &$form_state, $conf) {
//  return $form;
}

/**
 * Switch the placeholder based upon user settings.
 */
function cod_session_argument_status_path_placeholder($argument) {
  return '%pm_arg';
}
