<?php

/**
 * @file
 * Include file for all CRUD related functions.
 *
 * @author skilip
 */

/**
 * Get all page_breaks.
 *
 * @param $entity_type
 *   The name of the entity.
 * @param $bundle
 *   The name of the bundle.
 * @param $view_mode
 *   The view mode.
 * @param $reset.
 *   Whether to reset the cache or not.
 */
function page_break_field_info_page_breaks($entity_type = NULL, $bundle = NULL, $view_mode = NULL, $reset = FALSE) {
  $page_breaks = &drupal_static(__FUNCTION__);

  if (empty($page_breaks) || $reset) {
    if (!$reset && $cached = cache_get('page_break_fields', 'cache_field')) {
      $page_breaks = $cached->data;
    }
    else {
      $page_breaks = page_break_field_read_page_breaks();
      cache_set('page_break_fields', $page_breaks, 'cache_field');
    }
  }

  if (!isset($entity_type)) {
    return $page_breaks;
  }
  else {
    if (isset($page_breaks[$entity_type][$bundle][$view_mode])) {
      return $page_breaks[$entity_type][$bundle][$view_mode];
    }
    else {
      return array();
    }
  }
}

/**
 * Read all page_breaks.
 */
function page_break_field_read_page_breaks($params = array()) {
  $page_breaks = array();
  ctools_include('export');

  $records = ctools_export_load_object('page_break_field', 'conditions', $params);
  foreach ($records as $page_break) {
    $name = @array_shift(explode('|', $page_break->identifier));
    $page_breaks[$page_break->entity_type][$page_break->bundle][$page_break->mode][$name] = page_break_field_unpack($page_break);
  }
  return $page_breaks;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function page_break_field_ctools_plugin_api($owner, $api) {
  if ($owner == 'page_break_field' && $api == 'page_break_field') {
    return array('version' => 1);
  }
}

/**
 * page_break_field_save().
 *
 * Saves a page_break definition.
 * This function is called by ctools export when calls are made
 * through ctools_export_crud_save().
 *
 * @param $page_break
 *   A page_break definition.
 */
function page_break_field_save(& $page_break) {

  // Prepare the record.
  $object = page_break_field_pack($page_break);

  if (isset($object->export_type) && $object->export_type & EXPORT_IN_DATABASE) {
    // Existing record.
    $update = array('id');
    module_invoke_all('page_break_field_update_page_break_field', $object);
  }
  else {
    // New record.
    $update = array();
    $object->export_type = EXPORT_IN_DATABASE;
    module_invoke_all('page_break_field_create_page_break_field', $object);
  }
  return drupal_write_record('page_break_field', $object, $update);

}

/**
 * Unpacks a database row in a FieldGroup object.
 * @param $page_break
 *   Database result object with stored page_break data.
 * @return $page_break
 *   Field page_break object.
 */
function page_break_field_unpack($page_break) {

  // Extract unserialized data.
  if (isset($page_break->data)) {
    $data = $page_break->data;
    unset($page_break->data);
    $page_break->label = $data['label'];
    $page_break->weight = $data['weight'];
  }

  return $page_break;

}

/**
 * Packs a FieldGroup object into a database row.
 * @param $page_break
 *   FieldGroup object.
 * @return $record
 *   Database row object, ready to be inserted/update
 */
function page_break_field_pack($page_break) {

  $record = clone $page_break;
  $record->data = array(
    'label' => $record->label,
    'weight' => $record->weight,
  );
  if (isset($record->format_settings)) {
    $record->data['format_settings'] = $record->format_settings;
  }
  return $record;
}
