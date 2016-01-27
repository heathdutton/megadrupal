<?php

/**
 * @file
 * drupal_sync.entity.inc
 * Processings entities
 */

/**
 * Implements hook_entity_insert().
 */
function drupal_sync_entity_insert($entity, $type) {
  //Using hook_node_insert against hook_entity_insert for nodes because hook_menu_link_insert invokes earlier
  //than hook_entity_insert
  //requires for drupal_sync_menu module
  if ($type != 'node') {
    drupal_sync_entity_prepare_to_queue($entity, $type, 'insert');
  }
}

/**
 * Implements hook_node_insert().
 */
function drupal_sync_node_insert($node) {
  drupal_sync_entity_prepare_to_queue($node, 'node', 'insert');
}

/**
 * Implements hook_entity_update().
 */
function drupal_sync_entity_update($entity, $type) {
  //Using hook_node_update against hook_entity_update for nodes because hook_menu_link_update invokes earlier
  //than hook_entity_insert
  //requires for drupal_sync_menu module
   if ($type != 'node') {
    drupal_sync_entity_prepare_to_queue($entity, $type, 'update');
   }
}

/**
 * Implements hook_node_update().
 */
function drupal_sync_node_update($node) {
  drupal_sync_entity_prepare_to_queue($node, 'node', 'update');
}

/**
 * Implements hook_entity_delete().
 */
function drupal_sync_entity_delete($entity, $type) {
    drupal_sync_entity_prepare_to_queue($entity, $type, 'delete');
}


/**
 * Prepare entity for adding to queue
 *
 * @param array $entity
 * @param string $type
 * @param string $op
 */
function drupal_sync_entity_prepare_to_queue($entity, $type, $op = 'insert') {
  if (!drupal_sync_check_site_domain()) {
    drupal_sync_drupal_sync_error_notification('settings_error', array(t('Operation') => 'Update to remote site'));
    return FALSE;
  }

  $entity = is_array($entity) ? $entity : (array) $entity;

  // Insert the new entity into a fictional table of all entities.
  $drupal_sync_settings = variable_get('drupal_sync_settings', array());

  $entities_settings = (isset($drupal_sync_settings['drupal_sync_entities']) && !empty($drupal_sync_settings['drupal_sync_entities'])) ? $drupal_sync_settings['drupal_sync_entities'] : array();
  $remotes = (isset($drupal_sync_settings['drupal_sync_remote']) && !empty($drupal_sync_settings['drupal_sync_remote'])) ? $drupal_sync_settings['drupal_sync_remote'] : array();


  if (!empty($remotes) && array_key_exists($type, $entities_settings)) {
    if (!empty($entity['drupal_sync_is_remote'])) {
      //if this entity is coming from remote site
      $source_changed = $entity['drupal_sync_source_changed'];
    }
    else {
      //if it's local changes
      $source_changed = time();
    }
    foreach ($remotes as $remote) {

      $entity['remote_site_name'] = $remote['name'];

      if ($op == 'insert' || $op == 'update') {
        if (!empty($entity['drupal_sync_remote_site_name']) && ($entity['drupal_sync_remote_site_name'] == $remote['name'])) {
          //dont push insert entity to the remote site from which we get this update
          continue;
        }
      }

      $sync_exist = drupal_sync_local_store_check_existance_remote_op($entity, $remote['name'], $type);

      if (($op == 'insert' || $op == 'update') && !empty($sync_exist)) {
        //dont push the same sync entity to the source site
        continue;
      }

      // send to all remote sites
      switch ($type) {
        case 'node':
          // check if this entity type in settings
          if (isset($entities_settings[$type][$entity['type']])) {
            // send node to remote sites
            drupal_sync_add_to_queue($entity['nid'], $type, $entity, $entity['type'], $op, $source_changed);
          }
          break;
        case 'taxonomy_term':
          // check if entity type in settings
          if (isset($entities_settings[$type][$entity['vid']])) {
            // send term to remote sites
            drupal_sync_add_to_queue($entity['tid'], $type, $entity, $entity['vid'], $op, $source_changed);
          }
          break;
      }
     module_invoke_all('drupal_sync_entity_prepare_to_queue', $entities_settings, $type, $entity, $op, $source_changed);
    }
  }

  $eid = _drupal_sync_get_entity_id($type, $entity);
  if ($op == 'delete') {
    //delete all rows about this entity in local sync store if exists
    drupal_sync_local_store_delete_all_rows($eid, $type);
  }
}

/**
 * Get entity bundle properties
 *
 * @param string $type
 * @param string $entity_type_id
 *
 * @return array
 */
function drupal_sync_get_entity_bundle_properties($type, $entity_type_id) {
  $properties = array();
  $properties = module_invoke_all('drupal_sync_entity_bundle_properties', $type, $entity_type_id);
  return $properties;
}

/**
 * Implements hook_drupal_sync_entity_bundle_properties
 *
 * @param string $type
 * @param string $entity_type_id
 */
function drupal_sync_drupal_sync_entity_bundle_properties($type, $entity_type_id) {
  $entity_property_info = entity_get_property_info($type);
  $properties = array();

  switch ($type) {
    case 'node':
      $properties = (isset($entity_property_info['bundles'][$entity_type_id]['properties'])) ? $entity_property_info['bundles'][$entity_type_id]['properties'] : array();
      break;
    case 'taxonomy_term':
      $vocabulary = taxonomy_vocabulary_load($entity_type_id);
      if (isset($vocabulary->machine_name)) {
        $properties = (isset($entity_property_info['bundles'][$vocabulary->machine_name]['properties'])) ? $entity_property_info['bundles'][$vocabulary->machine_name]['properties'] : array();
      }
      break;
  }
  return $properties;
}