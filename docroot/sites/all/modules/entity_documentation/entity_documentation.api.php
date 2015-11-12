<?php

/**
 * @file
 * Hooks provided by Entity Documentation module.
 */

/**
 * Set Entity Documentation entity types.
 *
 * @param $types
 *   Current entity types array.
 *
 * @return array
 *   Array with new entity type in.
 */
function hook_ed_type($types) {

  // New bundle.
  $types['node'] = array(
    'name' => 'Node',
    'bundles' => array(
      'article' => array(
        'name' => 'Article',
        'description' => 'Content type with time sensitive data.',
        'entity' => 'node',
      ),
    ),
  );

  return $types;
}

/**
 * Get bundle Documentation array.
 *
 * @param $exporters
 *   Current exporters array.
 *
 * @return array
 *   Array with new exporters in.
 */
function hook_bundle_documentation($documentation_array, $entity, $bundle) {

  $node_type = node_type_get_type($bundle);
  $fields = field_info_instances($entity, $bundle);

  // Node type parameters.
  $documentation_array[$entity][$bundle]['params'] = array(
    'name' => $node_type->name,
    'description' => $node_type->description,
    'properties' => array(
      'module' => array(
        'name' => t('Module'),
        'value' => $node_type->module,
      ),
      'custom' => array(
        'name' => t('Custom'),
        'value' => $node_type->custom,
      ),
    ),
  );

  // Field columns.
  $documentation_array[$entity][$bundle]['field_columns'] = array(
    'field' => t('Field'),
    'name' => t('Name'),
    'description' => t('Description'),
    'required' => t('Required'),
  );

  // Fields.
  foreach ($fields as $field_key => $field) {
    $documentation_array[$entity][$bundle]['fields'][$field_key] = array(
      'field' => $field_key,
      'name' => $fields[$field_key]['label'],
      'description' => $fields[$field_key]['description'],
      'required' => $fields[$field_key]['required'] ? t('Yes') : t('No'),
    );
  }

  return $documentation_array;
}

/**
 * Set a new Entity Documentation exporter.
 *
 * @param $exporters
 *   Current exporters array.
 *
 * @return array
 *   Array with new exporters in.
 */
function hook_ed_exporter($exporters) {

  $exporters['json'] = array(
    'name' => 'JSON',
  );

  return $exporters;
}

/**
 * Hook to export documentation in browser.
 *
 * @param $exporter
 *   Current exporter.
 * @param $entity
 *   Entity type.
 * @param $bundle
 *   Bundle.
 */
function hook_ed_documentation_export($exporter, $entity, $bundle) {
  // Check entity_documentation_pdf.module for example.
}

/**
 * Hook to export documentation in file.
 *
 * @param $exporter
 *   Current exporter.
 * @param $entity
 *   Entity type.
 * @param $bundle
 *   Bundle.
 * @param $file
 *   File name.
 */
function hook_ed_documentation_file_export($exporter, $entity, $bundle, $file) {
  // Check entity_documentation_pdf.module for example.
}
