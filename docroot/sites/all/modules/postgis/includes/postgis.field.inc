<?php

/**
 * @file
 * Field for geospatial data using the Field API.
 */

/**
 * Implements hook_field_info().
 */
function postgis_field_info() {

  $default = module_exists('openlayers') ? 'openlayers' : 'wkt';

  return array(
    'postgis' => array(
      'label' => t('Geospatial data'),
      'description' => t('This field stores geospatial data in the database.'),
      'settings' => array(
        'type' => 'point',
        'srid' => POSTGIS_DEFAULT_SRID,
      ),
      'instance_settings' => array(),
      'default_widget' => $default,
      'default_formatter' => $default,
      'property_type' => 'postgis',
      'property_callbacks' => array('postgis_property_info_callback'),
    ),
  );
}

/**
 * Callback to alter the property info of postgis fields.
 *
 * @see postgis_field_info().
 */
function postgis_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];
  $property['type'] = ($field['cardinality'] != 1) ? 'list<postgis>' : 'postgis';
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'postgis_field_property_set';
  $property['property info'] = array(
    'wkt' => array(
      'type' => 'postgis',
      'label' => t('Well-known text'),
      'getter callback' => 'postgis_property_get_wkt',
      'required' => TRUE,
    ),
  );

  unset($property['query callback']);
}

/**
 * Callback for getting the file entity of file fields.
 */
function postgis_property_get_wkt($item) {
  $geometry = new PostgisGeometry(NULL, NULL);
  $geometry->fromGeometry($item['geometry']);
  return $geometry->getText();
}

/**
 * Writes the passed field items in the object and set the transformed option.
 *
 * Based on entity_metadata_field_verbatim_set().
 */
function postgis_field_property_set($entity, $name, $items, $langcode, $entity_type) {
  $field = field_info_field($name);
  $langcode = entity_metadata_field_get_language($entity_type, $entity, $entity_type, $langcode);
  $value = $field['cardinality'] == 1 ? array($items) : (array) $items;
  // Filter out any items set to NULL.
  $entity->{$name}[$langcode] = array_filter($value);

  // Geometry comes in right projection, so set the transformed option.
  foreach ($entity->{$name}[$langcode] as &$item) {
    $item['transformed'] = TRUE;
  }

  // Empty the static field language cache, so the field system picks up any
  // possible new languages.
  drupal_static_reset('field_language');
}

/**
 * Form that is shown when a postgis field is added to a content type.
 *
 * Implements hook_field_settings_form().
 */
