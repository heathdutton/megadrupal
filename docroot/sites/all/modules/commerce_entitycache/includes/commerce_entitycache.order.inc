<?php

/**
 * @file
 * The controller for the commerce_order entity providing the entity cache load
 * and resetCache methods.
 */

/**
 * This controller class for commerce_order overrides the order entity cache
 * load and resetCache methods.
 *
 * Mainly relies on the CommerceOrderEntityController class provided by the
 * Commerce module, just overrides entity cache relevant methods.
 */
class CommerceEntityCacheCommerceOrderEntityController extends CommerceOrderEntityController {

  public function resetCache(array $ids = NULL) {
    EntityCacheControllerHelper::resetEntityCache($this, $ids);
    parent::resetCache($ids);
  }

  public function load($ids = array(), $conditions = array()) {
    return EntityCacheControllerHelper::entityCacheLoad($this, $ids, $conditions);
  }

}
