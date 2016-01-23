<?php

/**
 * @file
 * Contains views related hooks and methods.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_role_based_global_text_views_data_alter(&$data) {
  $data['views']['area']['area']['handler'] = 'views_role_based_global_text_handler_extender';
}
