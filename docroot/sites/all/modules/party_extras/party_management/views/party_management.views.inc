<?php
/**
 * @file
 * Views handlers for the OG-based party_management stuff.
 */

/**
 * Implements hook_views_data().
 */
function party_management_views_data() {
  $data['views']['og_manager'] = array(
    'title' => t('OG manager checkbox'),
    'help' => t('Provide a checkbox for making a certain individual an OG manager.'),
    'field' => array(
      'handler' => 'party_management_handler_field_og_manager',
    ),
  );
  $data['views']['og_manager_operations'] = array(
    'title' => t('OG manager operations'),
    'help' => t('Provide links for OG manager operations.'),
    'field' => array(
      'handler' => 'party_management_handler_field_og_manager_operations',
    ),
  );

  return $data;
}

