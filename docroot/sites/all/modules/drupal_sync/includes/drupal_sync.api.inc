<?php

/**
 * @file
 * drupal_sync.api.inc
 */

/**
 * Processing incoming sync operations
 *
 * @param array $entity
 * @param string $type
 * @param string $op
 * @param string $remote_url
 * @param array $result
 *
 * @return array
 */
function drupal_sync_xmlrpc_sync_alert($entity, $type, $op, $remote_url, $result) {

  $remote_config = drupal_sync_get_remote_site_access($remote_url);

  if (empty($remote_config)) {
    $error_message = t('Incoming sync request has url  =@url, that is absent in sync configuration settings', array('@url' => $remote_url));
    watchdog('sync_error', $error_message);
    return array(
      'status' => 'error',
      'error_info' => array(
        'message' => $error_message,
      ),
    );
  }
  else {
    $remote_site_name = $remote_config['name'];
  }

  $check_result = drupal_sync_local_store_check_changes($entity, $type);
  //if this entity changes was already applied, return status ok
  if (!empty($check_result)) {
    return array(
      'status' => 'ok',
      'id' => $check_result,
    );
  }
  if (($op == 'insert' || $op == 'update')) {
    //if the same insert or update operation in progres, stop sync
    $in_progress_op = drupal_sync_get_in_progress_operation($entity['source_eid'], $entity['source_site_id'], $op, $type);
    if (!empty($in_progress_op)) {
      watchdog('drupal_sync', 'Trying to process operation that allready in proggress @entity', array('@entity' => '<pre>' . print_r($entity, TRUE) . '</pre>'));
      return FALSE;
    }
    else {
      drupal_sync_add_in_progress_operation($entity, $op, $type);
    }
  }
  $op_result = FALSE;

  $context = array('type' => $type, 'op' => $op, 'remote_site_name' => $remote_site_name);

  drupal_alter('drupal_sync_processings_incoming_operation_before', $op_result, $entity, $context);

  if (empty($op_result)) {
    switch ($type) {
      case 'node':
        switch ($op) {
          case 'insert':
            $op_result = drupal_sync_entity_node_insert($entity, $remote_site_name);
            if ($op_result['status'] == 'ok') {
              drupal_sync_local_store_insert($op_result['id'], $entity['entity_id'], $type, $remote_site_name, $entity);
            }
            drupal_sync_delete_in_progress_operation($entity, $op, $type);
            return $op_result;
            break;
          case 'update':
            $op_result = drupal_sync_entity_node_update($entity, $remote_site_name);
            if ($op_result['status'] == 'ok') {
              drupal_sync_local_store_update($op_result['id'], $entity['entity_id'], $type, $remote_site_name, $entity);
            }
            drupal_sync_delete_in_progress_operation($entity, $op, $type);
            return $op_result;
            break;
          case 'delete':
            $op_result = drupal_sync_entity_node_delete($entity, $remote_site_name);
            if ($op_result['status'] == 'ok') {
              drupal_sync_local_store_delete($op_result['id'], $entity['entity_id'], $type, $remote_site_name);
            }
            return $op_result;
            break;
        }
        break;
      case 'taxonomy_term':
        switch ($op) {
          case 'insert':
            $op_result = drupal_sync_entity_term_insert($entity, $remote_site_name);
            if ($op_result['status'] == 'ok') {
              drupal_sync_local_store_insert($op_result['id'], $entity['entity_id'], $type, $remote_site_name, $entity);
            }
            drupal_sync_delete_in_progress_operation($entity, $op, $type);
            return $op_result;
            break;
          case 'update':
            $op_result = drupal_sync_entity_term_update($entity, $remote_site_name);
            if ($op_result['status'] == 'ok') {
              drupal_sync_local_store_update($op_result['id'], $entity['entity_id'], $type, $remote_site_name, $entity);
            }
            drupal_sync_delete_in_progress_operation($entity, $op, $type);
            return $op_result;
            break;
          case 'delete':
            $op_result = drupal_sync_entity_term_delete($entity, $remote_site_name);
            if ($op_result['status'] == 'ok') {
              drupal_sync_local_store_delete($op_result['id'], $entity['entity_id'], $type, $remote_site_name);
            }
            return $op_result;
            break;
        }
        break;
    }
  }
  $context = array('type' => $type, 'op' => $op, 'remote_site_name' => $remote_site_name);
  //hook_for_processings other entities
  drupal_alter('drupal_sync_processings_incoming_operation_after', $op_result, $entity, $context);

  return isset($op_result) ? $op_result : FALSE;
}

