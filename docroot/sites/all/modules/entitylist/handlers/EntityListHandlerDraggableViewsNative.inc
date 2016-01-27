<?php

class EntityListHandlerDraggableViewsNative extends EntityListHandlerDraggableViews {

  function requiredArguments() {
    return array();
  }

  function execute() {
    list($name, $display) = explode(':', $this->config['view']);
    $results = db_select('draggableviews_structure', 'ds')
      ->fields('ds', array('entity_id'))
      ->condition('ds.view_name', $name)
      ->condition('ds.view_display', $display)
      ->orderBy('weight', 'ASC')
      ->execute();

    $entity_ids = array();
    foreach ($results as $row) {
      $entity_ids[] = $row->entity_id;
    }
    return entity_load($this->entityType(), $entity_ids);
  }
}
