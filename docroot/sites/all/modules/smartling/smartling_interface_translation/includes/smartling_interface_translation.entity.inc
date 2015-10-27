<?php

/**
 * @file
 * Smartling interface translate entity.
 */

/**
 * Implements hook_entity_info().
 */
function smartling_interface_translation_entity_info() {
  $info['smartling_interface_entity'] = array(
    // A human readable label to identify our entity.
    'label' => t('Smartling Interface Entity'),
    'entity class' => 'SmartlingInterfaceEntity',
    'controller class' => 'SmartlingInterfaceEntityController',
    'views controller class' => 'EntityDefaultViewsController',
    // The table for this entity defined in hook_schema()
    'base table' => 'smartling_interface_entity',
    // If fieldable == FALSE, we can't attach fields.
    'fieldable' => FALSE,
    'entity keys' => array(
      // The 'id' (basic_id here) is the unique id.
      'id' => 'gid',
      'bundle' => 'textgroup',
      'label' => 'textgroup_name'
    ),
    'static cache' => FALSE,
    'view modes' => array(
      'full' => array(
        'label' => t('Default'),
        'custom settings' => FALSE,
      ),
    ),
    'language callback' => 'smartling_interface_callback_entity_info_language'
  );

  return $info;
}

function smartling_interface_callback_entity_info_language($entity, $entity_type) {
  return 'en';
}

/**
 * Fetch a basic object.
 */
function smartling_interface_entity_load($entity_id = NULL, $reset = FALSE) {
  $entity_ids = (isset($entity_id) ? array($entity_id) : array());
  $basic = smartling_interface_entity_load_multiple($entity_ids, array(), $reset);
  return $basic ? reset($basic) : FALSE;
}

/**
 * Loads multiple basic entities.
 */
function smartling_interface_entity_load_multiple($entity_ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('smartling_interface_entity', $entity_ids, $conditions, $reset);
}

/**
 * Return entity object by conditions.
 */
function smartling_interface_entity_load_by_conditions($properties = NULL) {
  $entity_type = 'smartling_interface_entity';
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
 * We create the entity by calling the controller.
 */
function smartling_interface_entity_create() {
  return entity_get_controller('smartling_interface_entity')->create();
}

/**
 * We save the entity by calling the controller.
 */
function smartling_interface_entity_save(&$entity) {
  return entity_get_controller('smartling_interface_entity')->save($entity);
}

/**
 * Use the controller to delete the entity.
 */
function smartling_interface_entity_delete($entity) {
  entity_get_controller('smartling_interface_entity')->delete($entity);
}

/**
 * Use the controller to multiple delete the entities.
 */
function smartling_interface_entity_delete_multiple($entity_ids) {
  entity_delete_multiple('smartling_interface_entity', $entity_ids);
}
