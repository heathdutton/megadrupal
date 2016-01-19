<?php

/**
 * @file
 * Page callbacks for the GitHub WebHook module.
 */

/**
 * Page callback; GutHub WebHook endpoint.
 */
function github_webhook_endpoint() {
  try {
    watchdog('github_webhook', 'Github webhook called', array(), WATCHDOG_DEBUG);

    $payload = github_webhook_payload();
    if (FALSE === $payload) {
      $message = t('Invalid JSON passed in payload');
      throw new \InvalidArgumentException($message);
    }
    elseif (!$payload) {
      watchdog('github_webhook', 'No payload sent', array(), WATCHDOG_DEBUG);
      return array('status' => 'ok');
    }

    $args = func_get_args();
    $response = array(
      'status' => 'ok',
      'messages' => array(),
    );
    $event = isset($_SERVER['HTTP_X_GITHUB_EVENT']) ? $_SERVER['HTTP_X_GITHUB_EVENT'] : FALSE;

    foreach (module_implements('github_webhook_event') as $module) {
      $function = $module . '_github_webhook_event';
      $function($payload, $response, $args, $event);
    }
  }
  catch (Exception $e) {
    watchdog_exception('github_webhook', $e);
    $response = array(
      'status' => 'error',
      'messages' => $e->getMessage(),
    );
  }

  return $response;
}

/**
 * Delivery callback; Convets the array to JSON and returns.
 *
 * @param array $page_callback_result
 *   The result of a page callback. The array is simply converted to JSON, which
 *   is the rendered output of the page request.
 */
function github_webhook_json_deliver(array $page_callback_result) {
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  print drupal_json_encode($page_callback_result);
  module_invoke_all('exit');
  drupal_session_commit();
}
