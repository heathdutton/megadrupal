<?php

/**
 *  Implements hook_drush_command().
 */
function drutils_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['drutils-delete-flag'] = array(
    'callback' => 'drush_drutils_delete_flag',
    'description' => dt('Delete flag configuration.'),
    'arguments' => array(
      'flag_name' => dt('Machine name of the flag'),
    ),
    'required-arguments' => TRUE,
  );

  $items['drutils-delete-entities'] = array(
    'callback' => 'drush_drutils_delete_entities',
    'description' => dt('Delete all entities of a given entity type and bundle.'),
    'arguments' => array(
      'entity_type' => dt('Machine name of the entity type'),
      'bundle' => dt('Machine name of the bundle'),
      'limit' => dt('Maximum number of items to delete'),
    ),
    'required-arguments' => 2,
    'examples' => array(
      'drush drutils-delete-entities taxonomy_term tags' => dt('Delete all terms of the vocabulary "tags"'),
      'drush drutils-delete-entities taxonomy_term tags 10' => dt('Delete 10 terms of the vocabulary "tags"'),
    ),
  );

  $items['drutils-update-entities'] = array(
    'callback' => 'drush_drutils_update_entities',
    'description' => dt('Invoke an update on all entities of a given entity type and bundle.'),
    'arguments' => array(
      'entity_type' => dt('Machine name of the entity type'),
      'bundle' => dt('Machine name of the bundle'),
      'limit' => dt('Maximum number of items to update'),
      'min-entity-id' => dt('ID of the first entity to process'),
    ),
    'required-arguments' => 2,
  );

  $items['drutils-remove-invalid-term-references'] = array(
    'callback' => 'drush_drutils_remove_invalid_term_references',
    'description' => dt('Delete invalid term references for a given field.'),
    'arguments' => array(
      'field' => dt('Machine name of the field, with leading field_. You may also enter "all" to clean up all term reference fields.'),
    ),
    'required-arguments' => 1,
  );

  $items['drutils-delete-node-type'] = array(
    'callback' => 'drush_drutils_delete_node_type',
    'description' => dt('Delete the configuration for the given node type.'),
    'arguments' => array(
      'node_type' => dt('Machine name of the node type'),
    ),
    'required-arguments' => 1,
  );

  $items['drutils-delete-vocabulary'] = array(
    'callback' => 'drush_drutils_delete_vocabulary',
    'description' => dt('Delete a given vocabulary configuration.'),
    'arguments' => array(
      'vocabulary' => dt('Machine name of the vocabulary'),
    ),
    'required-arguments' => 1,
  );

  $items['drutils-delete-view'] = array(
    'callback' => 'drush_drutils_delete_view',
    'description' => dt('Delete a given view.'),
    'arguments' => array(
      'view' => dt('Machine name of the view'),
    ),
    'required-arguments' => 1,
  );

  $items['drutils-delete-image-style'] = array(
    'callback' => 'drush_drutils_delete_image_style',
    'description' => dt('Delete a given image style (with optional replacement style).'),
    'arguments' => array(
      'style_name' => dt('Machine name of the image style to delete'),
      'replacement_style_name' => dt('(optional) When deleting a style, specify a replacement style name so that existing settings (if any) may be converted to a new style'),
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush drutils-delete-image-style stylename' => dt('Delete "stylename" image style'),
      'drush drutils-delete-entities stylename otherstylename' => dt('Delete "stylename" image style and replace it with "otherstylename"'),
    ),
  );

  return $items;
}

/**
 * Implementats hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function drutils_drush_help($section) {
  switch ($section) {
    case 'drush:drutils-delete-flag':
      return dt('Delete specific flag definition.');
  }
}

/**
 * Drush callback to delete a flag.
 *
 * @param $machine_name
 *   Machine name of the flag.
 */
