<?php
/**
 * @file
 * File MenuRoutingItem.inc contains class MenuRoutingItem.
 *
 * Helper class to take care of inconsistencies in associative arrays coming
 * from menu router functions.
 */

/**
 * Class MenuRoutingItem.
 *
 * Helper class to take care of inconsistencies how information are returned
 * in the associative array returned but either menu_get_router() or
 * menu_get_item() functions. These arrays should have in theory the same,
 * but in reality they have different keys, so we try to handle that here.
 *
 * @private @property array $routerItem Internal router item holder.
 */
class MenuRoutingItem {

  /* @var array $routerItem Internal router item holder. */
  private $routerItem;

  /**
   * Class constructor.
   *
   * @param array $router_item
   *   Array which is output of either menu_get_item() or one item from
   *   menu_get_router() function.
   */
  public function __construct(array $router_item) {
    $this->routerItem = $router_item;
  }

  /**
   * Returns page callback function name for this router/menu item.
   *
   * Looks up for callback function name under following aliases:
   * - 'page callback'
   * - 'page_callback'
   *
   * @return string
   *   Page callback function name.
   */
  public function getPageCallbackName() {
    $page_callback = '';

    // Sometimes it's 'page_callback', sometimes, 'page callback' - let's try
    // to handle that here.
    if (isset($this->routerItem['page callback'])) {
      $page_callback = $this->routerItem['page callback'];
    }
    elseif (isset($this->routerItem['page_callback'])) {
      $page_callback = $this->routerItem['page_callback'];
    }

    return $page_callback;
  }

}
