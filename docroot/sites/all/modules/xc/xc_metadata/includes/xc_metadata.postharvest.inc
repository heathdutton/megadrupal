<?php
/**
 * @file
 * XC Metadata postharvest functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Confirmation form to launch node creation
 */
function xc_metadata_postharvest_batch_launch_form($form, &$form_state) {
  return confirm_form(array(),
    t('Are you sure you want to start node creation batch process?'),
    'admin/xc/metadata', '', t('Start'), t('Cancel'));
}

/**
 * Confirmation form to launch relationship creation
 */
function xc_metadata_relationship_batch_launch_form($form, &$form_state) {
  return confirm_form(array(),
    t('Are you sure you want to start entity relationships creating batch process?'),
    'admin/xc/metadata', '', t('Start'), t('Cancel'));
}

/**
 * Handler function for node creation launching form.
 */
function xc_metadata_postharvest_batch_launch_form_submit() {
  xc_metadata_postharvest_batch_launch();
}

/**
 * Handler function for relationship creation launching form.
 */
function xc_metadata_relationship_batch_launch_form_submit() {
  xc_metadata_relationship_batch_launch();
}

/**
 * Doing the post harvester tasks
 * - creating nodes
 * - seting up relationships
 * - refreshing Solr
 */
function xc_metadata_postharvest_batch_launch() {
  variable_set('xc_metadata_node_creation_finished', 0);

  // drop indexes
  /*
   $count = db_result(db_query('SELECT COUNT(metadata_id) FROM {xc_entity_properties} WHERE node_id = 0'));
   if ($count > 0) {
   $ret = array();
   $schema = drupal_get_schema('xc_entity_properties');
   $indexes = $schema['indexes'];
   foreach ($indexes as $name => $fields) {
   db_drop_index($ret, 'xc_entity_properties', $name);
   }
   }
   */

  $batch_set = array(
    'operations' => array(array('xc_metadata_node_creation_batch_operation', array(NULL, NULL, NULL))),
    'title' => t('Node creation batch process'),
    'init_message' => t('The node creation batch is starting'),
    'progress_message' => t('Completed @current of @total'),
    'finished' => 'xc_metadata_node_creation_batch_finished',
    // 'file'             => drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.postharvest.inc',
  );
  batch_set($batch_set);
  batch_process('admin/xc/metadata');
}

/**
 * Doing the relationship creation tasks
 */
function xc_metadata_relationship_batch_launch() {
  variable_set('xc_metadata_relationship_creation_finished', 0);

  $batch_set = array(
    'operations' => array(array('xc_metadata_relationship_creation_batch_operation', array(NULL, NULL, NULL))),
    'title' => t('Relationship creation batch process'),
    'init_message' => t('The relationship creation batch is starting'),
    'progress_message' => t('Completed @current of @total'),
    'finished' => 'xc_metadata_relationship_creation_batch_finished',
    'file' => drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.postharvest.inc',
  );
  batch_set($batch_set);
  batch_process('admin/xc/metadata');
}

/**
 * The batch finishing function
 * @param $success
 * @param $results
 * @param $operations
 */
function xc_metadata_node_creation_batch_finished($success, $results, $operations) {
  $_xc_meta_time = variable_get('xc_metadata_time', array());
  drupal_set_message(filter_xss('success: ' . var_export($success, TRUE)));
  drupal_set_message(filter_xss('results: ' . var_export($results, TRUE)));
  drupal_set_message(filter_xss('operations: ' . var_export($operations, TRUE)));
  drupal_set_message(filter_xss('meta time: ' . var_export($_xc_meta_time, TRUE)));

  // set that node creation is finished
  variable_set('xc_metadata_node_creation_finished', 1);
  // re-create index
  /*
   $ret = array();
   $schema = drupal_get_schema('xc_entity_properties');
   $indexes = $schema['indexes'];
   foreach ($indexes as $name => $fields) {
   db_add_index($ret, 'xc_entity_properties', $name, $fields);
   }
   */

  $now = microtime(TRUE);
  drupal_set_message(t('Node creation is finished! Created %total nodes in %duration',
    array(
    '%total' => $results['total'],
    '%duration' => oaiharvester_sec_to_time($now - $results['start']
  ),
  )));
}

/**
 * The relationship creation finishing function
 * @param $success
 * @param $results
 * @param $operations
 */