function drush_drutils_delete_flag($machine_name) {
  $flag = flag_load($machine_name);
  if ($flag) {
    $flag->delete();
    drush_log(dt('Flag @name deleted.', array('@name' => $machine_name)), 'success');
  }
  else {
    drush_log(dt('Flag @name does not exist.', array('@name' => $machine_name)), 'notice');
  }
}

/**
 * Drush command to delete entities of a given entity type and bundle.
 *
 * @param $entity_type
 *   machine name of the entity type
 * @param string $bundle
 *   machine name of the bundle
 * @param integer $limit
 *   Maximum number of items to delete, 0 == unlimited
 */
function drush_drutils_delete_entities($entity_type, $bundle, $limit = 0) {

  // Base query to count overall items.
  $query = _drush_drutils_get_entities_base_query($entity_type, $bundle);

  // Limit the result set, if we have to.
  if ($limit > 0) {
    $query->range(0, $limit);
  }
  // Get max limit.
  $process_limit = $query->count()->execute();

  // Only process if we found items.
  if ($process_limit > 0) {
    drush_log(dt('We are about to delete @count of @max entities of type "@type" and bundle "@bundle".', array(
      '@count' => $process_limit,
      '@limit' => ($limit) ? $limit : $process_limit,
      '@type' => $entity_type,
      '@bundle' => $bundle,
    )));

    // Set up a batch to process the given $limit of items.
    $batch = array(
      'title' => t('Delete entities'),
      'operations' => array(
        array('_drush_drutils_entities_op', array('_drush_drutils_delete_entities_callback', $entity_type, $bundle, $process_limit, 0)),
      ),
      // Set it to be proccessed by drush.
      'progressive' => FALSE,
    );
    batch_set($batch);
    drush_backend_batch_process();
  }
  else {
    return dt('No entities to delete.');
  }
}

/**
 * Drush command to update entities of a given entity type and bundle.
 *
 * @param $entity_type
 *   machine name of the entity type
 * @param string $bundle
 *   machine name of the bundle
 * @param integer $limit
 *   Maximum number of items to delete, 0 == unlimited
 * @param integer $min_entity_id
 *   ID of the first entity to process.
 */
function drush_drutils_update_entities($entity_type, $bundle, $limit = 0, $min_entity_id = 0) {

  // Base query to count overall items.
  $query = _drush_drutils_get_entities_base_query($entity_type, $bundle, $min_entity_id - 1);

  // Limit the result set, if we have to.
  if ($limit > 0) {
    $query->range(0, $limit);
  }
  // Get max limit.
  $process_limit = $query->count()->execute();

  // Only process if we found items.
  if ($process_limit > 0) {
    drush_log(dt('We are about to update @count of @max entities of type "@type" and bundle "@bundle".', array(
      '@count' => $process_limit,
      '@limit' => ($limit) ? $limit : $process_limit,
      '@type' => $entity_type,
      '@bundle' => $bundle,
    )));

    // Set up a batch to process the given $limit of items.
    $batch = array(
      'title' => t('Update entities'),
      'operations' => array(
        array('_drush_drutils_entities_op', array('_drush_drutils_update_entities_callback', $entity_type, $bundle, $process_limit, $min_entity_id)),
      ),
      // Set it to be proccessed by drush.
      'progressive' => FALSE,
    );
    batch_set($batch);
    drush_backend_batch_process();
  }
  else {
    return dt('No entities to update.');
  }
}


/**
 * Base query for getting entities to delete.
 *
 * @param $enity_type
 * @param $bundle
 *
 * @return EntityFieldQuery
 */
function _drush_drutils_get_entities_base_query($entity_type, $bundle, $last_entity_id = 0) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type)
    ->entityCondition('bundle', $bundle)
    ->entityOrderBy('entity_id')
    ->entityCondition('entity_id', $last_entity_id, '>');

  return $query;
}

/**
 * Helper operation callback to process a given callback on a set of entities.
 *
 * @param string $callback
 * @param string $entity_type
 * @param string $bundle
 * @param integer $limit
 * @param integer $min_entity_id
 * @param array $context
 */
