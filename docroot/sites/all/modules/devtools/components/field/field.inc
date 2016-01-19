<?php

/**
 * Truncates table content for specified field.
 *
 * Useful for manually truncating field data before deleting the fields
 * to speed up the overall deletion process.
 *
 * @param  [type] $field_names [description]
 * @return [type] [description]
 */
function field_truncate($field_names) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    if (!is_array($field_names)) {
      $field_names = array($field_names);
    }

    foreach ($field_names as $field_name) {
      // if the admin tried to delete the fields via UI, the tables may be missing,
      // so we need to do a check here to prevent PDO errors in TRUNCATE queries
      if (db_query("SHOW TABLES LIKE :table", array(':table' => 'field_data_' . $field_name))->fetchField()) {
        db_query("TRUNCATE field_data_" . $field_name);
      }

      if (db_query("SHOW TABLES LIKE :table", array(':table' => 'field_revision_' . $field_name))->fetchField()) {
        db_query("TRUNCATE field_revision_" . $field_name);
      }
    }
  }
}

/**
 * Deletes field instance from specified entity type & bundle.
 * 
 * @param  [type] $field_name  [description]
 * @param  [type] $entity_type [description]
 * @param  [type] $bundle      [description]
 * @return [type]              [description]
 */
function field_del_instance($field_name, $entity_type, $bundle) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $instance = field_info_instance($entity_type, $field_name, $bundle);
    field_delete_instance($instance);
  }
}

/**
 * Checks that the specified field has a set value, and (optionally) that
 * the value is greater than a minimum.
 * 
 * @param  [type] $entity [description]
 * @param  [type] $field  [description]
 * @param  string $source [description]
 * @param  [type] $min    [description]
 * @return [type]         [description]
 */
function field_has_value($entity, $field, $source = 'value', $min = NULL) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    if (!is_array($entity)) {
      $entity = (array) $entity;
    }

    $lang = LANGUAGE_NONE;
    if (isset($entity['language'])) {
      $lang = $entity['language'];
    }

    if (isset($entity[$field][$lang][0][$source])) {
      if (!is_null($min)) {
        return (bool) ($entity[$field][$lang][0][$source] > $min);
      }

      return TRUE;
    }

    return FALSE;
  }
}

/**
 * Returns first value for specified field.
 * @param  [type] $entity [description]
 * @param  [type] $field  [description]
 * @param  string $source [description]
 * @param  [type] $lang   [description]
 * @return [type]         [description]
 */
function field_get_value($entity, $field, $source = 'value', $lang = LANGUAGE_NONE) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $entity = (array) $entity;

    if (isset($entity['language'])) {
      $lang = $entity['language'];
    }

    if (field_has_value($entity, $field, $source)) {
      return $entity[$field][$lang][0][$source];
    }

    return FALSE;
  }
}

/**
 * Returns first value for specified field.
 * @param  [type] $entity [description]
 * @param  [type] $field  [description]
 * @return [type]         [description]
 */
function field_entity_value($entity, $field) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    // get value via entity module's wrapper
    if (function_exists('entity_metadata_wrapper')) {
      $wrappers = &drupal_static(__FUNCTION__);
      if (!isset($wrappers[$entity->entity_type][$entity->entity_id])) {
        $wrappers[$entity->entity_type][$entity->entity_id] = entity_metadata_wrapper($entity->entity_type, $entity);
      }

      $field_obj = &$wrappers[$entity->entity_type][$entity->entity_id]->{$field};
      if (is_object($field_obj) && method_exists($field_obj, 'value')) {
        return $field_obj->value();
      }
    }

    return FALSE;
  }
}

/**
 * Sets value for specified field.
 * 
 * @param  [type]  $entity [description]
 * @param  [type]  $field  [description]
 * @param  [type]  $value  [description]
 * @param  integer $index  [description]
 * @param  string  $source [description]
 * @return [type]          [description]
 */
function field_set_value($entity, $field, $value, $index = 0, $source = 'value') {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $entity = (array) $entity;

    $lang = LANGUAGE_NONE;
    if (isset($entity['language'])) {
      $lang = $entity['language'];
    }

    $entity[$field][$lang][$index][$source] = $value;
    
    return $entity;
  }
}

/**
 * Returns the allowed values for specified field.
 * @param  [type] $field [description]
 * @return [type]        [description]
 */
function field_allowed_values($field) {
  $info = field_info_field($field);

  if (isset($info['settings']['allowed_values'])) {
    return $info['settings']['allowed_values'];
  }

  return array();
}