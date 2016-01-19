<?php

/**
 * Entity handler selection handler for sharedcontent_index.
 *
 * Adds the connection name in front of the default entity label.
 */
class SharedContentSelectionHandler extends EntityReference_SelectionHandler_Generic {

  /**
   * Implements  EntityReferenceHandler::getInstance().
   *
   * The generic implementation would always return the generic handler.
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new SharedContentSelectionHandler($field, $instance, $entity_type, $entity);
  }

  /**
   * Implements EntityReferenceHandler::getLabel().
   *
   * Add the connection name in front of the default label.
   */
  public function getLabel($entity) {
    $label = entity_label($this->field['settings']['target_type'], $entity);
    $connection_labels = sharedcontent_get_connection_labels();
    $connection = t('None');
    if (!empty($entity->connection_name)) {
      if (isset($connection_labels[$entity->connection_name])) {
        $connection = $connection_labels[$entity->connection_name];
      }
    }
    else {
      $connection = $entity->connection_name;
    }

    return t('(@connection) @label', array(
      '@label' => $label,
      '@connection' => $connection,
    ));
  }
}
