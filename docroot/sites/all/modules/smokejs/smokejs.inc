<?php

/**
 * @file
 * Functions for use with Drupal's Ajax framework.
 */

/**
 * @defgroup ajax_commands Ajax framework commands
 * @{
 * Functions to create various Ajax commands.
 *
 * These functions can be used to create arrays for use with the
 * ajax_render() function.
 */

/**
 * Creates a smoke.js 'signal' command.
 *
 * @param $text
 *   The message string to display to the user.
 *
 * @param $settings
 *   alertify dialog type.
 *
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function ajax_command_smoke_signal($text, $settings = array()) {
  $default = array(
    'duration' => 3000,
  );
  return array(
    'command' => 'smokeSignal',
    'text' => $text,
    'settings' => array_merge($default, $settings),
  );
}
