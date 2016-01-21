<?php

/**
 * @file
 * The controller for the commerce_product entity providing the entity cache
 * load and resetCache methods.
 */

/**
 * This controller class for commerce_product overrides the product entity
 * cache load and resetCache methods.
 *
 * Mainly relies on the CommerceProductEntityController class provided by the
 * Commerce module, just overrides entity cache relevant methods.
 */
class CommerceEntityCacheCommerceProductEntityController extends CommerceProductEntityController {

  public function resetCache(array $ids = NULL) {
    EntityCacheControllerHelper::resetEntityCache($this, $ids);
    parent::resetCache($ids);
  }

  public function load($ids = array(), $conditions = array()) {
    return EntityCacheControllerHelper::entityCacheLoad($this, $ids, $conditions);
  }

}