function xc_metadata_relationship_creation_batch_finished($success, $results, $operations) {
  $_xc_meta_time = variable_get('xc_metadata_time', array());
  drupal_set_message(filter_xss('success: ' . var_export($success, TRUE)));
  drupal_set_message(filter_xss('results: ' . var_export($results, TRUE)));
  drupal_set_message(filter_xss('operations: ' . var_export($operations, TRUE)));
  drupal_set_message(filter_xss('meta time: ' . var_export($_xc_meta_time, TRUE)));

  // set that node creation is finished
  variable_set('xc_metadata_relationship_creation_finished', 1);
  // re-create index

  $table = 'xc_entity_relationships';
  $path = $_SERVER['DOCUMENT_ROOT'] . $GLOBALS['base_path'] . $results['csv_file_name'];

  $sql = "LOAD DATA LOCAL INFILE '" . $path . "' INTO TABLE {" . $table . '}'
      . " CHARACTER SET utf8 FIELDS TERMINATED BY '\\0\\t' ESCAPED BY '' LINES TERMINATED BY '\\0\\n'";
  // TODO Please convert this statement to the D7 database API syntax.
  $result = db_query($sql);
  if (!result) {
    drupal_set_message(t('Unsuccessfull load data with %sql', array('%sql' => $sql)));
  }

  $now = microtime(TRUE);
  drupal_set_message(t('Node creation is finished! Created %total nodes in %duration',
    array(
    '%total' => $results['total'],
    '%duration' => oaiharvester_sec_to_time($now - $results['start']
  ),
  )));
}

/**
 * Create nodes
 *
 * @param $metadata (Object)
 *   A 'light' metadata object with the fields: metadata_type, identifier, metadata_id
 * @param $return_node_id (Boolean)
 *   Whether to return the created node's ID
 */
function xc_metadata_create_node($metadata, $return_node_id = TRUE) {
  global $_xc_meta_time, $_xc_metadata_statistics;

  $t1 = microtime(TRUE);
  $node = new stdClass();
  $skip_node_type = TRUE;
  if (empty($metadata->node_type)) {
    $node->type  = _metadata_type_to_node_type($metadata->metadata_type);
    $skip_node_type = FALSE;
  }
  else {
    $node->type = $metadata->node_type;
  }

  $node->title = empty($metadata->identifier) ? t('Untitled') : $metadata->identifier;

  // node_save calls
  // 1) node_invoke($node, 'insert') -> xc_metadata_insert($node)
  // 2) node_invoke_nodeapi($node, 'insert') -> xc_metadata_nodeapi($node, 'insert'), mymodule_nodeapi($node, 'insert'), ...

  node_save($node);
  $t2 = microtime(TRUE);

  if (isset($node->nid)) {
    if (!isset($metadata->metadata_type)) {
      $metadata->metadata_type = _node_type_to_metadata_type($node->type);
    }
    if (!isset($_xc_metadata_statistics->nodes['total'])) {
      $_xc_metadata_statistics->nodes['total'] = 0;
    }
    $_xc_metadata_statistics->nodes['total']++;
    if (!isset($_xc_metadata_statistics->nodes[$metadata->metadata_type])) {
      $_xc_metadata_statistics->nodes[$metadata->metadata_type] = 0;
    }
    $_xc_metadata_statistics->nodes[$metadata->metadata_type]++;
  }

  // save node_id into xc_entity_properties table
  if ($skip_node_type) {
    $sql = 'UPDATE {xc_entity_properties} SET updated = %d, node_id = %d WHERE metadata_id = %d';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql, time(), $node->nid, $metadata->metadata_id) */
    $result = db_update('xc_entity_properties')
                ->fields(array(
                    'updated' => time(),
                    'node_id' => $node->nid,
                  ))
                ->condition('metadata_id', $metadata->metadata_id)
                ->execute();
  }
  else {
    $sql = 'UPDATE {xc_entity_properties} SET updated = %d, node_type = \'%s\', node_id = %d WHERE metadata_id = %d';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql, time(), $node->type, $node->nid, $metadata->metadata_id) */
    $result = db_update('xc_entity_properties')
                ->fields(array(
                    'updated' => time(),
                    'node_type' => $node->type,
                    'node_id' => $node->nid,
                  ))
                ->condition('metadata_id', $metadata->metadata_id)
                ->execute();
  }

  $t3 = microtime(TRUE);
  if (!isset($_xc_meta_time['node'])) {
    $_xc_meta_time['node'] = 0;
  }
  $_xc_meta_time['node'] += ($t2 - $t1);

  if (!isset($_xc_meta_time['update'])) {
    $_xc_meta_time['update'] = 0;
  }
  $_xc_meta_time['update'] += ($t3 - $t2);

  if ($return_node_id) {
    return $node->nid;
  }
}

/**
 * Create node based on a metadata_id
 *
 * @param $metadata_id (int)
 *   The metadata ID
 */
function xc_metadata_create_node_for_metadata($metadata_id = 0) {
  if ($metadata_id != 0) {
    $select_sql = 'SELECT metadata_id, node_id, metadata_type, identifier FROM {xc_entity_properties} WHERE metadata_id = %d';
    $metadata = db_query('SELECT metadata_id, node_id, metadata_type, identifier FROM {xc_entity_properties} WHERE metadata_id = :metadata_id', array(':metadata_id' => $metadata_id))->fetchObject();
    if (empty($metadata->node_id) || $metadata->node_id == 0) {
      $node_id = xc_metadata_create_node($metadata, TRUE);
    }
    else {
      $node_id = $metadata->node_id;
    }
  }
  if (!empty($node_id)) {
    return $node_id;
  }
  return FALSE;
}
