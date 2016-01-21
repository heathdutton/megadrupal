<?php

/**
 * @file
 * drupal_sync.queue.inc
 */

/**
 * Implements hook_drupal_sync_entity_field_types_info().
 */
function drupal_sync_drupal_sync_entity_field_types_info($types = array()) {
  $types['text_formatted'] = array('value', 'format', 'summary');
  $types['list<text_formatted>'] = array('value', 'format', 'summary');
  $types['date'] = array('value', 'value2', 'offset', 'offset2', 'timezone', 'show_todate', 'date_type', 'timezone_db');
  $types['list<date>'] = array('value', 'value2', 'offset', 'offset2', 'timezone', 'show_todate', 'date_type', 'timezone_db');
  $types['field_item_link'] = array('url', 'title');
  $types['list<field_item_link>'] = array('url', 'title');
  $types['field_item_image'] = array('fid', 'alt', 'title', 'description');
  $types['list<field_item_image>'] = array('fid', 'alt', 'title', 'description');
  $types['text'] = array('value');
  $types['list<text>'] = array('value');
  $types['node'] = array('target_id');
  $types['list<node>'] = array('target_id');
  $types['decimal'] = array('value');
  $types['list<decimal>'] = array('value');
  $types['taxonomy_term'] = array('tid');
  $types['list<taxonomy_term>'] = array('tid');
  $types['integer'] = array('value');
  $types['list<integer>'] = array('value');
  $types['tablefield'] = array('tablefield', 'value');//array('value', 'format', 'tabledata');
  $types['list<tablefield>'] = array('value', 'format', 'tabledata');
  $types['boolean'] = array('value');
  $types['list<boolean>'] = array('value');
  $types['field_item_file'] = array('fid', 'display', 'description');
  $types['list<field_item_file>'] = array('fid', 'display', 'description');
  $types['struct'] = array('value', 'value2', 'offset', 'offset2', 'timezone', 'show_todate', 'date_type', 'timezone_db', 'rrule');
  $types['list<struct>'] = array('value', 'value2', 'offset', 'offset2', 'timezone', 'show_todate', 'date_type', 'timezone_db', 'rrule');

  return $types;
}

/**
 * Add sync task to queue
 *
 * @param int $local_eid
 * @param string $type
 * @param array $entity
 * @param string $op
 *
 * @return int
 */
function drupal_sync_add_to_queue($local_eid, $type, $entity, $entity_type_id, $op, $source_changed) {

  $drupal_sync_relation_settings = variable_get('drupal_sync_remote_relation_settings', array());
  $remote_site_info = drupal_sync_get_remote_site_access_by_name($entity['remote_site_name']);
  $ref = (isset($drupal_sync_relation_settings[$type][$entity_type_id][$remote_site_info['key']]['ref'])) ? $drupal_sync_relation_settings[$type][$entity_type_id][$remote_site_info['key']]['ref'] : array();

  // if sync was set only to one side. (not selected remote content type)
  if (empty($ref)) {
    return NULL;
  }

  $opt = drupal_sync_get_entity_fields_for_sending($type, $entity_type_id, $entity, $ref);

  if (!empty($entity['drupal_sync_is_remote'])) {
    //if this entity is coming from remote site
    $source_eid = $entity['drupal_sync_source_eid'];
    $source_site_id = $entity['drupal_sync_source_site_id'];
  }
  else {
    //if it's local changes
    if ($op == 'update' || $op == 'delete') {
      $sync_entity_info = drupal_sync_get_local_store_info($local_eid, $type);
      if (!empty($sync_entity_info)) {
        //if source id allredy created, get from storage
        $source_eid = $sync_entity_info['source_eid'];
        $source_site_id = $sync_entity_info['source_site_id'];
      }
      if ($op == 'update') {
        //update source_changed time
        drupal_sync_update_source_changed_time($local_eid, $entity, $type, $source_changed);
      }
    }
  }

  //if source id is not created before, add it
  $source_eid = isset($source_eid) ? $source_eid : $local_eid;
  $source_site_id = isset($source_site_id) ? $source_site_id : drupal_sync_make_site_id_hash();

  $entity_data = array(
    'entity_id' => $local_eid,
    'entity_content_type' => $entity_type_id,
    'entity_fields' => $opt,
    'source_eid' => $source_eid,
    'source_site_id' => $source_site_id,
    'source_changed' => $source_changed,
  );

  drupal_alter('drupal_sync_add_to_queue', $entity, $entity_data, $op);

  if (!empty($entity_data)) {
    return db_insert('drupal_sync_queue')
            ->fields(array(
              'remote_site_id' => $entity['remote_site_name'],
              'entity_id' => $local_eid,
              'entity_type' => $type,
              'entity' => serialize($entity_data),
              'operation' => $op,
              'created' => REQUEST_TIME
            ))
            ->execute();

  }
  else {
    return NULL;
  }
}

