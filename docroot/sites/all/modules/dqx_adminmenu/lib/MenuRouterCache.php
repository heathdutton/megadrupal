<?php


class dqx_adminmenu_MenuRouterCache {

  protected $items = array();
  protected $prefixes = array();
  protected $children = array();

  function getTreeItems($seedPath) {

    $routerItem = menu_get_item($seedPath);
    $route = $routerItem['path'];
    $this->loadTree($route);
    // The link path might be different from the seed path.
    $seedLinkPath = $routerItem['href'];

    $children = array();
    $this->collectChildrenRecursive($children, $route, $seedLinkPath);

    if (!empty($this->items[$route]) && $seedPath === $seedLinkPath) {
      $children[$seedPath] = $this->items[$route];
      $children[$seedPath]['link_path'] = $seedLinkPath;
    }

    $items = array();
    foreach ($children as $linkPath => $item) {
      $item = $this->prepareItem($item);
      if ($item) {
        $items[$linkPath] = $item;
      }
    }

    return $items;
  }

  function forceItem($linkPath) {

    $routerItem = menu_get_item($linkPath);
    $route = $routerItem['path'];
    $this->loadTree($route);

    if ($routerItem['href'] !== $linkPath) {
      $linkPath = $routerItem['href'];
    }

    if (!empty($this->items[$route])) {
      $item = $this->items[$route];
      $item['link_path'] = $linkPath;
      return $this->prepareItem($item, TRUE);
    }
  }

  protected function collectChildrenRecursive(&$children, $parentRoute, $parentPath, $recLevel = 0) {
    if (isset($this->children[$parentRoute])) {
      foreach ($this->children[$parentRoute] as $substr => $childRoute) {
        if ($substr === '%') {
          break;
        }
        $childPath = $parentPath . '/' . $substr;
        if (!empty($this->items[$childRoute])) {
          $children[$childPath] = $this->items[$childRoute];
          $children[$childPath]['link_path'] = $childPath;
        }
        $this->collectChildrenRecursive($children, $childRoute, $childPath, $recLevel + 1);
      }
    }
  }

  protected function prepareItem($item, $force = FALSE) {
    if ($force) {
      // Accept the item, anyway.
    }
    elseif ($item['type'] & (MENU_VISIBLE_IN_TREE | MENU_VISIBLE_IN_BREADCRUMB | MENU_IS_LOCAL_TASK)) {
      // Accept the item.
    }
    elseif ($item['type'] & MENU_VISIBLE_IN_BREADCRUMB) {
      // Accept the item.
      // TODO: That's a strange heuristic, isn't it?
      // TODO: How does this needs_approval work??
      $item['needs_approval'] = TRUE;
    }
    else {
      // Skip this item.
      return;
    }
    $item['route'] = $item['path'];
    // TODO: We would prefer to keep 'path' as the router path.
    $item['path'] = $item['link_path'];

    return $this->prepareMenuItem($item);
  }

  protected function prepareMenuItem($item) {

    if ($item['type'] == MENU_CALLBACK) {
      $item['hidden'] = -1;
    }
    elseif ($item['type'] == MENU_SUGGESTED_ITEM) {
      $item['hidden'] = 1;
    }

    // TODO: Why do we need this?
    $item['module'] = 'dqx_adminmenu';

    $item += array(
      'menu_name' => 'dqx_adminmenu',
      'link_title' => $item['title'],
      'link_path' => $item['path'],
      'hidden' => 0,
      'options' => empty($item['description']) ? array() : array(
        'attributes' => array('title' => $item['description'])
      ),

      'router_path' => $item['path'],

      // this is taken from menu_item_save() in D6.
      'weight' => 0,
      'has_children' => 0,
      'expanded' => 1,
      'customized' => 0,
      'updated' => 0,

      // stuff added for D7
      'external' => FALSE,
    );

    // _menu_link_translate() wants serialized options
    $item['options'] = serialize($item['options']);

    // Check access
    // This does expect serialized options, but it does unserialize them again after.
    _menu_link_translate($item);
    if (empty($item['title']) || $item['path'] == 'admin/config/group/group-membership') {
      // dpm($item);
    }

    if ($item['page_callback'] === 'dqx_adminmenu_action_page') {
      @$item['localized_options']['attributes']['class'] .= ' dqx_adminmenu-action';
      @$item['localized_options']['attributes']['class'] .= ' dqx_adminmenu-redirect';
    }

    return $item;
  }

  protected function loadTree($rootPath) {
    if (isset($this->prefixes[$rootPath])) {
      // Already done with this.
      // TODO: Find a more effective way to check!!
      // return;
    }
    $v = str_replace('%', '%%', $rootPath);
    $this->loadByPath($rootPath);
    $this->loadByPath("$v/%", 'LIKE');
  }

  protected function loadByPath($path, $operator = '=') {
    $q = db_select('menu_router', 'mr');
    $q->fields('mr');
    // $q->condition('title', '', '!=');
    $q->condition('path', $path, $operator);
    $q->orderBy('weight', 'ASC');
    $q->orderBy('title', 'ASC');
    $items = $q->execute()->fetchAllAssoc('path', PDO::FETCH_ASSOC);
    foreach ($items as $path => $item) {
      $this->insertItem($path, $item);
    }
  }

  protected function insertItem($path, $item) {
    foreach (array(
      'load_functions',
      // We can't do this one now, because it is done in _menu_check_access().
      // 'access_arguments',
      'page_arguments',
      'theme_arguments',
    ) as $k) {
      if (isset($item[$k])) {
        $item[$k] = unserialize($item[$k]);
      }
    }
    $this->items[$path] = $item;
    $pos = strpos($path, '/');
    while (FALSE !== $pos) {
      $prefix = substr($path, 0, $pos);
      $this->prefixes[$prefix] = TRUE;
      $prev = $pos;
      $pos = strpos($path, '/', $prev + 1);
      if (FALSE === $pos) {
        $substr = substr($path, $prev + 1);
      }
      else {
        $substr = substr($path, $prev + 1, $pos - $prev - 1);
      }
      $this->children[$prefix][$substr] = $prefix . '/' . $substr;
    }
    $this->prefixes[$path] = TRUE;
  }
}

