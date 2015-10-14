<?php

/**
 * @file
 * Views integration
 */

/**
 * Implements hook_views_data_alter().
 */
function cps_views_data_alter(&$data) {
  // This uses views_data_alter() because most of the Views integration
  // is handled by entity module, but this filter is a bit special.
  $data['cps_changeset']['status_type'] = array(
    'filter' => array(
      'title' => t('Status type'),
      'help' => t('Filter by all statuses of a given type'),
      'real field' => 'status',
      'handler' => 'cps_handler_filter_status_type',
    )
  );

  // Replace entity API created handler with custom one that can reduce to just
  // a set of types.
  $data['cps_changeset']['status']['filter']['handler'] = 'cps_handler_filter_status';

  $types = cps_get_supported();
  $info = entity_get_info();
  foreach ($types as $type) {
    $entity_info = $info[$type];
    $table = $entity_info['base table'];

    $data[$table]['cps_changeset'] = array(
      'title' => t('CPS Site Version'),
      'help' => t('Relates the @label entity to the CPS changeset table through the CPS site version.', array('@label' => $entity_info['label'])),
      'relationship' => array(
        'handler' => 'cps_handler_relationship_changeset',
        'label' => t('cps'),
        'base' => 'cps_changeset',
        'entity type' => $type,
      ),
    );
  }
}