/**
 * Insert local node.
 *
 * @param array $entity
 * @param string $remote_site_name
 *
 * @return int
 */
function drupal_sync_entity_node_insert($entity, $remote_site_name) {

  $node = new stdClass();
  $node->type = $entity['entity_content_type'];
  node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
  $node->language = LANGUAGE_NONE;
  $node->drupal_sync_is_remote = TRUE;
  $node->drupal_sync_source_eid = $entity['source_eid'];
  $node->drupal_sync_source_site_id = $entity['source_site_id'];
  $node->drupal_sync_source_changed = $entity['source_changed'];
  $node->drupal_sync_remote_site_name = $remote_site_name;

  drupal_sync_entity_add_fields($node, $entity, 'node');

  drupal_alter('drupal_sync_entity_node_insert', $entity, $node, $remote_site_name);

  try {
    node_save($node);
  } catch (Exception $e) {
    watchdog('sync_error', 'Error saving node:' . '<pre>' . 'Exception' . print_r($e->getMessage(), TRUE) . ' Node: ' . print_r($node, TRUE) . '</pre>');
    return array(
      'status' => 'error',
      'error_info' => array(
        'exception' => $e->getMessage(),
      )
    );
  }

  return array(
    'status' => 'ok',
    'id' => $node->nid,
  );
}

/**
 * Update local node.
 *
 * @param array $entity
 * @param string $remote_url
 *
 * @return int
 */
function drupal_sync_entity_node_update($entity, $remote_site_name) {

  $nid = drupal_sync_local_store_get_id_by_source($entity['source_eid'], $entity['source_site_id'], 'node');
  $node = node_load($nid);
  if (empty($node)) {
    return drupal_sync_entity_node_insert($entity, $remote_site_name);
  }

  $node->drupal_sync_is_remote = TRUE;
  $node->drupal_sync_source_eid = $entity['source_eid'];
  $node->drupal_sync_source_site_id = $entity['source_site_id'];
  $node->drupal_sync_source_changed = $entity['source_changed'];
  $node->drupal_sync_remote_site_name = $remote_site_name;

  drupal_sync_entity_add_fields($node, $entity, 'node');

  try {
    node_save($node);
  } catch (Exception $e) {
    watchdog('sync_error', 'Error saving node in update:' . '<pre>' . 'Exception' . print_r($e->getMessage(), TRUE) . ' Node: ' . print_r($node, TRUE) . '</pre>');
    return array(
      'status' => 'error',
      'error_info' => array(
        'exception' => $e->getMessage(),
      )
    );
  }

  return array(
    'status' => 'ok',
    'id' => $nid,
  );
}

/**
 * Delete local node.
 *
 * @param array $entity
 * @param string $remote_url
 *
 * @return int
 */
function drupal_sync_entity_node_delete($entity, $remote_url) {

  $nid = drupal_sync_local_store_get_id_by_source($entity['source_eid'], $entity['source_site_id'], 'node');
  node_delete($nid);

  return array(
    'status' => 'ok',
    'id' => $nid,
  );
}

/**
 * Insert local term.
 *
 * @param array $entity
 * @param string $remote_site_name
 *
 * @return int
 */
