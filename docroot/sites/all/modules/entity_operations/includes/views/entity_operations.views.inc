<?php
/**
 * @file entity_operations.views.inc
 * Contains Views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function entity_operations_views_plugins() {
  return array(
    'display' => array(
      'entity_operation' => array(
        'title' => t('Entity tab'),
        'help' => t("Display the view as a tab on an entity."),
        'handler' => 'entity_operations_plugin_display_operation_tab',
        'theme' => 'views_view',
        'provides entity operation' => TRUE,
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'use more' => TRUE,
        'accept attachments' => TRUE,
        'admin' => t('Entity tab'),
        'help topic' => 'display-entity',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 *
 * Add Views fields for links to entity operations that support it.
 */
function entity_operations_views_data_alter(&$data) {
  // Avoid a DX WTF if Views has its data cache disabled, and force a rebuild of
  // entity operations. Without this, the parts of Views data we supply would
  // remain stale.
  if (variable_get('views_skip_cache', FALSE)) {
    cache_clear_all('entity_operation_info', 'cache');
  }

  foreach (entity_get_info() as $entity_type => $entity_info) {
    // Skip entity types that don't have a base table.
    if (!isset($entity_info['base table'])) {
      continue;
    }

    // Only add operations to entities if their base table has Views data.
    $base_table = $entity_info['base table'];
    if (!isset($data[$base_table])) {
      continue;
    }

    // Only add an entity operations field if the entity operations UI is in
    // use.
    if (!isset($entity_info['operations ui'])) {
      continue;
    }

    $data[$base_table]['entity_operation'] = array(
      'title' => t('Entity operation link'),
      'help' => t('Displays a link to an Entity Operation tab on the entity.'),
      'field' => array(
        'real field' => $entity_info['entity keys']['id'],
        'handler' => 'entity_operations_handler_field_operation_link',
      ),
    );
  }

  // Area handler for outputting an operation.
  $data['views']['entity_operation'] = array(
    'title' => t('Entity operation'),
    'help' => t('Output an entity operation.'),
    'area' => array(
      'handler' => 'entity_operations_handler_area_operation',
    ),
  );
}