function _drush_drutils_entities_op($callback, $entity_type, $bundle, $limit, $min_entity_id, &$context) {
  // Init batch operation.
  if (!isset($context['sandbox']['count'])) {
    $context['sandbox']['count'] = 0;
  }

  $last_entity_id = 0;
  if (isset($context['sandbox']['last_entity_id'])) {
    $last_entity_id = $context['sandbox']['last_entity_id'];
  }
  elseif ($min_entity_id > 0) {
    $last_entity_id = $min_entity_id - 1;
  }

  // Get the next entity to delete.
  $query = _drush_drutils_get_entities_base_query($entity_type, $bundle, $last_entity_id);
  $query->range(0, 1);

  $entities = $query->execute();

  // If we got a result, we process.
  if (!empty($entities[$entity_type])) {

    // Call the callbac function
    call_user_func($callback, $entities, $entity_type);

    $context['sandbox']['count']++;

    // Get last entity ID.
    $context['sandbox']['last_entity_id'] = max(array_keys($entities[$entity_type]));

    $context['message'] = dt('Processed @count of @limit (@percentage %)', array(
      '@count' => $context['sandbox']['count'],
      '@limit' => $limit,
      '@percentage' => round(100 * $context['sandbox']['count'] / $limit, 2),
    ));

    $context['sandbox']['progress'] = $context['sandbox']['count'] / $limit;
    $context['finished'] = $context['sandbox']['progress'];
  }
  else {
    $context['finished'] = TRUE;
  }
}

/**
 * Delete callback for the batch process.
 *
 * @param array $entities
 *   Return value fo EntityFieldQuery
 * @param string $entity_type
 *   Machine name of the entity type.
 */
function _drush_drutils_delete_entities_callback($entities, $entity_type) {
  // Try to get entity id and delete that entity.
  try {
    $entity_id = key($entities[$entity_type]);
    if ($entity_id) {
      $ret = entity_delete($entity_type, $entity_id);
      if ($ret !== FALSE) {
        drush_log(dt('Deleted entity @entity_type - @id', array('@entity_type' => $entity_type, '@id' => $entity_id)), 'ok');
      }
      else {
        drush_log(dt('Seems entity @entity_type - @id could not be deleted.', array('@entity_type' => $entity_type, '@id' => $entity_id)), 'warning');
      }
    }
  }
  catch (Exception $e) {
    drush_log(dt('Deletion of entity @entity_type - @id failed with message: @message', array('@entity_type' => $entity_type, '@id' => $entity_id, '@message' => $e->getMessage())), 'error');
  }
}


/**
 * Update callback for the batch process.
 *
 * @param array $entities
 *   Return value fo EntityFieldQuery
 * @param string $entity_type
 *   Machine name of the entity type.
 */
function _drush_drutils_update_entities_callback($entities, $entity_type) {
  // Try to get entity id and delete that entity.
  try {
    $entity_id = key($entities[$entity_type]);
    if ($entity_id) {
      $entity = entity_load_single($entity_type, $entity_id);
      $ret = entity_save($entity_type, $entity);
      if ($ret !== FALSE) {
        drush_log(dt('Updated entity @entity_type - @id', array('@entity_type' => $entity_type, '@id' => $entity_id)), 'ok');
      }
      else {
        drush_log(dt('Seems entity @entity_type - @id could not be deleted.', array('@entity_type' => $entity_type, '@id' => $entity_id)), 'warning');
      }
    }
  }
  catch (Exception $e) {
    drush_log(dt('Deletion of entity @entity_type - @id failed with message: @message', array('@entity_type' => $entity_type, '@id' => $entity_id, '@message' => $e->getMessage())), 'error');
  }
}

/**
 * Drush callback to delete invalid term references due to missing terms.
 *
 * @param string $field
 *   Machine name of the field.
 */
