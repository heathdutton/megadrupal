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
  'title' => t("Schedule Day"),
  // keyword to use for %substitution
  'keyword' => 'cod_schedule_day_argument',
  'description' => t('Creates a date context from a day.'),
  'settings form' => 'cod_schedule_argument_day_settings_form',
  'context' => 'cod_schedule_argument_day_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter a value for this argument'),
  ),
  'path placeholder' => 'ctools_string_path_placeholder', // This is in pagemanager.
);

/**
 * Discover if this argument gives us the term we crave.
 */
function cod_schedule_argument_day_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('string');
  }

  $context = ctools_context_create('string', $arg);
  $context->original_argument = $arg;

  return $context;
}

/**
 * Settings form for the argument
 */
function cod_schedule_argument_day_settings_form(&$form, &$form_state, $conf) {
//  return $form;
}

/**
 * Switch the placeholder based upon user settings.
 */
function cod_schedule_argument_day_path_placeholder($argument) {
  return '%pm_arg';
}
