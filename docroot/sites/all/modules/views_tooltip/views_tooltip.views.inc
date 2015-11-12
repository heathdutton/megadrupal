<?php

/**
 * @file
 */

/**
 * Implements hook_views_data_alter().
 *
 * Register our tooltip field.
 */
function views_tooltip_views_data_alter(&$data) {
  $data['views']['tooltip'] = array(
    'title' => t('Tooltip field'),
    'help' => t('Displays a field with a tooltip.'),
    'field' => array(
      'handler' => 'views_tooltip_handler_field_tooltip',
    ),
  );
}