function drupal_sync_entity_term_insert($entity, $remote_site_name) {

  $term = new stdClass();
  $term->vid = $entity['entity_content_type'];
  $term->drupal_sync_is_remote = TRUE;
  $term->drupal_sync_source_eid = $entity['source_eid'];
  $term->drupal_sync_source_site_id = $entity['source_site_id'];
  $term->drupal_sync_source_changed = $entity['source_changed'];
  $term->drupal_sync_remote_site_name = $remote_site_name;
  drupal_sync_entity_add_fields($term, $entity, 'taxonomy_term');

  try {
    taxonomy_term_save($term);
  } catch (Exception $e) {
    watchdog('sync_error', 'Error insert term:' . '<pre>' . 'Exception' . print_r($e->getMessage(), TRUE) . ' Term: ' . print_r($term, TRUE) . '</pre>');
    return array(
      'status' => 'error',
      'error_info' => array(
        'exception' => $e->getMessage(),
      )
    );
  }

  return array(
    'status' => 'ok',
    'id' => $term->tid,
  );
}

/**
 * Update local term.
 *
 * @param array $entity
 * @param string $remote_site_name
 *
 * @return int
 */
function drupal_sync_entity_term_update($entity, $remote_site_name) {

  $tid = drupal_sync_local_store_get_id_by_source($entity['source_eid'], $entity['source_site_id'], 'taxonomy_term');

  $term = taxonomy_term_load($tid);

  if (empty($term)) {
    return drupal_sync_entity_term_insert($entity, $remote_site_name);
  }

  $term->drupal_sync_is_remote = TRUE;
  $term->drupal_sync_source_eid = $entity['source_eid'];
  $term->drupal_sync_source_site_id = $entity['source_site_id'];
  $term->drupal_sync_source_changed = $entity['source_changed'];
  $term->drupal_sync_remote_site_name = $remote_site_name;
  drupal_sync_entity_add_fields($term, $entity, 'taxonomy_term');

  try {
    taxonomy_term_save($term);
  } catch (Exception $e) {
    watchdog('sync_error', 'Error update term:' . '<pre>' . 'Exception' . print_r($e->getMessage(), TRUE) . ' Term: ' . print_r($term, TRUE) . '</pre>');
    return array(
      'status' => 'error',
      'error_info' => array(
        'exception' => $e->getMessage(),
      )
    );
  }

  return array(
    'status' => 'ok',
    'id' => $tid,
  );
}

/**
 * Delete local term.
 *
 * @param array $entity
 * @param string $remote_url
 *
 * @return int
 */
function drupal_sync_entity_term_delete($entity, $remote_url) {

  $tid = drupal_sync_local_store_get_id_by_source($entity['source_eid'], $entity['source_site_id'], 'taxonomy_term');
  taxonomy_term_delete($tid);
  return array(
    'status' => 'ok',
    'id' => $tid,
  );
}

/**
 * Attach fields from remote entity, to local entity
 *
 * @param array $entity_local
 * @param array $entity_remote
 * @param string $entity_type
 */
function drupal_sync_entity_add_fields(&$entity_local, $entity_remote, $entity_type) {

  $properties = drupal_sync_get_entity_bundle_properties($entity_type, $entity_remote['entity_content_type']);
  $types = array();
  $types = module_invoke_all('drupal_sync_entity_field_types_info', $types);
  foreach ($entity_remote['entity_fields'] as $field_key => $values) {
    $local_field_type = (isset($properties[$field_key]['type'])) ? $properties[$field_key]['type'] : '';

    if (empty($local_field_type)) {
      $field_info = field_info_field($field_key);
      $local_field_type = (isset($field_info['type'])) ? $field_info['type'] : '';
    }

    $new_field_value = NULL;
    foreach (module_implements('drupal_sync_entity_add_field') as $module) {
      $new_field_value = module_invoke($module, 'drupal_sync_entity_add_field', $field_key, $values, $local_field_type,  $entity_local, $entity_remote, $entity_type);
    }

    if (isset($new_field_value)) {
      //if entity field value provided in hook "drupal_sync_entity_add_field" implementation
      $entity_local->$field_key = $new_field_value;
    }
    elseif (!is_array($values)) {
      //if it's not array, just set value, for example node Titles
      $entity_local->$field_key = $values;
    }
    else {
      //default field values setting
      if (!empty($local_field_type)) {
        $sync_field_fields = (isset($types[$local_field_type])) ? $types[$local_field_type] : array('value');
        $local_field_values = (isset($values)) ? $values : array();
        $field_value = array();
        foreach ($local_field_values as $local_field_key => $local_field_value) {
          drupal_alter('drupal_sync_entity_fields_values', $local_field_value, $entity_remote, $entity_type);
          if (!empty($local_field_value)) {
              foreach ($sync_field_fields as $sync_field_field) {
                $field_value[LANGUAGE_NONE][$local_field_key][$sync_field_field] = array_shift($local_field_value);
              }
          }
        }
        $entity_local->$field_key = $field_value;
      }
    }
  }
  
}

