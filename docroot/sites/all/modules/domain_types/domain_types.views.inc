<?php

/**
 * @file
 * Views hooks.
 */

/**
 * Implements hook_views_data_alter().
 */
function domain_types_views_data_alter(&$data) {
  // Field: x_type.
  $data['domain']['x_type'] = array(
    'title' => t('Domain type'),
    'help' => t('The custom type of this domain.'),
    'field' => array(
      'handler' => 'views_handler_field_domain_type',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_domain_type',
    ),
  );
}
