<?php
/**
 * @file
 * Features file for the exif_custom module.
 */

/**
 * Implements hook_features_api().
 */
function exif_custom_features_api() {
  return array(
    'exif_custom' => array(
      'name'            => t('EXIF Custom mappings'),
      'default_hook'    => 'exif_custom_export_maps',
      'feature_source'  => TRUE,
      'default_file'    => FEATURES_DEFAULTS_INCLUDED,
      'file'            => drupal_get_path('module', 'exif_custom') . '/exif_custom.features.inc',
    ),
  );
}

/**
 * Implements COMPONENT_features_export_options().
 *
 * Inform features about the available mappings in the database.
 */
function exif_custom_features_export_options() {
  return db_select('exif_custom_maps', 'ecm')
    ->fields('ecm', array('name', 'name'))
    ->execute()
    ->fetchAllKeyed();
}

/**
 * Implements COMPONENT_features_export().
 *
 * Process the features export array for mappings.
 */
function exif_custom_features_export($data, &$export, $module_name) {
  $export['dependencies']['exif_custom'] = 'exif_custom';

  foreach ($data as $component) {
    $export['features']['exif_custom'][$component] = $component;
  }

  return array();
}


/**
 * Implements COMPONENT_features_export_render().
 *
 * Render mappings as code.
 */
function exif_custom_features_export_render($module_name, $data) {
  $items = array();
  foreach ($data as $name) {
    $items[$name] = exif_custom_export_map($name);
  }

  $code = "  \$items = " . features_var_export($items, '  ') . ";\n";
  $code .= '  return $items;';

  return array('exif_custom_export_maps' => $code);
}


/**
 * Implements COMPONENT_features_revert().
 */
function exif_custom_features_revert($module) {
  exif_custom_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_enable_feature().
 */
function exif_custom_features_enable_feature($module) {
  exif_custom_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_rebuild().
 *
 * Store each exported mapping in the database.
 */
function exif_custom_features_rebuild($module) {
  $defaults = features_get_default('exif_custom', $module);
  foreach ($defaults as $map) {
    exif_custom_import_map($map);
  }
}

/**
 * Custom function for exporting mappings to code
 */
function exif_custom_export_map($name) {
  $map = array();
  //get the mid
  if ($mid = exif_custom_get_mid($name)) {
    //get name
    $name = db_select('exif_custom_maps', 'm')
      ->fields('m', array('name'))
      ->condition('m.mid', $mid)
      ->execute()
      ->fetchField();

    //get all of the field mappings
    $fields = db_select('exif_custom_mapped_fields', 'mf')
      ->fields('mf',array('exif_field', 'exif_example', 'img_field'))
      ->condition('mf.mid', $mid)
      ->execute()
      ->fetchAll();

    $map = array(
      'name'          => $name,
      'fields'        => $fields,
    );
  }

  return $map;
}

/**
 * Custom function for import mappings from code.
 */
function exif_custom_import_map($map) {
  if ($mid = exif_custom_get_mid($map['name'])) {
    //delete the old field mappings to be updated.
    db_delete('exif_custom_mapped_fields')
      ->condition('mid',$mid)
      ->execute();
  }else {
    //create new map
    db_insert('exif_custom_maps')
      ->fields(array('name'))
      ->values(array('name'  => $map['name']))
      ->execute();
    //fetch mid
    $mid = exif_custom_get_mid($map['name']);
  }

  //insert fields
  $insert = db_insert('exif_custom_mapped_fields');
  $field_names = array_keys($map['fields'][0]);
  $field_names[] = 'mid';

  $insert->fields($field_names);

  //add mid to field array
  foreach ($map['fields'] as $field) {
    $field['mid'] = $mid;
    $insert->values($field);
  }

  $insert->execute();

}
