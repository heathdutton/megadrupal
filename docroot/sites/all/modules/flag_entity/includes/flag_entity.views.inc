<?php

/**
 * @file
 * 
 */

/**
 * Implementation of hook_views_handlers().
 */
function flag_entity_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'flag') . '/includes',
    ),
    'handlers' => array(
      'flag_entity_handler_relationship_entity' => array(
        'parent' => 'views_handler_relationship',
        'file' => 'flag_entity_handler_relationships.inc',
      ),
      'flag_entity_handler_relationship_counts' => array(
        'parent' => 'views_handler_relationship',
        'file' => 'flag_entity_handler_relationships.inc',
      ),
      'flag_entity_handler_relationship_user_entity' => array(
        'parent' => 'views_handler_relationship',
        'file' => 'flag_entity_handler_relationships.inc',
      ),
      'flag_handler_field_ops' => array(
        'parent' => 'views_handler_field',
      ),
      'flag_handler_filter_flagged' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'flag_handler_argument_entity_id' => array(
        'parent' => 'views_handler_argument_numeric',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function flag_entity_views_data_alter($data) {
  $flag_definition = flag_fetch_definition();
  // Get all flagable entities
  foreach ($data as $name => $info) {
    if (in_array($name, array_keys($flag_definition)) && $flag_definition[$name]['handler'] == 'flag_entity') {
      $flag = flag_entity::factory_by_content_type($name);
      $info = $flag->get_views_info();

      $data[$name]['flag_content_rel'] = array(
        'group' => t('Flags'),
        'title' => t('Entity flag'),
        'help' => t('Limit results to only those nodes flagged by a certain flag; Or display information about the flag set on a entity.'),
        'relationship' => array(
          'flag type' => 'entity',
          'handler' => 'flag_handler_relationship_entity',
          'label' => 'flag',
          'base' => 'flag_content',
          'base_field' => 'content_id',
          'relationship field' => 'uid',
        ),
      );
      $data[$name]['flag_count_rel'] = array(
        'group' => t('Flags'),
        'title' => t('Entity flag counter'),
        'help' => t('Include this to gain access to the flag counter field.'),
        'relationship' => array(
          'flag type' => 'entity',
          'handler' => 'flag_handler_relationship_counts',
          'label' => 'counter',
          'base' => 'flag_counts',
          'base field' => 'content_id',
          'relationship_field' => 'uid',
        ),
      );
      $data[$name]['flag_user_content_rel'] = array(
        'group' => t('Flags'),
        'title' => t('Entity flag content'),
        'help' => t('Limit results to users that have flagged certain content.'),
        'relationship' => array(
          'base' => 'flag_content',
          'base field' => 'uid',
          'relationship field' => 'uid',
          'handler' => 'flag_handler_relationship_user_entity',
          'label' => 'user flagged content'
        ),
      );
    }
  }
}