function postgis_field_settings_form($field, $instance, $has_data) {

  $types = array(
    POSTGIS_PRIMITIVE_POINT => t('Point'),
    POSTGIS_PRIMITIVE_LINESTRING => t('Linestring'),
    POSTGIS_PRIMITIVE_POLYGON => t('Polygon'),
    POSTGIS_PRIMITIVE_MULTIPOINT => t('Multi Point'),
    POSTGIS_PRIMITIVE_MULTILINESTRING => t('Multi Linestring'),
    POSTGIS_PRIMITIVE_MULTIPOLYGON => t('Multi Polygon'),
    POSTGIS_PRIMITIVE_GEOMETRYCOLLECTION => t('Geometry Collection'),
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Geometry type'),
    '#options' => $types,
    '#default_value' => $field['settings']['type'],
    '#description' => t('Choose the geometry that best describes the information you are storing. "Point" is suitable for addresses and other fixed points, but you may also choose "Linestring" for lines or tracks, and "Polygon" for shapes and areas. If you have different types of geometry choose "Geometry Collection". To store multiple geometries of the same type in one table row the "Multi" geometry types and "Geometry Collection" are available.'),
    '#disabled' => $has_data,
  );

  $form['srid'] = array(
    '#type' => 'textfield',
    '#title' => t('SRID'),
    '#default_value' => $field['settings']['srid'],
    '#description' => t('Select the map projection that shall be used to map the globe to a plain surface. In general 4326 fits the whole globe pretty well but local projections might give more precise results for specific areas (at the cost of more distortions in other areas).'),
    '#disabled' => $has_data,
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements hook_field_validate().
 */
function postgis_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    // Catch empty input.
    $wkt = isset($item['wkt']) ? $item['wkt'] : '';

    $set = new PostgisGeometrySet($field['settings']['type'], $field['settings']['srid'], $field['cardinality']);
    $set->fromText($wkt);

    $validation_error = $set->validate($field['field_name']);
    // Add validation error if validation returned an array with description.
    if (is_array($validation_error)) {
      $errors[$field['field_name']][$langcode][$delta][] = $validation_error;
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function postgis_field_is_empty($item, $field) {

  $set = new PostgisGeometrySet($field['settings']['type'], $field['settings']['srid']);
  $set->fromText($item['wkt']);

  return $set->isEmpty();
}


/**
 * Combines entries from multiple items in a single widgets
 * into a single collection.
 *
 * Implements hook_field_presave().
 */
function postgis_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  $type = $field['settings']['type'];

  if ($instance['widget']['type'] === 'openlayers' && !isset($items[0]['feeds'])) {
    $map = openlayers_map_load($instance['widget']['settings']['map']);
    $srid = $map->data['projection'];
    if (!is_numeric($srid) && (strpos($srid, ':') != -1)) {
      $srid = substr($srid, 1+strpos($srid, ':'));
    }
  } else {
    $srid = $field['settings']['srid'];
  }

  $set = new PostgisGeometrySet($type, $srid);
  foreach ($items as $delta => $item) {
    // Handle WKT as well as WKB from mass updates of nodes.
    if (isset($item['wkt']) && is_string($item['wkt'])) {
      $set->fromText($item['wkt']);
    }
    else {
      $set->fromGeometry(array($item));
    }
  }

  if ($instance['widget']['type'] === 'openlayers' && !isset($item['feeds'])) {
    $set->transform($field['settings']['srid']);
  }

  switch ($type) {
    case POSTGIS_PRIMITIVE_GEOMETRYCOLLECTION:
      $wkt = $set->getText();
      $geometry = new PostgisGeometry($type, $field['settings']['srid']);
      $geometry->fromText($wkt);
      $items = array(array(
        'wkt' => $wkt,
        'geometry' => $geometry->getGeometry(),
      ),);
      break;
    default:
      $items = $set->getItems();
  }
}

/**
 * Keep table "geometry_columns" up to date.
 */
function postgis_field_add_geometry_column($field) {

  // check if the field to be updated is really a postgis field
  $fieldtype = empty($field['type']) ? NULL : $field['type'];
  if ($fieldtype != 'postgis') {
    return;
  }

  $srid = empty($field['settings']['srid']) ? NULL : $field['settings']['srid'];
  if (empty($srid)) {
    watchdog('postgis', t('PostGIS fields require a srid parameter. None given.'),
            array(), WATCHDOG_ERROR);
    return;
  }

  $type = drupal_strtoupper(empty($field['settings']['type']) ? '' : $field['settings']['type']);
  if (empty($type)) {
    watchdog('postgis', t('PostGIS fields require a geometry type. None given.'),
            array(), WATCHDOG_ERROR);
    return;
  }

  $storage = field_sql_storage_field_storage_details($field);
  $connection = Database::getConnection();

  foreach ($storage['sql'] as $tables) {

    foreach ($tables as $table => $columns) {

      // Use prefix as schema and/or table prefix.
      $prefix = $connection->tablePrefix($table);
      if ($pos = strpos($prefix, '.')) {
        $schema = substr($prefix, 0, $pos);
        $prefix_table = substr($prefix, $pos + 1, strlen($prefix)) . $table;
      } else {
        $schema = '';
        $prefix_table = $prefix . $table;
      }

      foreach ($columns as $field_type => $column) {

        db_drop_field($table, $column);

        db_query(
          'SELECT AddGeometryColumn(:schema, :table, :column, :srid, :type, 2)',
          array(
            ':schema' => $schema,
            ':table'  => $prefix_table,
            ':column' => $column,
            ':srid'   => $srid,
            ':type'   => $type,
          )
        );

        db_query('CREATE INDEX ' . $table . '_' . $column . '_idx ON {' . $table . '} USING GIST ( ' . $column . ' )');
      }
    }
  }
}

/**
 * Implements hook_field_create_field().
 *
 * Keep table "geometry_columns" up to date.
 */
function postgis_field_create_field($field) {
  postgis_field_add_geometry_column($field);
}

/**
 * Implements hook_field_update_field().
 *
 * Keep table "geometry_columns" up to date.
 */
function postgis_field_update_field($field, $prior_field, $has_data) {

  if($has_data && $field['type'] === 'postgis') {
    watchdog('postgis', t('PostGIS fields containing data can not be updated.'),
            array(), WATCHDOG_WARNING);
  } elseif ($field['type'] === 'postgis') {
    postgis_field_add_geometry_column($field);
  }

  // Recreate GeoServer PostGIS field views.
  if (module_exists('geoserver')) {
    $layers = geoserver_layers_load();
    foreach($layers as $layer) {
      $source = $layer->data['source'];
      if (substr($source, strrpos($source, '.')+1) === $field['field_name']) {
        $layer->data['options']['srid'] = $field['settings']['srid'];
      }
      if ($layer->data['type'] === 'geoserver_layer_type_postgis_field') {
        $layer->save();
      }
    }
  }
}

/**
 * Implements hook_field_update_forbid().
 *
 * Drop GeoServer PostGIS field views, because they depend on the PostGIS field.
 *
 */
function postgis_field_update_forbid($field, $prior_field, $has_data) {
  if (module_exists('geoserver')) {
    $layers = geoserver_layers_load();
    foreach($layers as $layer) {
      $source = $layer->data['source'];
      if ($layer->data['type'] === 'geoserver_layer_type_postgis_field') {
        db_query('DROP VIEW IF EXISTS {' . $layer->data['options']['sql_name'] . '}');
      }
    }
  }
}

/**
 * Get spatial extent of a given field.
 */
function postgis_field_extent($field) {
  if ($field['type'] === 'postgis') {
    $connection = Database::getConnection();
    $prefix = $connection->tablePrefix('field_data_' . $field['field_name']);
    $extent = db_query('SELECT ST_Extent(' . $field['field_name'] . '_geometry) from ' . $prefix . 'field_data_' . $field['field_name'] . ';')->fetchField();
    $bbox = preg_split('/[\, ]/', drupal_substr($extent, 4, -1));
    return (object) array(
      'minx' => $bbox[0],
      'miny' => $bbox[1],
      'maxx' => $bbox[2],
      'maxy' => $bbox[3],
    );
  }
}
