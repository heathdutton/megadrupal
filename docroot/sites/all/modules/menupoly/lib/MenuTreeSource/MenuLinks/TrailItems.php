<?php


class menupoly_MenuTreeSource_MenuLinks_TrailItems {

  /**
   * @var array
   *   Paths in the "active trail".
   */
  protected $trailPaths;

  /**
   * @param array $trail_paths
   *   Paths in the "active trail".
   */
  function __construct($trail_paths) {
    $this->trailPaths = $trail_paths;
  }

  /**
   * @param object $root_condition
   *   Condition to be applied to all queries.
   *
   * @return array
   *   Mlids for "active trail" items that match the root condition.
   */
  function mlids($root_condition) {
    $deep_trail_item = $this->deepest($root_condition);
    if (empty($deep_trail_item)) {
      return array(0);
    }
    $mlids = array(0);
    for ($i = 1; !empty($deep_trail_item['p' . $i]); ++$i) {
      $mlids[] = $deep_trail_item['p' . $i];
    }
    return $mlids;
  }

  /**
   * Of all menu links whose path is in the current active trail, this method
   * returns the one where
   * - the path is th deepest within the active trail paths
   * - the depth of the menu link is highest
   *
   * @param boolean|object $root_condition
   *   Condition to apply to all queries.
   *
   * @return array
   *   Deepest menu link in active trail.
   */
  function deepest($root_condition) {
    $mlid = $this->fetchDeepest($root_condition, 'mlid');
    $link = $this->fetchLink($mlid);
    return $link;
  }

  /**
   * Same as above, but returns the parent of this item.
   *
   * @param boolean|object $root_condition
   *   Condition to apply to all queries.
   *
   * @return array
   *   Parent of deepest menu link in active trail.
   */
  function parentOfDeepest($root_condition) {
    $mlid = $this->fetchDeepest($root_condition, 'plid');
    $link = $this->fetchLink($mlid);
    return $link;
  }

  /**
   * Of all menu links whose path is in the current active trail, this method
   * returns the one where
   * - the path is th deepest within the active trail paths
   * - the depth is exactly as $depth.
   *
   * @param boolean|object $root_condition
   *   Condition to apply to all queries.
   *
   * @param int $depth
   *
   * @return array
   *   Parent of deepest menu link in active trail.
   */
  function withDepth($root_condition, $depth) {
    $mlid = $this->fetchDeepest($root_condition, 'mlid', $depth);
    return $this->fetchLink($mlid);
  }

  /**
   * @param boolean|object $root_condition
   *   Condition to apply to all queries.
   * @param string $field
   * @param int|NULL $depth
   *
   * @return int|string|null
   *   Mlid of deepest trail item, or NULL if none found.
   */
  protected function fetchDeepest($root_condition, $field, $depth = NULL) {
    $q = db_select('menu_links', 'ml')->fields('ml', array('link_path', $field));
    $root_condition->apply($q);
    $q->condition('link_path', $this->trailPaths);
    if (isset($depth)) {
      $q->condition('depth', $depth);
    }
    else {
      $q->orderBy('depth', 'ASC');
    }
    $sorted = array();
    foreach ($q->execute()->fetchAll(PDO::FETCH_ASSOC) as $item) {
      $sorted[$item['link_path']] = $item[$field];
    }
    foreach (array_reverse($this->trailPaths) as $path) {
      if (isset($sorted[$path])) {
        return $sorted[$path];
      }
    }
    return NULL;
  }

  /**
   * @param int|string $mlid
   *   The mlid to fetch the link for.
   *
   * @return array|NULL
   *   Menu link with that mlid.
   */
  protected function fetchLink($mlid) {
    if (!empty($mlid)) {
      $q = db_select('menu_links', 'ml')->fields('ml');
      $q->condition('mlid', $mlid);
      $link = $q->execute()->fetchAssoc();
    }
    return isset($link) ? $link : NULL;
  }
}
