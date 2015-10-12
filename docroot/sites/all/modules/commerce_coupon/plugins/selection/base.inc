<?php

$plugin = array(
  'title' => t('Coupon simple'),
  'handler' => array(
    'class' => 'EntityReference_SelectionHandler_Generic_commerce_coupon',
  ),
);

include_once drupal_get_path('module', 'entityreference') . '/plugins/selection/EntityReference_SelectionHandler_Generic.class.php';

/**
 * Extend the entity reference selection class to use the coupon code for
 * building the EFQ.
 */
class EntityReference_SelectionHandler_Generic_commerce_coupon extends EntityReference_SelectionHandler_Generic {
  public function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = parent::buildEntityFieldQuery($match, $match_operator);
    
    // If there's a match, query the coupon code.
    if (isset($match)) {
      $query->fieldCondition('commerce_coupon_code', 'value', $match, $match_operator);
    }
    return $query;
  }

  public function getLabel($entity) {
    $wrapper = entity_metadata_wrapper('commerce_coupon', $entity);
    return $wrapper->commerce_coupon_code->value();
  }
  
}