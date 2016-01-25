<?php

/**
 * @file
 * Plugin to provide an argument handler for a user id
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Route: active step"),
  'keyword' => 'route',
  'description' => t('Creates a route context from an active step argument.'),
  'context' => 'route_active_step_argument_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter active step'),
  ),
);

/**
 * Discover if this argument gives us the context we crave.
 */
function route_active_step_argument_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If step name is empty it wants a generic, unfilled context. Also the journey should start from the beginning.
  if ($empty) {
    return ctools_context_create_empty('route');
  }

  $context = ctools_context_create('route', array('active_step' => $arg));

  // Call enter step hook
  // This is called ONLY for GET requests. This is to prevent form posts to call it.
  if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    module_invoke_all('route_activate_step', $context->data);
    if ($messages = $context->data->get_messages()) {
      foreach ($messages as $message) {
        drupal_set_message($message['message'], $message['type']);
      }
    }
  }

  return $context;
}