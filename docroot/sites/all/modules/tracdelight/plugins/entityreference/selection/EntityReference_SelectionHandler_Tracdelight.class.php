<?php

class EntityReference_SelectionHandler_Tracdelight extends EntityReference_SelectionHandler_Generic {

  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {

    return new EntityReference_SelectionHandler_Tracdelight($field, $instance, $entity_type, $entity);
  }

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    return array();
  }

  /**
   * Build an EntityFieldQuery to get referencable entities.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = parent::buildEntityFieldQuery($match, $match_operator);

    if (isset($match)) {

      if ($ein = tracdelight_get_ein_from_uri(request_path())) {
        $query->propertyCondition('ein', $ein, $match_operator);
      } else if (tracedelight_string_seems_to_be_ein($match)) {
        $query->propertyCondition('ein', $match, $match_operator);
      } else if (strpos($match, 'http:') !== 0){
        $query->propertyCondition('title', $match, $match_operator);
      }
    }

    return $query;
  }



  /**
   * Implements EntityReferenceHandler::getReferencableEntities().
   */
  public function getReferencableEntities($match = NULL, $match_operator = 'CONTAINS', $limit = 0) {

    $entities = parent::getReferencableEntities($match, $match_operator, $limit);

    if (empty($entities)) {
      $entities['tracdelight_product'] = array();

      $products = array();
      if ($ein = tracdelight_get_ein_from_uri(request_path())) {
        $query = array('EIN' => $ein);
        $products = tracdelight_import_products($query);
      }
      elseif (isset($match)) {
        $products = tracdelight_import_products(array('Query' => $match), 10);
      }

      foreach ($products as $product) {
        $entity_id = tracdelight_get_entity_id($product['ein']);
        if ($entity_id) {
          $entities['tracdelight_product'][$entity_id] = $product['title'];
        }
      }

    }

    return $entities;
  }
}