/**
 * Implements hook_drupal_sync_entity_add_field().
 */
function drupal_sync_drupal_sync_entity_add_field($field_key, $values, $local_field_type, $entity_local, $entity_remote, $entity_type) {

   if ($entity_type == 'taxonomy_term' && $field_key == 'parent') {
      //provides values for taxonomy_term relations, find local tids for remote taxonomy tids
      $field_value = array();
      foreach ($values as $value) {
        if (isset($value['tid']) && is_numeric($value['tid'])) {
          if ($value['tid'] == 0) {
            $field_value[0] = 0;
          }
          else {
            if (isset($value['source_site_id'])) {
              $local_tid = drupal_sync_local_store_get_id_by_source($value['tid'], $value['source_site_id'], 'taxonomy_term');
              if (!empty($local_tid)) {
                $field_value[$local_tid] = $local_tid;
              }
            }
          }
        }
      }
      //if we cant find parent tid, setup root parent array(0 => 0)
      return !empty($field_value) ? $field_value : array(0 => 0);
    }
    if ($local_field_type == 'list<node>' || $local_field_type == 'node' ) {
      //provides values for node relations, find local ids' for remote node references
      $field_value = array();
      foreach ($values as $value) {
       if (isset($value['target_id']) && is_numeric($value['target_id'])) {
            if (isset($value['source_site_id'])) {
              $local_id = drupal_sync_local_store_get_id_by_source($value['target_id'], $value['source_site_id'], 'node');
              if (!empty($local_id)) {
                $field_value[$local_id] = $local_id;
                $field_value[] = array('target_id' => $local_id);
              }
          }
        }
      }
    }
}

/**
 * Implements hook_drupal_sync_entity_fields_values_alter().
 *
 * @param mixed $values
 * @param array $entity_remote
 * @param string $entity_type
 */
function drupal_sync_drupal_sync_entity_fields_values_alter(&$values, $entity_remote, $entity_type) {
  //Check file fields and download external files, and save as managed file
  if (!empty($values['file_url'])) {
    $local_file = drupal_sync_upload_remote_file($values['file_url'], $entity_remote, $entity_type);
    if (!empty($local_file->fid)) {
      unset($values['file_url']);
      $values['fid'] = $local_file->fid;
    }
    else {
      $values = array();
    }
  }
  //change remote term reference tid on the corresponding local term tid

  if (isset($values['tid'])) {
    if (isset($values['tid_site_source_url'])) {
      $local_tid = drupal_sync_local_store_get_id_by_source($values['tid'], $values['tid_site_source_url'], 'taxonomy_term');
    }
    if (!empty($local_tid)) {
      unset($values['tid_site_source_url']);
      $values['tid'] = $local_tid;
    }
    else {
      $values = array();
    }
  }
}

/**
 * Insert row to local sync store
 *
 * @param int $eid
 * @param int $reid
 * @param string $type
 * @param string $remote_site_name
 * @param array $entity
 */
function drupal_sync_local_store_insert($eid, $reid, $type, $remote_site_name, $entity) {
  return db_insert('drupal_sync')
          ->fields(array(
            'eid' => $eid,
            'reid' => $reid,
            'entity_type' => $type,
            'site_id' => $remote_site_name,
            'source_eid' => $entity['source_eid'],
            'source_site_id' => $entity['source_site_id'],
            'source_changed' => $entity['source_changed'],
          ))
          ->execute();
}

