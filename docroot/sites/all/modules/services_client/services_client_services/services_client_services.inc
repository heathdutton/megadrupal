<?php

/**
 * @file
 * Services implementation.
 */

/**
 * Check if user has access to specific action.
 *
 * @param $action
 */
function _servics_client_services_user_access($action = '') {
  switch ($action) {
    case 'list_roles':
      return user_access('administer permissions');
    default:
      return FALSE;
  }
}

/**
 * List all roles available on site.
 *
 * @return array
 *   User roles keyed by role id
 *   array(rid => 'name', ...)
 */
function _servics_client_services_user_list_roles() {
  return user_roles();
}

/**
 * Check if user has access to services_client resource action.
 *
 * @param string $action
 *   Name of the action.
 *
 * @return bool
 *   TRUE if user has access.
 */
function _services_client_services_info_access($action = '') {
  switch ($action) {
    case 'entity_info':
      return user_access('administer content types');
    default:
      return FALSE;
  }
}

/**
 * Retrieve info about entities.
 *
 * @param string $entity
 *   Name of entity. 'node', 'user'
 *
 * @return array
 *   Entity description
 *   array(
 *     'name' => 'Node',
 *     'bundles' => array('page', 'article'),
 *     'fields' => array(
 *       'field_name' => array('column_1', 'column_2'),
 *     )
 *     'properties' => array(
 *       'property_name' => 'Property label',
 *       'nid' => 'Node ID',
 *     ),
 *   )
 */
function _services_client_services_entity_info($entity = NULL) {
  $entity_info = entity_get_info();
  $properties = entity_get_property_info();
  $field_info = field_info_fields();

  $out = array();

  foreach ($entity_info as $entity_type => $info) {
    $out[$entity_type] = array(
      'name' => $info['label'],
      'bundles' => array(),
      'fields' => array(),
      'properties' => array(),
    );

    // Prepare bundles
    if (!empty($info['bundle keys'])) {
      foreach ($info['bundles'] as $bundle_name => $bundle_info) {
        $out[$entity_type]['bundles'][$bundle_name] = $bundle_info['label'];
      }
    }

    // Properties
    if (!empty($properties[$entity_type]['properties'])) {
      foreach ($properties[$entity_type]['properties'] as $property_name => $property_info) {
        // Some properties may be fields
        if (!isset($field_info[$property_name])) {
          $out[$entity_type]['properties'][$property_name] = $property_info['label'];
        }
      }
    }

    // Fields
    $fields = array_filter($field_info, function ($item) use ($entity_type) {
      return isset($item['bundles'][$entity_type]);
    });

    foreach ($fields as $field_name => $field_description) {
      $out[$entity_type]['fields'][$field_name] = array(
        'columns' => array_keys($field_description['columns']),
        'bundles' => $field_description['bundles'][$entity_type],
      );
    }
  }

  if (!empty($entity)) {
    return isset($out[$entity]) ? $out[$entity] : array();
  }

  return $out;
}
