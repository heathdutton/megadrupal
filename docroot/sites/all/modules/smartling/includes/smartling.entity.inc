<?php

/**
 * @file
 * Smartling entity.
 */

/**
 * Implements hook_entity_info().
 */
function smartling_entity_info() {
  $info['smartling_entity_data'] = array(
    // A human readable label to identify our entity.
    'label' => t('Smartling Entity Data'),
    'entity class' => 'SmartlingEntityData',
    'controller class' => 'SmartlingEntityDataController',
    'views controller class' => 'EntityDefaultViewsController',
    // The table for this entity defined in hook_schema()
    'base table' => 'smartling_entity_data',
    // If fieldable == FALSE, we can't attach fields.
    'fieldable' => FALSE,
    'entity keys' => array(
      // The 'id' (basic_id here) is the unique id.
      'id' => 'eid',
    ),
    'static cache' => FALSE,
    'view modes' => array(
      'full' => array(
        'label' => t('Default'),
        'custom settings' => FALSE,
      ),
    ),
  );

  return $info;
}

/**
 * Fetch a basic object.
 */
function smartling_entity_data_load($entity_id = NULL, $reset = FALSE) {
  $entity_ids = (isset($entity_id) ? array($entity_id) : array());
  $basic = smartling_entity_data_load_multiple($entity_ids, array(), $reset);
  return $basic ? reset($basic) : FALSE;
}

/**
 * Loads multiple basic entities.
 */
function smartling_entity_data_load_multiple($entity_ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('smartling_entity_data', $entity_ids, $conditions, $reset);
}

/**
 * Return entity object by conditions.
 */
function smartling_entity_load_by_conditions($properties = NULL) {
  $entity_type = 'smartling_entity_data';
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type);
  if ($properties) {
    foreach ($properties as $name => $value) {
      $query->propertyCondition($name, $value);
    }
  }

  $query->range(0, 1);
  $result = $query->execute();
  if ($result) {
    $id = key($result[$entity_type]);
    $entities = entity_load($entity_type, array($id));
    return reset($entities);
  }
  else {
    return FALSE;
  }
}

/**
 * Return entity objects by conditions.
 */
function smartling_entity_load_all_by_conditions($properties = NULL) {
  $entity_type = 'smartling_entity_data';
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type);
  if ($properties) {
    foreach ($properties as $name => $value) {
      $query->propertyCondition($name, $value);
    }
  }

  $result = $query->execute();
  if ($result) {
    foreach ($result[$entity_type] as $entity_data) {
      $ids[] = $entity_data->eid;
    }
    $entities = smartling_entity_data_load_multiple($ids);
    return $entities;
  }
  else {
    return FALSE;
  }
}

/**
 * Delete entity by conditions.
 */
function smartling_entity_delete_all_by_conditions($properties = NULL) {
  $entity_type = 'smartling_entity_data';
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type);
  if ($properties) {
    foreach ($properties as $name => $value) {
      $query->propertyCondition($name, $value);
    }
  }

  $result = $query->execute();
  if ($result) {
    foreach ($result[$entity_type] as $entity_data) {
      $ids[] = $entity_data->eid;
    }
    smartling_entity_data_delete_multiple($ids);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * We create the entity by calling the controller.
 */
function smartling_entity_data_create() {
  return entity_get_controller('smartling_entity_data')->create();
}

/**
 * We save the entity by calling the controller.
 */
function smartling_entity_data_save(&$entity) {
  return entity_get_controller('smartling_entity_data')->save($entity);
}

/**
 * Use the controller to delete the entity.
 */
function smartling_entity_data_delete($entity) {
  entity_get_controller('smartling_entity_data')->delete($entity);
}

/**
 * Use the controller to multiple delete the entities.
 */
function smartling_entity_data_delete_multiple($entity_ids) {
  entity_delete_multiple('smartling_entity_data', $entity_ids);
}
