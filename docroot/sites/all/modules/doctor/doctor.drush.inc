<?php
/**
 * @file
 * the doctor module drush integration file.
 */

/**
 * Implements hook_drush_command().
 */
function doctor_drush_command() {
  $items = array();
  $items['doctor-listen'] = array(
    'description' => 'Activate the doctor listen functionality.',
    'options' => array(
      'timeout' => 'The amount of time in seconds that the doctor should listen to the actions in the site. 0 value will allow to the doctor listen forever(or until the server fall down/you\'ll terminate him).',
    ),
    'examples' => array(
      'drush doctor-listen --timeout=120',
      'drush doctor-listen --timeout=0',
    ),
    'aliases' => array('dols'),
  );
  return $items;
}

/**
 * Each doctor listen item that will be created, will be displayed here.
 */
function drush_doctor_listen() {
  $timeout = drush_get_option('timeout', 120);

  // Delete muted items if needed.
  if (variable_get('purge_muted_items')) {
    doctor_purge_muted_items();
  }

  $second = 0;
  $continue = TRUE;
  do {
    // Get all the doctor listener items.
    if ($items = doctor_items()) {
      foreach ($items as $item) {

        // Other modules arne't implementing any handler, use drush_print_r().
        if (!$listeners = doctor_listener_handler()) {
          drush_print_r($item->data);
        }
        else {
          foreach ($listeners as $listener) {
            call_user_func($listener['callback'], $item->data);
          }
        }
        $item->watched();
      }
    }

    if ($timeout > 0) {
      if ($timeout > $second) {
        $second++;
      }
      else {
        $continue = FALSE;
      }
    }
    sleep(1);

  } while($continue);
}
