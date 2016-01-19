<?php

/**
 * @file
 * Hooks provided by the DRPG Event module.
 */

/**
 * Provides an array of methods that can be used as events.
 *
 * Events are triggered by the game client.
 *
 * @see drpg_event_process_events()
 *
 * @return array
 *   Array of event methods indexed by method name.
 */
function hook_drpg_event_method_info() {
  $events = array();

  // Event method information is indexed by method name.
  $events['drpg_avatar_set_name_event'] = array(
    // A short description of this event method.
    'description' => 'Set the name of the currently active avatar.',
    // A callback method to determine whether the user can process this event.
    'access' => 'drpg_event_can_process_event',
  );

  return $events;
}
