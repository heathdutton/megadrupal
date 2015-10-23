<?php

/**
 * @file
 * The controller for the commerce_customer_profile entity providing the entity
 * cache load and resetCache methods.
 */

/**
 * This controller class for commerce_customer_profile overrides the product
 * entity cache load and resetCache methods.
 *
 * Mainly relies on the CommerceCustomerProfileEntityController class provided
 * by the Commerce module, just overrides entity cache relevant methods.
 */
class CommerceEntityCacheCommerceCustomerProfileEntityController extends CommerceCustomerProfileEntityController {
  public function resetCache(array $ids = NULL) {
    EntityCacheControllerHelper::resetEntityCache($this, $ids);
    parent::resetCache($ids);
  }

  public function load($ids = array(), $conditions = array()) {
    return EntityCacheControllerHelper::entityCacheLoad($this, $ids, $conditions);
  }
}