function drush_drutils_remove_invalid_term_references($field) {

  // On argument "all", we process all term reference fields in a loop.
  if ($field == 'all') {
    $info = field_info_fields();

    foreach ($info as $f) {
      if ($f['type'] == 'taxonomy_term_reference') {
        drush_drutils_removed_invalid_term_references($f['field_name']);
      }
    }
    return;
  }

  // Process a single field.
  $table = 'field_data_' . $field;
  $column = $field . '_tid';

  // Do not proceed with invalid fields.
  if (!db_table_exists($table)) {
    drush_log(dt('There is no table @table for field @field', array('@field' => $field, '@table' => $table)), 'error');
    return;
  }

  // Get term ids that are not valid anymore.
  $query = db_select($table, 'f');
  $query->fields('f', array($column));
  $query->leftJoin('taxonomy_term_data', 't', 'f.' . $column . ' = t.tid');
  $query->isNull('t.tid');
  $query->distinct();

  $tids = $query->execute()->fetchCol();

  if (!empty($tids)) {
    $del = db_delete($table)
      ->condition($column, $tids);
    $count = $del->execute();

    drush_log(dt('Deleted @count field values for "@field" because of missing terms: @tids', array('@count' => $count, '@field' => $field, '@tids' => implode(', ', $tids))), 'ok');
  }
  else {
    drush_log(dt('No field values to delete for @field.', array('@field' => $field)), 'status');
  }
}

/**
 * Simple drush callback to delete a node type.
 *
 * @param string $node_type
 *   The machine name of the node type to delete.
 */
function drush_drutils_delete_node_type($node_type) {
  node_type_delete($node_type);
}

/**
 * Drush callback to delete a vocabulary by machine name.
 *
 * @param string $voc_machine_name
 *   Machine name of the vocabulary to delete.
 */
function drush_drutils_delete_vocabulary($voc_machine_name) {

  $voc = taxonomy_vocabulary_machine_name_load($voc_machine_name);

  if ($voc) {
    $success = taxonomy_vocabulary_delete($voc->vid);

    if ($success) {
      drush_log(dt('Vocabulary @name successfully deleted.', array('@name' => $voc_machine_name)), 'ok');
    }
    else {
      drush_log(dt('Could not properly delete vocabulary @name.', array('@name' => $voc_machine_name)), 'error');
    }
  }
  else {
    drush_log(dt('No vocabulary @name given.', array('@name' => $voc_machine_name)), 'status');
  }
}

/**
 * Drush callback to delete a view by machine name.
 *
 * @param string $view_machine_name
 *   Machine name of the view to delete.
 */
function drush_drutils_delete_view($view_machine_name) {

  if ($view = views_get_view($view_machine_name)) {
    views_delete_view($view);
    $view->delete();
    // Empty cache now.
    if (function_exists('views_invalidate_cache')) {
      views_invalidate_cache();
    }
    drush_log(dt('View @name successfully deleted.', array('@name' => $view_machine_name)), 'ok');
  }
}

/**
 * Drush callback to delete an image style by machine name.
 *
 * @param string $style_name
 *   Machine name of the image style to delete.
 * @param string $replacement_style_name
 *  (optional) When deleting a style, specify a replacement style name so that
 *  existing settings (if any) may be converted to a new style.
 */
function drush_drutils_delete_image_style($style_name, $replacement_style_name = '') {
  if (!($style = image_style_load($style_name))) {
    drush_log(dt('Image style @name does not exist.', array('@name' => $style_name)));
  }
  else {
    // Check replacement style (if any)
    if (!empty($replacement_style_name) && !image_style_load($replacement_style_name)) {
      drush_log(dt('Replacement image style @name not found.', array('@name' => $replacement_style_name)), 'error');
    }
    else {
      // Delete image style
      image_style_delete($style, $replacement_style_name);
      drush_log(dt('Deleted @name image style.', array('@name' => $style_name)), 'ok');
    }
  }
}
