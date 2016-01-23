<?php

/**
 * @file
 * Implements a EntityReference Selection Handler for MongoEntity
 *
 * The default handler supplied with EntityReference doesn't return an instance
 * if the entity type does not have a base table set. This simply overrides
 * getInstance and returns a new instance every time.
 */

include_once drupal_get_path('module', 'entityreference') . '/plugins/selection/EntityReference_SelectionHandler_Generic.class.php';

/**
 * A EntityReference SelectionHandler for MongoEntity.
 */
class MongoEntitySelectionHandler extends EntityReference_SelectionHandler_Generic {

  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    // Empty base tables are OK by me.
    return new MongoEntitySelectionHandler($field, $instance, $entity_type, $entity);
  }

  /**
   * {@inheritdoc}
   */
  protected function __construct($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $this->field = $field;
    $this->instance = $instance;
    $this->entity_type = $entity_type;
    $this->entity = $entity;
  }

}