/**
 * Update row in local sync store
 *
 * @param int $eid
 * @param int $reid
 * @param string $type
 * @param string $remote_site_name
 * @param array $entity
 */
function drupal_sync_local_store_update($eid, $reid, $type, $remote_site_name, $entity) {
  return db_merge('drupal_sync')
          ->key(array(
            'eid' => $eid,
            'reid' => $reid,
            'site_id' => $remote_site_name,
            'entity_type' => $type,
          ))
          ->insertFields(array(
            'eid' => $eid,
            'reid' => $reid,
            'entity_type' => $type,
            'site_id' => $remote_site_name,
            'source_eid' => $entity['source_eid'],
            'source_site_id' => $entity['source_site_id'],
            'source_changed' => $entity['source_changed'],
          ))
          ->updateFields(array(
            'source_eid' => $entity['source_eid'],
            'source_site_id' => $entity['source_site_id'],
            'source_changed' => $entity['source_changed'],
          ))
          ->execute();
}

/**
 * Check if this changes was already applied, retrun local entity_id
 *
 * @param array $entity
 * @param string $type
 */
function drupal_sync_local_store_check_changes($entity, $type) {
  return db_select('drupal_sync', 's')
          ->fields('s', array('eid'))
          ->condition('entity_type', $type)
          ->condition('source_eid', $entity['source_eid'])
          ->condition('source_site_id', $entity['source_site_id'])
          ->condition('source_changed', $entity['source_changed'], '>=')
          ->execute()
          ->fetchField();
}

/**
 * Check sync operation existance, if exist - return entity id, else return FALSE
 *
 * @param array $entity
 * @param string $remote_site_id
 *
 * @return int
 */
function drupal_sync_local_store_check_existance_remote_op($entity, $remote_site_id, $type) {

  if (!empty($entity['drupal_sync_source_eid'])) {
    return db_select('drupal_sync', 's')
            ->fields('s', array('eid'))
            ->condition('site_id', $remote_site_id)
            ->condition('entity_type', $type)
            ->condition('source_eid', $entity['drupal_sync_source_eid'])
            ->condition('source_site_id', $entity['drupal_sync_source_site_id'])
            ->condition('source_changed', $entity['drupal_sync_source_changed'])
            ->execute()
            ->fetchField();
  }
  else {
    //it's local node save
    return FALSE;
  }
}

/**
 * Get sync info about local entity id
 *
 * @param int $eid
 * @param string $type
 *
 * @return array
 */
function drupal_sync_get_local_store_info($eid, $type) {
  return db_select('drupal_sync', 's')
          ->fields('s', array('source_eid', 'source_site_id'))
          ->condition('eid', $eid)
          ->condition('entity_type', $type)
          ->execute()
          ->fetchAssoc();
}

/**
 * Delete row from local sync store
 *
 * @param int $eid
 * @param int $reid
 * @param string $type
 * @param string $remote_site_name
 */
function drupal_sync_local_store_delete($eid, $reid, $type, $remote_site_name) {
  return db_delete('drupal_sync')
          ->condition('eid', $eid)
          ->condition('reid', $reid)
          ->condition('entity_type', $type)
          ->condition('site_id', $remote_site_name)
          ->execute();
}

/**
 * Delete row from local sync store
 *
 * @param int $eid
 * @param string $type
 * @param string $remote_site_name
 */
function drupal_sync_local_store_delete_remote_site($eid, $type, $remote_site_name) {
  return db_delete('drupal_sync')
          ->condition('eid', $eid)
          ->condition('entity_type', $type)
          ->condition('site_id', $remote_site_name)
          ->execute();
}

/**
 * Count entity relations in drupal sync
 *
 * @param int $eid
 * @param string $entity_type
 *
 * @return int
 */
function drupal_sync_count_relations($eid, $entity_type) {
  $query = db_select('drupal_sync', 'ds')
      ->condition('eid', $eid)
      ->condition('entity_type', $entity_type);
  $query->addExpression('count(eid)', 'ecount');
  return $query->execute()->fetchField();
}

