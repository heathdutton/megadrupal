<?php

/**
 * @file
 * File for Views hooks.
 */

/**
 * Implements hook_views_data_alter().
 */
function identicon_views_data_alter(&$data) {
  $data['users']['picture']['field']['handler'] = 'identicon_handler_field_user_picture';
}
