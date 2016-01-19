<?php

/**
 * @file
 * Views hooks implementations.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_fields_combine_views_data_alter(&$data) {
  $data['views']['fields_combine'] = array(
    'title' => t('Combined fields'),
    'help' => t('Combine the output of multiple fields.'),
    'field' => array(
      'handler' => 'views_fields_combine_handler_field_combine',
    ),
  );
}
