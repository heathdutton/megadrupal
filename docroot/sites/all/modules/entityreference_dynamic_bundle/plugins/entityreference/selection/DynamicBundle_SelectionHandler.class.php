<?php

/**
 * A Entity handler for DynamicBundle filter.
 */
class DynamicBundle_SelectionHandler extends EntityReference_SelectionHandler_Generic {
  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new DynamicBundle_SelectionHandler($field, $instance, $entity_type, $entity);
  }

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    $form['view']['dynamicbundle'] = array(
      '#markup' => '<p>' . t('Dynamic mode selection makes the bundle of listed entities match the bundle of field_instance.') . '</p>' . '<p><em>' . t('NOTE: Dynamic Bundle filter will be applied only for entities that match \'entity type\' with \'target type\' configured in the field. Otherwise, all entities are listed') . '</em></p>',
    );

    return $form;
  }

  /**
   * Build an EntityFieldQuery to get referenceable entities.
   */
  public function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->field['settings']['target_type']);

    // Filter bundle only if the 'target_type' is the same of 'entity_type'.
    if ($this->field['settings']['target_type'] == $this->entity_type) {
      if ($this->field['settings']['handler'] == 'DynamicBundle_SelectionHandler') {
        $target_bundles['bundle_by_instance'] = $this->instance['bundle'];

        // Filter by bundle.
        $query->entityCondition('bundle', array($this->instance['bundle']), 'IN');
      }
    }

    return $query;
  }
}