/**
 * Get sync info about local entity id for remote site
 *
 * @param int $eid
 * @param string $type
 * @param string $remote_site_name
 *
 * @return array
 */
function drupal_sync_get_local_store_info_for_remote_site($reid, $type, $remote_site_name) {
  return db_select('drupal_sync', 's')
          ->fields('s', array('eid', 'source_eid', 'source_site_id'))
          ->condition('reid', $reid)
          ->condition('entity_type', $type)
          ->condition('site_id', $remote_site_name)
          ->execute()
          ->fetchAssoc();
}

/**
 * Delete all rows from local sync store for given local entity id
 *
 * @param int $eid
 */
function drupal_sync_local_store_delete_all_rows($eid, $entity_type) {
  return db_delete('drupal_sync')
          ->condition('eid', $eid)
          ->condition('entity_type', $entity_type)
          ->execute();
}

/**
 * Get local entity id by source id
 *
 * @param int $source_eid
 * @param int $source_site_id
 * @param string $type
 */
function drupal_sync_local_store_get_id_by_source($source_eid, $source_site_id, $type) {
  return db_select('drupal_sync', 's')
          ->fields('s', array('eid'))
          ->condition('source_eid', $source_eid)
          ->condition('source_site_id', $source_site_id)
          ->condition('entity_type', $type)
          ->execute()
          ->fetchField();
}

/**
 * Add operation to 'in_progress' table
 *
 * @param array $entity
 * @param string $op
 */
function drupal_sync_add_in_progress_operation($entity, $op, $entity_type) {
  db_insert('drupal_sync_in_progress')
      ->fields(array(
        'source_eid' => $entity['source_eid'],
        'source_site_id' => $entity['source_site_id'],
        'source_changed' => $entity['source_changed'],
        'entity_type' => $entity_type,
        'operation' => $op,
      ))
      ->execute();
}

/**
 * Return operation info if this operation in progress
 *
 * @param int $source_eid
 * @param string $source_site_id
 * @param string $op
 *
 * @return array
 */
function drupal_sync_get_in_progress_operation($source_eid, $source_site_id, $op, $entity_type) {
  return db_select('drupal_sync_in_progress', 'p')
          ->fields('p', array('source_eid', 'source_site_id', 'source_changed', 'operation'))
          ->condition('source_eid', $source_eid)
          ->condition('source_site_id', $source_site_id)
          ->condition('operation', $op)
          ->condition('entity_type', $entity_type)
          ->execute()
          ->fetchAssoc();
}

/**
 * Remove operation from in_progress table
 *
 * @param array $entity
 * @param string $op
 * @param string $entity_type
 */
function drupal_sync_delete_in_progress_operation($entity, $op, $entity_type) {
  db_delete('drupal_sync_in_progress')
      ->condition('source_eid', $entity['source_eid'])
      ->condition('source_site_id', $entity['source_site_id'])
      ->condition('entity_type', $entity_type)
      ->condition('operation', $op)
      ->execute();
}

/**
 * Get field mapping from settings
 *
 * @param string $entity_type
 * @param string $entity_type_id
 * @param string $site_url
 *
 * @return array
 */
function _drupal_sync_get_fields_refs($entity_type, $entity_type_id, $site_url) {
  $fields_refs = array();
  $remote_site_info = drupal_sync_get_remote_site_access($site_url);
  if (isset($remote_site_info['key'])) {
    $drupal_sync_relation_settings = variable_get('drupal_sync_remote_relation_settings', array());
    $site_key = $remote_site_info['key'];
    foreach ($drupal_sync_relation_settings[$entity_type] as $enity_type_id => $remote_settings) {
      if (!empty($remote_settings[$site_key]['type']) && $remote_settings[$site_key]['type'] == $entity_type_id) {
        $remote_settings[$site_key]['local_entity_type'] = $enity_type_id;
        $fields_refs[$enity_type_id] = $remote_settings[$site_key];
      }
    }
  }
  return $fields_refs;
}

