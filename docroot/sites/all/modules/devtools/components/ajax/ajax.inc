<?php

/**
 * Creates a Drupal Ajax 'callback' command.
 *
 * The 'callback' command calls the specified JavaScript function.
 *
 * @param $behavior
 *   The behavior containing the call back method.
 *
 * @param $method
 *   The method to call.
 *
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function ajax_command_callback($behavior, $method, $data = array()) {
  return array(
    'command' => 'callback',
    'behavior' => $behavior,
    'method' => $method,
    'data' => $data,
  );
}

/**
 * Creates a Drupal Ajax 'remove_class' command.
 *
 * @param $selector
 *  Selector to act on.
 * 
 * @param $class
 *  Class to remove.
 * 
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function ajax_command_remove_class($selector, $class) {
  return array(
    'command' => 'remove_class',
    'selector' => $selector,
    'class' => $class,
  );
}