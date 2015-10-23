<?php

/**
 * Implements hook_views_data_alter().
 */
function views_entity_rules_views_data_alter(&$data) {
  $data['views']['views_entity_rules_field_visibility_filter_handler'] = array(
    'title' => t('Field visibility rule'),
    'help' => t('Defines rule for views field visibility.'),
    'filter' => array(
      'help' => t('Defines rule for views field visibility.'),
      'handler' => 'views_entity_rules_field_visibility_filter_handler',
    ),
  );
}
