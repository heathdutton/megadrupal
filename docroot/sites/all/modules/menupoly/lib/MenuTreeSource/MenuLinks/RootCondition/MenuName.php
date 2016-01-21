<?php

/**
 * A condition to filter a select query on menu_links by menu name.
 */
class menupoly_MenuTreeSource_MenuLinks_RootCondition_MenuName implements menupoly_MenuTreeSource_MenuLinks_RootCondition_Interface {

  /**
   * @var string
   *   Machine name of a menu.
   */
  protected $menuName;

  /**
   * @param string $menu_name
   *   Machine name of a menu.
   */
  function __construct($menu_name) {
    $this->menuName = $menu_name;
  }

  /**
   * {@inheritdoc}
   */
  function apply($q) {
    $q->condition('menu_name', $this->menuName);
  }

  /**
   * {@inheritdoc}
   */
  function applyFinal($q, $settings, $trail_mlids) {
    $maxdepth = $depth = isset($settings['depth']) ? $settings['depth'] : NULL;
    $q->condition('menu_name', $this->menuName);
    if ($settings['expand'] === MENUPOLY_EXPAND_ALL) {
      // Expand the full tree.
      if (isset($maxdepth)) {
        $q->condition('depth', $maxdepth, '<=');
      }
    }
    else {
      if ($settings['expand'] & MENUPOLY_EXPAND_ACTIVE) {
        $plids = array_slice($trail_mlids, 0, $depth, TRUE);
        if (!empty($plids)) {
          // Note: If $plids is empty, this condition will always be false.
          $q->condition('plid', $plids);
          return;
        }
      }
      $q->condition('plid', 0);
    }
  }

  /**
   * {@inheritdoc}
   */
  function getRootMlid() {
    return 0;
  }
}
