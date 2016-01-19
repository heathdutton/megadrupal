<?php

/**
 * @file
 * Handler code.
 */

/**
 * Handler for field title.
 */
class SmartlingHandlerFieldSmartlingTitle extends views_handler_field {

  /**
   * Render.
   *
   * @param object $values
   *   Smartling entity.
   *
   * @return string
   *   Return title.
   */
  public function render($values) {
    $eid = $values->smartling_entity_data_rid;
    $entity_type = $values->smartling_entity_data_entity_type;
    $title = $values->smartling_entity_data_title;

    if (empty($entity_type) || empty($eid)) {
      return $title;
    }

    $result = $title;
    $entity = entity_load($entity_type, array($eid));
    if (!empty($entity)) {
      $entity = $entity[$eid];

      $uri = entity_uri($entity_type, $entity);
      $path = $uri['path'];
      $result = l($title, $path);
    }

    return $result;
  }

}
