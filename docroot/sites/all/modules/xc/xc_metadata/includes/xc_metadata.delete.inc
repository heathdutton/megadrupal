<?php
/**
 * @file
 * Functions related to metadata deletions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Metadata deletion form
 */
function xc_metadata_delete_form($form, &$form_state) {
  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instructions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['instructions']['how'] = array(
    '#type' => 'item',
    '#title' => t('How it works'),
    '#markup' => t('This batch process <strong>completely deletes and removes</strong> all entity and node from Drupal based on the selected criteria.'),
  );

  $form['types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $type_options = array('all' => t('<em>Delete all types</em>'));
  foreach (xc_metadata_get_entities() as $key => $value) {
    $type_options[$key] = empty($value['title']) ? $key : $value['title'];
  }
  $form['types']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Entity types to delete'),
    '#options' => $type_options,
    '#default_value' => array('all'),
    '#required' => TRUE,
  );

  $form['locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Storage locations'),
    '#description' => t('This option only works for enities with explicit locations declared, such as entities imported with Import/Export module. It does not work for entities for with its locations defined by its source, such as those imported by the OAI Harvester.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $location_options = array('all' => t('<em>Delete from all locations</em>'));
  foreach (xc_location_get_all() as $location_id => $location) {
    $location_options[$location_id] = empty($location->name)
      ? 'Location #' . $location_id
      : $location->name;
  }
  $form['locations']['locations'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Storage locations of entities to delete'),
    '#options' => $location_options,
    '#default_value' => array('all'),
    '#required' => TRUE,
  );

  $form['created'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date created'),
    '#description' => t('Range dates entities to delete were created. Leave blank for all dates.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['created']['use_date_range'] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit by the following date range'),
    '#default_value' => 0,
  );

  $form['created']['from'] = array(
    '#type' => 'date',
    '#title' => t('From'),
    '#default_value' => array(
      'year' => 1970,
      'month' => 1,
      'day' => 1,
    ),
  );

  $form['created']['to'] = array(
    '#type' => 'date',
    '#title' => t('To'),
  );

  $form['updated'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date modified or updated'),
    '#description' => t('Range dates entities to delete were last modified or updated. Leave blank for all dates.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['updated']['use_date_range'] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit by the following date range'),
    '#default_value' => 0,
  );

  $form['updated']['from'] = array(
    '#type' => 'date',
    '#title' => t('From'),
    '#default_value' => array(
      'year' => 1970,
      'month' => 1,
      'day' => 1,
    ),
  );

  $form['updated']['to'] = array(
    '#type' => 'date',
    '#title' => t('To'),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['advanced']['batch_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of entity/nodes processed in a batch'),
    '#description' => t('Set appropriately to what your system resources can handle.'),
    '#multiple' => FALSE,
    '#options' => array(
      5 => 5,
      25 => 25,
      50 => 50,
      100 => 100,
      200 => 200,
      500 => 500,
      1000 => 1000,
      2000 => 2000,
      5000 => 5000,
    ),
    '#default_value' => 200,
    '#required' => TRUE,
  );

  $form['confirm'] = array(
    '#type' => 'value',
    '#value' => FALSE,
  );

  $form['warning'] = array(
    '#type' => 'item',
    '#prefix' => '<div class="messages error">',
    '#suffix' => '</div>',
    '#markup' => t('<center>Are you sure you want to delete all metadata entities and nodes that meet the specified settings? <br /><strong>This process may destroy a lot of stored metadata and cannot be undone!</strong></center>'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete entities/nodes'),
  );

  return $form;
}

/**
 * Metadata deletion form submission
 */
function xc_metadata_delete_form_submit($form, &$form_state) {

  variable_set('xc_metadata_delete_stat', array('start' => microtime(TRUE)));

  // Types
  $types = array();
  if (isset($form_state['values']['types']['all'])
     && $form_state['values']['types']['all'] === 0) {
    foreach ($form_state['values']['types'] as $type => $value) {
      if ($type && $value !== 0) {
        $types[] = $type;
      }
    }
  }

  // Locations
  $location_ids = array();
  if ($form_state['values']['locations']['all'] != 'all') {
    foreach ($form_state['values']['locations'] as $location) {
      if ($location) {
        $location_ids[] = $location;
      }
    }
  }
  else {
    foreach ($form_state['values']['locations'] as $location_id => $value) {
      if ($location_id != 'all') {
        $location_ids[] = $location_id;
      }
    }
  }
  $locations = array();
  foreach ($location_ids as $location_id) {
    $locations[] = xc_location_load($location_id);
  }

  $updated = $created = array(
    'from' => FALSE,
    'to' => FALSE,
  );
  if ($form_state['values']['created']['use_date_range'] == 1) {
    // Created
    $from = $form_state['values']['created']['from'];
    $to = $form_state['values']['created']['to'];
    $created['from'] = strtotime(sprintf('%s-%s-%s',    $from['year'], $from['month'], $from['day']));
    $created['to']   = strtotime(sprintf('%s-%s-%s %s', $to['year'],   $to['month'],   $to['day'], '23:59:59'));
  }

  if ($form_state['values']['updated']['use_date_range'] == 1) {
    // Updated
    $from = $form_state['values']['updated']['from'];
    $to = $form_state['values']['updated']['to'];
    $updated['from'] = strtotime(sprintf('%s-%s-%s',    $from['year'], $from['month'], $from['day']));
    $updated['to']   = strtotime(sprintf('%s-%s-%s %s', $to['year'],   $to['month'],   $to['day'], '23:59:59'));
  }

  $advanced = $form_state['values']['advanced'];
  $source = NULL;
  variable_del('xc_solr_batch_remove');

  $batch = array(
    'operations' => array(
      array('xc_metadata_delete_batch', array($types, $locations, $created, $updated, $advanced, $source)),
    ),
    'finished' => 'xc_metadata_delete_batch_finished',
    'title' => t('Metadata deletion in progress&hellip;'),
    'init_message' => t('Beginning metadata deletion process&hellip;'),
    'progress_message' => t('Deleting nodes') . ' | ' . l(t('Stop process'), 'admin/xc/metadata/delete'),
    'error_message' => t('Metadata deletion has encountered an error.'),
  );
  batch_set($batch);
}

/**
 * Metadata deletion batch API operation
 *
 * @param $types (Array)
 *   The array of metadata types to delete
 * @param $locations (Array)
 *   The array of location objects (fields: location_id, name, description, types)
 * @param $created (Array)
 *   The timestaps of the date range of creation. The array has two keys: from
 *   and to. Both contains a timestamp.
 * @param $updated (Array)
 *   The timestaps of the date range of update. The array has two keys:
 *   from and to. Both contains a timestamp.
 * @param $advanced
 * @param $source_id (int)
 *   The source id of the XCEntity
 * @param $context (Array)
 *   The Batch API's standard context object
 *
 * @return unknown_type
 */
function xc_metadata_delete_batch($types, $locations, $created, $updated, $advanced, $source_id, &$context) {
  global $_xc_solr_batch_remove;
  $_xc_solr_batch_remove = variable_get('xc_solr_batch_remove', array());

  /*
   xc_log_info('delete', join(', ', array(var_export($types, TRUE),
   var_export($locations, TRUE), var_export($created, TRUE),
   var_export($updated, TRUE), var_export($advanced, TRUE),
   var_export($source_id, TRUE))));
   */
  // Sandbox and context
  if (!isset($context['sandbox']['progress'])) {
    // Begin the correct SQL for counting and selecting
    $count_sql  = 'SELECT COUNT(DISTINCT metadata_id) ';
    // $select_min_sql = 'SELECT min(metadata_id) ';
    $select_sql = 'SELECT metadata_id, node_id, identifier_int ';
    $_sql = ' FROM {xc_entity_properties} ';

    $conditions = array();
    if ($created['from'] !== FALSE) {
      if ($created['from'] < 0) {
        $created['from'] = 0;
      }
      $time_conditions[] = sprintf('(created BETWEEN %d AND %d)', $created['from'], $created['to']);
    }

    if ($updated['from'] !== FALSE) {
      if ($updated['from'] < 0) {
        $updated['from'] = 0;
      }
      $time_conditions[] = sprintf('(updated BETWEEN %d AND %d)', $updated['from'], $updated['to']);
    }

    if (!empty($time_conditions)) {
      if (count($time_conditions) == 2) {
        if (isset($advanced['date_operation'])
             && in_array($advanced['date_operation'], array('AND', 'OR'))) {
          $date_operation = $advanced['date_operation'];
        }
        else {
          $date_operation = 'AND';
        }
        $conditions[] = join(" $date_operation ", $time_conditions);
      }
      else {
        $conditions[] = $time_conditions[0];
      }
    }

    if ($types) {
      $conditions[] = 'AND metadata_type IN (\'' . implode("','", $types) . '\') ';
    }

    if (!empty($source_id)) {
      $conditions[] = 'AND source_id = ' . $source_id;
    }

    // TODO: how to search for locations? it is in serialized format....
    /*
     if ($locations) {
     $_sql .= " AND type IN (". implode(',', $locations) . ') ';
     }
     */
    if (!empty($conditions)) {
      $_sql .= ' WHERE ' . join(' ', $conditions);
    }
    else {
      $context['sandbox']['no_conditions'] = TRUE;
    }

    // Combine SQL
    $count_sql  .= $_sql;
    $select_sql .= $_sql;

    $context['sandbox']['progress'] = 0;
    $context['sandbox']['select_sql'] = $select_sql;
    $context['results']['count_sql'] = $count_sql;
    // TODO Please convert this statement to the D7 database API syntax.
    $context['sandbox']['max'] = db_query($count_sql)->fetchField();
    $context['sandbox']['limit'] = !empty($advanced['batch_limit']) ? $advanced['batch_limit'] : 200;
    $context['sandbox']['offset_id'] = -1;
  }

  $t1 = microtime(TRUE);
  // TODO: instead of db_query_range, use the WHERE id > currect LIMIT x type
  $sql = $context['sandbox']['select_sql']
       . ($context['sandbox']['no_conditions'] ? ' WHERE ' : ' AND ')
       . 'metadata_id > :d1';

  // TODO Please convert this statement to the D7 database API syntax.
  $result = db_query_range($sql, 0, $context['sandbox']['limit'], array(':d1' => $context['sandbox']['offset_id']));
  foreach ($result as $entity) {
    if (empty($entity)) {
      continue;
    }
    if ($entity->node_id == 0) {
      db_delete('xc_entity_properties')
        ->condition('metadata_id', $entity->metadata_id)
        ->execute();
      db_delete('xc_sql_metadata')
        ->condition('metadata_id', $entity->metadata_id)
        ->execute();
      $or = db_or()->condition('parent', $entity->identifier_int)->condition('child', $entity->identifier_int);
      db_delete('xc_entity_relationships')
        ->condition($or)
        ->execute();
      xc_solr_xc_remove($entity, $locations);
    }
    else {
      node_delete($entity->node_id);
    }

    // Store some result for post-processing in the finished callback.
    // $context['results'] = $entity->node_id;

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['offset_id'] = $entity->metadata_id;
  }

  $context['message'] = t('Now deleting %progress out of %max entities/nodes',
    array(
      '%progress' => $context['sandbox']['progress'],
      '%max' => $context['sandbox']['max'],
  ));
  xc_log_info('metadata', $context['message']);

  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['finished'] = 1;
    $context['results']['count'] = $context['sandbox']['max'];
  }

  // Drupal dumps too much messages into the session
  unset($_SESSION['messages']);
  variable_set('xc_solr_batch_remove', $_xc_solr_batch_remove);
}

/**
 * Metadata deletion batch API finished callback
 */
function xc_metadata_delete_batch_finished($success, $results, $operations) {
  global $_xc_solr_batch_remove;
  $_xc_solr_batch_remove = variable_get('xc_solr_batch_remove', array());

  $stat = variable_get('xc_metadata_delete_stat', array());

  if ($success) {
    // Run commit and optimize incase it's needed
    _xc_commit();
    _xc_optimize();
    variable_del('xc_solr_batch_remove');

    $message = t('Successfully deleted %count metadata entity/nodes in %total sec.',
      array(
      '%count' => $results['count'],
      '%total' => oaiharvester_sec_to_time(intval(microtime(TRUE) -$stat['start'])),
    )
    );
  }
  else {
    $error_operation = reset($operations);
    $message = t('An error occurred while deleting %error_operation with arguments: @arguments',
      array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE),
    ));
  }
  drupal_set_message($message);
  drupal_goto('admin/xc/metadata/delete');
}

