<?php

/**
 * @file
 * Functions and hooks related to SkaFlash push notification processing.
 */


/**
 * Primary callback for push notification handling.
 */
function skaflash_push_handler() {
  $params['email'] = isset($_POST['email']) ? check_plain($_POST['email']) : FALSE;
  $params['phone'] = isset($_POST['phone']) ? check_plain($_POST['phone']) : FALSE;
  $params['timestamp'] = isset($_POST['timestamp']) ? check_plain($_POST['timestamp']) : FALSE;

  // If all data was passed properly, attempt to handle the request.
  if ($params['email'] && $params['phone'] && $params['timestamp']) {
    // Allow other modules to react to a SkaFlash push notification.
    module_invoke_all('skaflash_pushed', $params);

    // Invoke a Rules event, if available.
    if (module_exists('rules')) {
      rules_invoke_event('skaflash_pushed', $params);
    }

    // Return the proper response and log the event.
    drupal_add_http_header('Status', '204 No Content');
    watchdog('skaflash', 'A SkaFlash push notification was received from %phone with the address %email.', array(
      '%phone' => $params['phone'],
      '%email' => $params['email'],
    ), WATCHDOG_NOTICE);
  }
  // Otherwise, return the proper response and log the event.
  else {
    drupal_add_http_header('Status', '400 Bad Request');
    watchdog('skaflash', 'A SkaFlash push notification was received, but the request did not contain valid data.', array(), WATCHDOG_WARNING);
  }

  // Perform normal Drupal shutdown operations and exit.
  module_invoke_all('exit');
  exit();
}
