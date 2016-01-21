<?php

/**
 * A condition to filter a select query on menu_links by root item.
 */
class menupoly_MenuTreeSource_MenuLinks_RootCondition_RootItem implements menupoly_MenuTreeSource_MenuLinks_RootCondition_Interface {

  /**
   * @var array
   *   The root item.
   */
  protected $rootItem;

  /**
   * @param array $root_item
   *   The root item.
   */
  function __construct($root_item) {
    $this->rootItem = $root_item;
  }

  /**
   * {@inheritdoc}
   */
  function apply($q) {
    $q->condition('p'. $this->rootItem['depth'], $this->rootItem['mlid']);
  }

  /**
   * {@inheritdoc}
   */
  function applyFinal($q, $settings, $trail_mlids) {
    $mindepth = $this->rootItem['depth'] + 1;
    $maxdepth = isset($settings['depth']) ? $mindepth + $settings['depth'] - 1 : NULL;
    $depth = isset($settings['depth']) ? $settings['depth'] : NULL;
    if ($settings['expand'] === MENUPOLY_EXPAND_ALL) {
      // Expand the full tree.
      // Don't add further conditions.
      $this->apply($q);
      if (isset($maxdepth)) {
        $q->condition('depth', $maxdepth, '<=');
      }
    }
    else {
      if ($settings['expand'] & MENUPOLY_EXPAND_ACTIVE) {
        $plids = array_slice($trail_mlids, $mindepth - 1, $depth);
        $plids[] = $this->rootItem['mlid'];
        $plids = array_unique($plids);
        if (count($plids) > 1) {
          // Note: If $plids is empty, this condition will always be false.
          $q->condition('plid', $plids);
          return;
        }
      }
      $q->condition('plid', $this->rootItem['mlid']);
    }
  }

  /**
   * {@inheritdoc}
   */
  function getRootMlid() {
    return $this->rootItem['mlid'];
  }

  /**
   * @return int
   *   Depth of the root item.
   */
  function getRootDepth() {
    return $this->rootItem['depth'];
  }
}