/**
 * Delete all records belonging to a given source and created of updated
 * between 'from' and 'to' timestamps.
 *
 * @param $source_id (int)
 *   The source id
 * @param $from (timestamp)
 *   The starting point of the date range
 * @param $to (timestamp)
 *   The ending point of the date range
 * @return unknown_type
 */
function xc_metadata_delete_by_source($source_id, $from, $to) {
  variable_set('xc_metadata_delete_stat', array('start' => microtime(TRUE)));

  $created = $updated = array(
    'from' => $from,
    'to' => $to,
  );
  xc_log_info('delete', var_export($created, TRUE));
  xc_log_info('delete', var_export($updated, TRUE));

  $batch = array(
    'operations' => array(
      array('xc_metadata_delete_batch', array(
          NULL,
          NULL,
          $created,
          $updated,
          array('date_operation' => 'OR'),
          $source_id,
        )),
    ),
    'finished' => 'xc_metadata_delete_batch_finished',
    'title' => t('Metadata deletion in progress&hellip;'),
    'init_message' => t('Beginning metadata deletion process&hellip;'),
    'progress_message' => t('Deleting nodes') . ' | ' . l(t('Stop process'), 'admin/xc/metadata/delete'),
    'error_message' => t('Metadata deletion has encountered an error.'),
  );
  batch_set($batch);
  batch_process('admin/xc/metadata/delete');
}
