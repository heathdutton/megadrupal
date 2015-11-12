<?php

/**
 * @file
 * Processes game events received from game clients.
 */

/**
 * Page callback for processing event data.
 */
function drpg_event_process_events() {
  $events_data = (isset($_REQUEST['events'])) ? $_REQUEST['events'] : NULL;
  $events = json_decode($events_data);

  // Get array of event methods registered via hook_drpg_event_method_info().
  $event_methods = drpg_event_methods();

  $success = TRUE;

  // Loop through and process each event.
  // Break loop if any event fails as some events may depend on others.
  if (is_array($events)) {
    foreach ($events as $event) {

      // Check the event method is registered.
      if (isset($event_methods[$event->method])) {
        $event_method_info = $event_methods[$event->method];

        // Check the user has permission to process the event.
        if (!call_user_func($event_method_info['access'])) {
          watchdog('drpg_event', 'User does not have permission to process event %event.', array(
            '%event' => $event->method,
          ), WATCHDOG_ERROR);
          $success = FALSE;
          break;
        }

        // Check the event method exists and attempt to process the event.
        if (!function_exists($event->method) || !call_user_func_array($event->method, $event->arguments)) {
          watchdog('drpg_event', 'Unable to process event %event.', array(
            '%event' => $event->method,
          ), WATCHDOG_ERROR);
          $success = FALSE;
          break;
        }
      }
      else {
        // The event method is not registered.
        watchdog('drpg_event', 'Attempted to process unregistered event %event. Make sure hook_drpg_event_method_info() has been implemented.', array(
          '%event' => $event->method,
        ), WATCHDOG_ERROR);
        $success = FALSE;
        break;
      }

    }
  }

  $response = new DrpgDataResponse();
  $response->success = $success;

  drpg_json_output($response);
}
