<?php

/**
 * @file
 * Views hooks.
 */

/**
 * Implements hook_views_data_alter().
 */
function voipqueue_views_data_alter(&$data) {
  $data['advancedqueue']['status']['field']['handler'] = 'voipqueue_handler_field_status';
}
