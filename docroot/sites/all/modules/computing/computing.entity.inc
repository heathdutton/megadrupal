<?php

/**
 * @file
 * Code for the entity. This does not allow UI to create Computing Record entities or Computing Application entities.
 * It has to be done in program.
 */

/**
 * Implement hook_entity_info().
 */
function computing_entity_info() {
  $return['computing_record'] = array(
    'label' => t('Computing Record'),
    // The entity class and controller class extend the classes provided by the Entity API
    'entity class' => 'ComputingRecord',
    'controller class' => 'ComputingRecordController',
    'base table' => 'computing_record',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'id',
      'bundle' => 'application',
      'label' => 'label',
    ),
    // Bundles are defined by the types below. Otherwise required by Drupal Core hook_entity_info().
    'bundles' => array(),
    // Bundle keys tell the FieldAPI how to extract information from the bundle objects
    'bundle keys' => array(
      'bundle' => 'application',
    ),
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    'creation callback' => 'computing_record_create_entity',
    'access callback' => 'computing_record_entity_access', // this can't be "user_access" because params are provided by entity_access().
    'module' => 'computing',
    // The information below is used by the ModelUIController (which extends the EntityDefaultUIController)
//    'admin ui' => array(
//      'path' => 'admin/content/models',
//      'file' => 'model.admin.inc',
//      'controller class' => 'ModelUIController',
//      'menu wildcard' => '%model',
//    ),
  );

  // The entity that holds information about the entity types
  $return['computing_application'] = array(
    'label' => t('Computing Application'),
    'entity class' => 'ComputingApplication',
    'controller class' => 'ComputingApplicationController',
    'base table' => 'computing_application',
    'fieldable' => FALSE,
    // specific to EntityAPI only.
    'bundle of' => 'computing_record',
    'exportable' => TRUE,
    'entity keys' => array(
      'id' => 'id',
      'name' => 'application',
      'label' => 'label',
    ),
    'creation callback' => 'computing_application_create_entity',
    // access callback if not set, will deny all access. see entity_access().
    'access callback' => 'computing_application_entity_access',
    'module' => 'computing',
    // Enable the entity API's admin UI.
//    'admin ui' => array(
//      'path' => 'admin/structure/computing_applications',
//      'controller class' => 'ComputingApplicationUIController',
//    ),
  );

  return $return;
}


/**
 * Implements hook_entity_info_alter().
 * This seems to be the correct way to set ['bundles'] for custom entity types.
 * Although for "node.module", it's set directly in hook_entity_info().
 */
function computing_entity_info_alter(&$entity_info) {
  foreach (computing_get_applications() as $application => $info) {
    $entity_info['computing_record']['bundles'][$application] = array(
      'label' => $info->label,
    );
  }
}


function computing_record_create_entity($values = array()) {
  return entity_get_controller('computing_record')->create($values);
}


function computing_application_create_entity($values = array()) {
  return entity_get_controller('computing_application')->create($values);
}


function computing_record_entity_access($op, $entity = NULL, $account = NULL, $entity_type = NULL) {
  return user_access('administer computing records', $account);
}

function computing_application_entity_access($op, $entity = NULL, $account = NULL, $entity_type = NULL) {
  return user_access('administer computing module', $account);
}


/**
 * Implements hook_entity_property_info_alter().
 */
function computing_entity_property_info_alter(&$entity_info) {
  if (isset($entity_info['computing_record'])) {
    $entity_info['computing_record']['properties']['created']['type'] = 'date';
    $entity_info['computing_record']['properties']['changed']['type'] = 'date';
  }
}


/**
 * The class used for computing record entities
 */
class ComputingRecord extends Entity {
  public function __construct($values = array()) {
    parent::__construct($values, 'computing_record');
  }
}


/**
 * The class used for computing application entities
 */
class ComputingApplication extends Entity {
  public function __construct($values = array()) {
    parent::__construct($values, 'computing_application');
  }
}


/**
 * The Controller for computing record entities
 */
class ComputingRecordController extends EntityAPIController {

  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Important: caller of the function should handle encode Input/Output, which are saved to DB as blob.
   */
  public function create(array $values = array()) {
    $timestamp = time();
    $values += array(
      'id' => '',
      'is_new' => TRUE,
      'created' => $timestamp,
      'changed' => $timestamp,
    );

    // input/output should not be handled here. Caller functions should be aware of how to pass input/output data as blob.

//    if (isset($values['input'])) {
//      $values['input'] = drupal_json_encode($values['input']);
//    }
//    if (isset($values['output'])) {
//      $values['output'] = drupal_json_encode($values['output']);
//    }

    $computing_record = parent::create($values);
    return $computing_record;
  }

  /**
   * Important: caller functions should handle encode/decode.
   */
//  public function load($ids = array(), $conditions = array()) {
//    $entities = parent::load($ids, $conditions);
//    if ($entities) {
//      foreach ($entities as &$entity) {
//        if (isset($entity->input)) {
//          $entity->input = drupal_json_decode($entity->input);
//        }
//        if (isset($entity->output)) {
//          $entity->output = drupal_json_decode($entity->output);
//        }
//      }
//    }
//  }

}


/**
 * The Controller for computing application entities
 */
class ComputingApplicationController extends EntityAPIControllerExportable {

  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    // Add values that are specific to our Model
    $values += array(
      'id' => '',
      'is_new' => TRUE,
    );
    $computing_application = parent::create($values);
    return $computing_application;
  }

}
