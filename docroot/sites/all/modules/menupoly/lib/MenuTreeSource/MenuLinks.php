<?php


/**
 * Builds menu trees from settings arrays,
 * using the {menu_links} db table as a source for menu items.
 *
 * This class is the heart and the most complex part of menupoly.
 * In previous versions it was split into multiple classes, but this approach
 * failed at reducing complexity. Maybe this is just the best we can get.
 *
 * TODO:
 *   Find a more convincing way to slice this up.
 *   We want separate unit-testable pieces!
 */
class menupoly_MenuTreeSource_MenuLinks implements menupoly_MenuTreeSource_Interface {

  /**
   * @var array|object
   *   Items in the "active trail".
   *   @todo: This needs documentation.
   */
  protected $trailItems = array();

  /**
   * {@inheritdoc}
   */
  function setTrailPaths(array $paths) {
    $this->trailItems = new menupoly_MenuTreeSource_MenuLinks_TrailItems($paths);
  }

  /**
   * {@inheritdoc}
   */
  function build(array $settings) {

    // Determine the root condition.
    $root_condition = $this->_dynamicRootCondition($settings);
    if (FALSE === $root_condition) {
      // The menu would be empty, so we stop right here.
      return array(0, array());
    }
    $trail_mlids = $this->trailItems->mlids($root_condition);

    // Fetch the actual menu items.
    $q = $this->_selectItems($settings['filter_by_language']);
    $root_condition->applyFinal($q, $settings, $trail_mlids);

    $items = $q->execute()->fetchAllAssoc('mlid', PDO::FETCH_ASSOC);

    if ($settings['expand'] & MENUPOLY_EXPAND_EXPANDED) {
      // Expand items set as "expanded".
      $mlids = array();
      foreach ($items as $mlid => $item) {
        $mlids[$mlid] = $mlid;
      }
      if (isset($plids)) {
        // Skip items that are already expanded due to being in the active trail.
        foreach ($plids as $plid) {
          unset($mlids[$plid]);
        }
      }
      $this->_expandExpanded($items, $mlids, $settings['filter_by_language']);
    }

    // Mark the active trail.
    foreach ($trail_mlids as $mlid) {
      if (isset($items[$mlid])) {
        $items[$mlid]['active-trail'] = TRUE;
      }
    }

    return array($root_condition->getRootMlid(), $items);
  }

  /**
   * Load submenu items for expanded items, recursively.
   * This only happens in case of MENUPOLY_EXPAND_EXPANDED.
   *
   * @param array &$items
   *   Items array to grow.
   * @param array $plids
   *   plids for the current recursion step.
   * @param array|bool $filter_by_language
   *   Allowed languages for menu items.
   */
  protected function _expandExpanded(&$items, array $plids, $filter_by_language = FALSE) {

    foreach ($plids as $k => $plid) {
      if (empty($items[$plid]['expanded'])) {
        unset($plids[$k]);
      }
    }
    if (!empty($plids)) {
      $q = $this->_selectItems($filter_by_language);
      $q->condition('plid', $plids);
      $items_new = $q->execute()->fetchAllAssoc('mlid', PDO::FETCH_ASSOC);
      $plids = array_keys($items_new);
      $items += $items_new;
      $this->_expandExpanded($items, $plids);
    }
  }

  /**
   * Build a root condition to be applied to all queries.
   * The effect is generally that only the sub-tree under a specific plid is
   * being loaded.
   * This may be based on a static root plid, or a dynamic root plid depending
   * on the active trail.
   *
   * @param array $settings
   *   Settings array.
   *
   * @return bool|menupoly_MenuTreeSource_MenuLinks_RootCondition_Interface
   *   Root condition to be applied to all queries.
   */
  protected function _dynamicRootCondition(array $settings) {
    $root_condition = $this->_settingsRootCondition($settings);
    if (FALSE === $root_condition) {
      return FALSE;
    }
    if (!empty($settings['follow'])) {
      // Root item to follow the active page.
      // Find deepest trail item within the current menu / submenu
      if ($settings['follow'] === MENUPOLY_FOLLOW_CHILDREN) {
        $root_item = $this->trailItems->deepest($root_condition);
      }
      else {
        $root_item = $this->trailItems->parentOfDeepest($root_condition);
      }
      if (!empty($root_item)) {
        return new menupoly_MenuTreeSource_MenuLinks_RootCondition_RootItem($root_item);
      }
    }
    elseif (!empty($settings['level']) && $settings['level'] > 1) {
      // Root item at a specified level.
      $root_item = $this->trailItems->withDepth($root_condition, $settings['level'] - 1);
      if (!empty($root_item)) {
        return new menupoly_MenuTreeSource_MenuLinks_RootCondition_RootItem($root_item);
      }
      else {
        return FALSE;
      }
    }
    return $root_condition;
  }

  /**
   * @param array $settings
   *   Settings array.
   *
   * @return bool|menupoly_MenuTreeSource_MenuLinks_RootCondition_Interface
   *   Root condition to apply to all queries, based on settings array.
   */
  protected function _settingsRootCondition(array $settings) {
    $q = db_select('menu_links', 'ml')->fields('ml');
    if (!empty($settings['root_mlid'])) {
      $q->condition('mlid', $settings['root_mlid']);
    }
    elseif (!empty($settings['root_path'])) {
      $q->condition('link_path', $settings['root_path']);
    }
    elseif (empty($settings['menu_name'])) {
      return FALSE;
    }
    else {
      return new menupoly_MenuTreeSource_MenuLinks_RootCondition_MenuName($settings['menu_name']);
    }
    $item = $q->execute()->fetchAssoc();
    if (empty($item)) {
      return FALSE;
    }
    if (
      isset($settings['menu_name']) &&
      $item['menu_name'] !== $settings['menu_name']
    ) {
      return FALSE;
    }
    return new menupoly_MenuTreeSource_MenuLinks_RootCondition_RootItem($item);
  }

  /**
   * @param array|bool $filter_by_language
   *   Allowed languages for menu items.
   *
   * @return SelectQuery
   *   Select query for menu_links joined with menu_router.
   *   Hidden items are filtered out.
   *   Results ordered by trail.
   */
  protected function _selectItems($filter_by_language = FALSE) {
    $q = $this->_selectMenuLinks()->fields('ml');
    $q->leftJoin('menu_router', 'm', 'm.path = ml.router_path');
    $q->fields('m', array(
      'load_functions', 'to_arg_functions', 'access_callback',
      'access_arguments', 'page_callback', 'page_arguments', 'title',
      'title_callback', 'title_arguments', 'type', 'description',
    ));
    if (is_array($filter_by_language)) {
      $q->condition('ml.language', $filter_by_language, 'IN');
    }
    for ($i = 1; $i <= 9; ++$i) {
      $q->orderBy("p$i", 'ASC');
    }
    return $q;
  }

  /**
   * @return SelectQuery
   *   Select query for menu_links table.
   *   Hidden items are filtered out.
   *   The query does not contain any fields yet.
   */
  protected function _selectMenuLinks() {
    $q = db_select('menu_links', 'ml');
    $q->condition('hidden', 1, '!=');
    return $q;
  }
}
