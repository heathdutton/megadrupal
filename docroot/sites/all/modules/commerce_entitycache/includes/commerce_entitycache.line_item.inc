<?php

/**
 * @file
 * The controller for the commerce_line_item entity providing the entity cache
 * load and resetCache methods.
 */

/**
 * This controller class for commerce_line_item overrides the line item entity
 * cache load and resetCache methods.
 *
 * Mainly relies on the CommerceLineItemEntityController class provided by the
 * Commerce module, just overrides entity cache relevant methods.
 */
class CommerceEntityCacheCommerceLineItemEntityController extends CommerceLineItemEntityController {

  public function resetCache(array $ids = NULL) {
    EntityCacheControllerHelper::resetEntityCache($this, $ids);
    parent::resetCache($ids);
  }

  public function load($ids = array(), $conditions = array()) {
    return EntityCacheControllerHelper::entityCacheLoad($this, $ids, $conditions);
  }

}