/**
 * Copy external file and save as managed file
 *
 * @param string $url
 *
 * @return obj
 *   Return drupal file object
 */
function drupal_sync_upload_remote_file($url, $entity_remote, $entity_type) {
  global $user;
  $temp = file_directory_temp();
  $basename = basename($url);

  $temp_path = $temp . '/' . $basename;
  $result = copy($url, $temp_path);

  if (!$result) {
    $message = 'Error copy remote file to temporary directory';
    watchdog('drupal_sync_error', $message);
    $message .= '</br> Site: ' . $_SERVER['HTTP_HOST'];
    $message .= '</br> Remote file URL: ' . $url;
    $message .= '</br> Entity: ' . $entity_type;
    $message .= '</br> EntityType: ' . $entity_remote['entity_content_type'];
    $message .= '</br> EntityId: ' . $entity_remote['entity_id'];
    drupal_sync_send_email_notification($message);
    return FALSE;
  }
  $file = (object) array(
        'uid' => $user->uid,
        'uri' => $temp_path,
        'filemime' => file_get_mimetype($temp_path),
        'filename' => $basename,
  );

  // Move the file, into the Drupal file system
  $file = file_move($file, 'public://');
  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;

    $file = file_save($file);
  }
  else {
    $message = 'Failed to write the uploaded file to the site\'s public file folder';
    watchdog('drupal_sync_error', $message);
    $message .= '</br> Site: ' . $_SERVER['HTTP_HOST'];
    $message .= '</br> Remote file URL: ' . $url;
    $message .= '</br> Entity: ' . $entity_type;
    $message .= '</br> EntityType: ' . $entity_remote['entity_content_type'];
    $message .= '</br> EntityId: ' . $entity_remote['entity_id'];
    drupal_sync_send_email_notification($message);
    return FALSE;
  }

  return $file;
}

/**
 * Implements hook_drupal_sync_error_notification().
 */
function drupal_sync_drupal_sync_error_notification($type, $data = array()) {
  $message = "";
  switch ($type) {
    case "synchronization_error":
      $message .= t("Site: @site - synchronization error.", array("@site" => $_SERVER['HTTP_HOST']));
      break;
    case "settings_error":
      $message .= t("Site: @site has wrong settings.", array("@site" => $_SERVER['HTTP_HOST']));
      break;
  }

  $message .= "</br></br>=============================================</br>";

  foreach ($data as $key => $value) {
    $message .= $key . ": " . $value . "</br>";
  }

  watchdog("drupal_sync", $message);
  return drupal_sync_send_email_notification($message);
}

/**
 * Drypal sync: send notification email
 *
 * @param string $message
 * @return boolean
 */
function drupal_sync_send_email_notification($message = '') {
  $module = 'drupal_sync';
  $mail_token = microtime();
  $from = variable_get('site_mail', '');

  $data = variable_get('drupal_sync_settings', array());
  $to = (isset($data['drupal_sync_notification_mail'])) ? $data['drupal_sync_notification_mail'] : '';

  $subject = t("Drupal sync notification");

  if (!empty($to) && !empty($from)) {
    $message = array(
      'id' => $module . '_' . $mail_token,
      'to' => $to,
      'subject' => $subject,
      'body' => array($message),
      'headers' => array(
        'From' => $from,
        'Sender' => $from,
        'Return-Path' => $from,
      ),
    );

    $system = drupal_mail_system($module, $mail_token);
    $message = $system->format($message);
    if ($system->mail($message)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Check if drupal sync settings has right current site domain.
 *
 * @return boolean
 */
function drupal_sync_check_site_domain() {
  $drupal_sync_settings = variable_get('drupal_sync_settings', array());
  $domains = isset($drupal_sync_settings['drupal_sync_domains']['drupal_sync_domain']) ? $drupal_sync_settings['drupal_sync_domains']['drupal_sync_domain'] : array();
  foreach ($domains as $domain) {
    if ($_SERVER['HTTP_HOST'] == $domain) {
      return TRUE;
    }
  }
  return FALSE;
}