/**
 * Return remote entity fields, prepared to sending to remote site
 *
 * @param string $type
 *   Entity type (node, taxonomy_term)
 * @param string $entity_type_id
 *   Type of entity example (for nodes it's content type name)
 * @param array $entity
 * @param array $ref
 *   Fields mapping
 *
 * @return array
 *   fields array
 */
function drupal_sync_get_entity_fields_for_sending($type, $entity_type_id, $entity, $ref) {
  $properties = drupal_sync_get_entity_bundle_properties($type, $entity_type_id);
  $types = array();
  $types = module_invoke_all('drupal_sync_entity_field_types_info', $types);

  $opt = array();

  foreach ($ref as $local_field => $remote_field) {
    if (!empty($remote_field)) {
      $opt[$remote_field] = array();
      $local_field_type = (isset($properties[$local_field]['type'])) ? $properties[$local_field]['type'] : NULL;

      if (empty($local_field_type)) {
        $field_info = field_info_field($local_field);
        $local_field_type = (isset($field_info['type'])) ? $field_info['type'] : NULL;
      }

      if (!empty($local_field_type)) {
        $sync_field_fields = (isset($types[$local_field_type])) ? $types[$local_field_type] : array('value');
        $local_field_values = (isset($entity[$local_field][LANGUAGE_NONE])) ? $entity[$local_field][LANGUAGE_NONE] : array();
        foreach ($local_field_values as $local_field_key => $local_field_value) {
          foreach ($sync_field_fields as $sync_field_field) {
            if (isset($local_field_value[$sync_field_field])) {
              $opt[$remote_field][$local_field_key][$sync_field_field] = $local_field_value[$sync_field_field];
            }
          }
          drupal_alter('drupal_sync_field_values_to_send', $opt[$remote_field][$local_field_key]);
        }
      }
      else {
        $opt[$remote_field] = (isset($entity[$local_field])) ? $entity[$local_field] : ''; // for titles
      }
      $alter_context = array('entity_type' => $type, 'local_field_type' => $local_field_type, 'entity' => $entity);
      drupal_alter('drupal_sync_field_to_send', $opt[$remote_field], $remote_field, $alter_context);
    }
  }

  return $opt;
}

/**
 * Implements hook_drupal_sync_field_to_send_alter().
 *
 * @param array $field
 * @param string $field_name
 * @param string $type
 */
function drupal_sync_drupal_sync_field_to_send_alter(&$field, $field_name, $context) {

  if ($context['entity_type'] == 'taxonomy_term' && $field_name == 'parent') {

    $new_field = array();
    if (is_array($field)) {
      foreach ($field as $value) {
        if ($value == 0) {
          $new_field[] = array(
            'tid' => 0,
          );
        }
        else {
          if (is_numeric($value)) {
            //if this term from remote site, change tid on remote sorce tid and add site source id
            $tid_source_info = drupal_sync_get_local_store_info($value, $context['entity_type']);
            if (!empty($tid_source_info)) {
              $new_field[] = array(
                'tid' => $tid_source_info['source_eid'],
                'source_site_id' => $tid_source_info['source_site_id'],
              );
            }
          }
        }
      }
    }
    $field = $new_field;
  }
  if ($context['local_field_type'] == 'list<node>' || $context['local_field_type'] == 'node') {
    //for entity reference fields
    if (!empty($field) && is_array($field)) {
      $new_field = array();
      foreach ($field as $delta => $value) {
        $eid_source_info = drupal_sync_get_local_store_info($value['target_id'], $context['entity_type']);
        if (!empty($eid_source_info)) {
            $new_field[] = array(
                'target_id' => $eid_source_info['source_eid'],
                'source_site_id' => $eid_source_info['source_site_id'],
              );
        }
      }
    }
  }
}

/**
 * Implements hook_drupal_sync_field_values_to_send_alter().
 *
 * @param array $field
 */
function drupal_sync_drupal_sync_field_values_to_send_alter(&$field) {
  // add file url
  if (isset($field['fid'])) {
    $fid = $field['fid'];
    $file = file_load($fid);
    $field['file_url'] = file_create_url($file->uri);
  }

  if (isset($field['tid']) && is_numeric($field['tid'])) {
    //if this term from remote site, change tid on remote sorce tid and add site source url
    $tid_source_info = drupal_sync_get_local_store_info($field['tid'], 'taxonomy_term');

    if (!empty($tid_source_info)) {
      $field['tid'] = $tid_source_info['source_eid'];
      $field['tid_site_source_url'] = $tid_source_info['source_site_id'];
    }
    else {
      $field['tid_site_source_url'] = drupal_sync_make_site_id_hash();
    }
  }
}

