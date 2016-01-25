<?php

/**
 * @file
 * Provides database interactions tools for entities.
 */

/**
 * Executes a filter query against the provided attributes.
 *
 * Also used to filter entity list pages.
 *
 * @param string $entity_type
 *   Entity type to filter by,
 * @param array $header
 *   Header array for sorting. Should match the table header used in output.
 * @param string $bundle
 *   Entity bundle.
 * @param array $properties
 *   Associative array of key/value pairs.
 * @param array $fields
 *   Entity fields to filter by.
 * @param int $items_page
 *   Results per page.
 *
 * @return array
 *   Executed query object.
 */
function _entityspice_filter_query($entity_type, $header, $bundle = '', $properties = array(), $fields = array(), $items_page = 10) {
  $has_revision = _entityspice_getinfo($entity_type, 'has revision');

  $query = new EntityFieldQuery();

  $query
    ->entityCondition('entity_type', $entity_type, '=')
    ->tablesort($header)
    ->pager($items_page);

  if (!empty($bundle) && $has_revision) {
    $query->entityCondition('bundle', $bundle, '=');
  }

  // Add property filters.
  foreach ($properties as $key => $value) {
    $property_info = entity_get_all_property_info($entity_type);
    if ($value != '') {
      // Processing for date ranges.
      if ($property_info[$key]['type'] == 'date') {
        if ($value['from'] != '') {
          $conditions = array(
            strtotime($value['from']),
            // Add 24 hours for full day search.
            strtotime($value['to']) + 86400,
          );
          $query->propertyCondition($key, $conditions, 'BETWEEN');
        }
      }
      else {
        // NULL operator will allow literal or array for $value.
        $operator = NULL;
        if (isset($property_info[$key]['filter_operator']) && !is_array($value)) {
          $operator = $property_info[$key]['filter_operator'];
        }
        $query->propertyCondition($key, $value, $operator);
      }
    }
  }
  // Add field filters.
  foreach ($fields as $key => $field) {
    $field_info = field_info_field($key);

    switch ($field_info['type']) {
      // Special handling for date fields, which have a two-value range.
      case 'date':
      case 'datestamp':
      case 'datetime':
        if ($field[LANGUAGE_NONE][0]['value'] != '' && $field[LANGUAGE_NONE][0]['value2'] != '') {
          $value = array(
            $field[LANGUAGE_NONE][0]['value'],
            $field[LANGUAGE_NONE][0]['value2'],
          );
          $query->fieldCondition($key, 'value', $value, 'BETWEEN', $key);
        }
        break;

      case 'list_boolean':
        foreach ($field[LANGUAGE_NONE][0] as $column => $value) {
          // Accommodate the skip option introduced in _entityspice_filter_form().
          if ($value != 'skip') {
            $query->fieldCondition($key, $column, (int) $value, '=', $key);
          }
        }
        break;

      default:
        // Accept only valid columns. Some field types add non-column data.
        $field[LANGUAGE_NONE][0] = array_intersect_key($field[LANGUAGE_NONE][0], $field_info['columns']);
        foreach ($field[LANGUAGE_NONE][0] as $column => $value) {
          // Only accept fields for which a value has been entered.
          if ($value != '') {
            $query->fieldCondition($key, $column, $value, 'CONTAINS', $key);
          }
        }
        break;
    }
  }

  return $query->execute();
}

/**
 * Populate database schema array for entity.
 *
 * @param string $etype
 *   Machine name of entity type, Will be name of base table.
 */
function _entityspice_entity_schema($etype, array $additional_fields = array(), array $overrides = array()) {
  $info = _entityspice_getinfo_override($etype, $overrides);

  $idkey          = $info['idkey'];
  $revision_key   = $info['ridkey'];
  $revision_table = $info['revision table'];;
  $bundle_idkey   = $info['bundle idkey'];
  $bundle_table   = $info['bundle table'];
  $bundle_key     = $info['bundle key name'];
  $base_table     = $info['base table'];

  $schema[$base_table] = array(
    'description' => "The base table for $etype entity.",
    'primary key' => array($idkey),
  );

  $schema[$base_table]['fields'] = $additional_fields + array(
    $idkey => array(
      'description' => 'Primary key of the entity.',
      'type' => 'serial',
      'unsigned' => TRUE,
      'not null' => TRUE,
    ),

    'uid' => array(
      'description' => 'Owner of the entity.',
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
    ),


    'created' => array(
      'description' => 'The Unix timestamp of the entity creation time.',
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0,
    ),
  );

  if($info['has title']) {
    $schema[$base_table]['fields']['title'] = array(
      'description' => 'Title',
      'type' => 'varchar',
      'length' => '255',
      'not null' => TRUE,
      'default' => '',
    );
  }

  if($info['has revision']) {
    $schema[$base_table]['fields']['updated'] = array(
      'description' => 'The Unix timestamp when the entity was most recently saved.',
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0,
    );

    $schema[$base_table]['fields'][$revision_key] = array(
      'description' => 'The primary identifier for version.',
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
    );

    $schema[$revision_table] = $schema[$base_table];

    $schema[$revision_table]['description'] = "Stores information about each saved version of a $etype.";
    $schema[$revision_table]['primary key'] = [$revision_key];

    $schema[$revision_table]['fields'][$idkey]['type'] = 'int';
    $schema[$revision_table]['fields'][$revision_key]['type'] = 'serial';
    $schema[$revision_table]['fields']['log'] = array(
      'description' => 'The log entry explaining the changes in this version.',
      'type' => 'text',
      'not null' => FALSE,
      'size' => 'big',
    );
  }

  if($info['has bundle']) {
    $schema[$base_table]['fields'][$bundle_key] = array(
      'description' => 'The bundle type',
      'type' => 'text',
      'size' => 'medium',
      'not null' => TRUE,
    );

    $schema[$revision_table]['fields'][$bundle_key] = array(
      'description' => 'The bundle type',
      'type' => 'text',
      'size' => 'medium',
      'not null' => TRUE,
    );

    $schema[$bundle_table] = array(
      'description' => "Stores information about all defined $etype bundles.",
      'fields' => array(
        $bundle_idkey => array(
          'type' => 'serial',
          'not null' => TRUE,
          'description' => 'Primary Key: Unique bundle ID.',
        ),

        'name' => array(
          'description' => 'The machine-readable name of entity bundle.',
          'type' => 'varchar',
          'length' => 32,
          'not null' => TRUE,
        ),

        'label' => array(
          'description' => 'The human-readable name of this entity bundle.',
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),

        'weight' => array(
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
          'size' => 'tiny',
          'description' => 'The weight of this bundle in relation to others.',
        ),

        'locked' => array(
          'description' => 'A boolean indicating whether the administrator may delete this type.',
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
          'size' => 'tiny',
        ),

        'data' => array(
          'type' => 'text',
          'not null' => FALSE,
          'size' => 'big',
          'serialize' => TRUE,
          'description' => 'A serialized array of additional data related to this bundle.',
          'merge' => TRUE,
        ),

        'status' => array(
          'type' => 'int',
          'not null' => TRUE,
          // Set the default to ENTITY_CUSTOM without using the constant as it is
          // not safe to use it at this point.
          'default' => 0x01,
          'size' => 'tiny',
          'description' => 'The exportable status of the entity.',
        ),

        'module' => array(
          'description' => 'The name of the providing module if the entity has been defined in code.',
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
        ),
      ),

      'primary key' => array($bundle_idkey),

      'unique keys' => array('name' => array('name')),
    );
  }

  return $schema;
}