/**
 * Get sync task from queue
 *
 * @return array
 */
function drupal_sync_get_from_queue() {
  $data = variable_get('drupal_sync_settings', array());
  $drupal_sync_queue_update_count = (isset($data['drupal_sync_queue_update_count'])) ? $data['drupal_sync_queue_update_count'] : 10;
  return db_select('drupal_sync_queue', 'dsq')
          ->fields('dsq', array())
          ->condition('status', 'awaiting')
          ->range(0, $drupal_sync_queue_update_count)
          ->orderBy('id', 'ASC')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);
}

/**
 * Delete sync task from queue
 *
 * @param int $id
 *
 * @return int
 */
function drupal_sync_delete_from_queue($id) {
  return db_delete('drupal_sync_queue')
          ->condition('id', $id)
          ->execute();
}

/**
 * Delete from queue by fileters
 *
 * @param string $entity_type
 * @param string $remote_site_id
 * @param int $entity_id
 * @param string $operation
 * @param string $status
 *
 * @return type
 */
function drupal_sync_delete_from_queue_by_filter($entity_type, $remote_site_id = NULL, $entity_id = NULL, $operation = NULL, $status = NULL) {

  $query = db_delete('drupal_sync_queue')
          ->condition('entity_type', $entity_type);

  if (!empty($remote_site_id)) {
    $query->condition('remote_site_id', $remote_site_id);
  }

  if (!empty($entity_id)) {
    $query->condition('entity_id', $entity_id);
  }

  if (!empty($operation)) {
    $query->condition('operation', $operation);
  }

  if (!empty($status)) {
    $query->condition('status', $status);
  }

  if (!empty($created)) {
    $query->condition('created', $created);
  }

  return $query->execute();
}

/**
 * Update source_changed time
 *
 * @param int $local_eid
 * @param array $entity
 * @param int $source_changed
 */
function drupal_sync_update_source_changed_time($local_eid, $entity, $type, $source_changed) {
  db_update('drupal_sync')
      ->fields(array('source_changed' => $source_changed))
      ->condition('eid', $local_eid)
      ->condition('site_id', $entity['remote_site_name'])
      ->condition('entity_type', $type)
      ->execute();
}

/**
 * Set error status
 *
 * @param int $id
 *
 * @return int
 */
function drupal_sync_queue_set_error_status($id) {
  return db_update('drupal_sync_queue')
          ->fields(array(
            'status' => 'error',
          ))
          ->condition('id', $id)
          ->execute();
}

/**
 * Run sync operation
 */
function drupal_sync_run_sync() {
  if (lock_acquire('drupal_sync_run_sync', 60)) {
    $queue_entities = drupal_sync_get_from_queue();
    foreach ($queue_entities as $queue_entity) {
      $entity = unserialize($queue_entity['entity']);

      // send entity
      $responce = drupal_sync_xmlrpc_sync_entity($queue_entity['remote_site_id'], $queue_entity['entity_type'], $entity, $queue_entity['operation']);

      if ($responce && !empty($responce['status']) && $responce['status'] == 'ok') {
        $remote_eid = $responce['id'];
        switch ($queue_entity['operation']) {
          case 'insert':
            drupal_sync_local_store_insert($queue_entity['entity_id'], $remote_eid, $queue_entity['entity_type'], $queue_entity['remote_site_id'], $entity);
            break;
          case 'update':
            drupal_sync_local_store_update($queue_entity['entity_id'], $remote_eid, $queue_entity['entity_type'], $queue_entity['remote_site_id'], $entity);
            break;
        }

        drupal_sync_delete_from_queue($queue_entity['id']);
      }
      else {
        //if status == error
        $error_info = array(
          'message' => !empty($responce) ? t('Error synchronize entity') : t('Error sending synchronize xmlrpc request (empty responce)'),
          'sync_id' => $queue_entity['id'],
          'Remote site name' => $queue_entity['remote_site_id'],
        );

        drupal_sync_queue_set_error_status($queue_entity['id']);

        module_invoke_all('drupal_sync_error_notification', 'synchronization_error', $error_info);

        $error_info['remote_error_info'] = !empty($responce['error_info']) ? $responce['error_info'] : '';
        watchdog('sync_error', '<pre>' . print_r($error_info, TRUE) . '</pre>');
      }
    }
    lock_release('drupal_sync_run_sync');
  }
  else {
    watchdog('drupal_sync', 'An attempt to run a "drupal_sync_run_sync" function that is already running.');
  }
}

