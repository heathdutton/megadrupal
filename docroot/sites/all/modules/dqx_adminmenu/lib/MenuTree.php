<?php


class dqx_adminmenu_MenuTree {

  protected $_submenus;
  protected $_items;
  protected $_inline;

  function __construct(array $submenus, array $items, array $inline_children) {
    $this->_submenus = $submenus;
    $this->_items = $items;
    $this->_inline = $inline_children;
  }

  function renderMenu($parent_path) {
    return $this->_renderSubmenu($parent_path);
  }

  function pathAccess($path) {
    $item = $this->_items[$path];
    if ($item && isset($item['access'])) {
      return $item['access'];
    }
    else {
      $router_item = menu_get_item($path);
      return $router_item['access'];
    }
  }

  function getItem($key) {
    return isset($this->_items[$key]) ? $this->_items[$key] : NULL;
  }

  function getSubmenu($key) {
    return isset($this->_submenus[$key]) ? $this->_submenus[$key] : NULL;
  }

  /**
   * Build link attributes, and alter item attributes.
   * This is used by plugins.
   */
  function itemLinkAttributes($key, $item_attr) {
    $item = $this->getItem($key);
    if ($item) {
      // late access check
      if (!isset($item['access'])) {
        $router_item = menu_get_item($item['link_path']);
        $item['access'] = $router_item['access'];
      }

      $options = $item['localized_options'];
      $options = is_array($options) ? $options : array();

      $link_attr = dqx_adminmenu_link_attributes($item['link_path'], $options);
      if (isset($item['item_attributes'])) {
        $item_attr->mergeAttributes($item['item_attributes']);
      }

      if (!$item['access']) {
        // no access, but still want to reveal some of the subtree items
        $item_attr->addClass('no-access');
        $link_attr->addClass('no-access');
        $link_attr->removeAttribute('href');
      }

      return $link_attr;
    }
  }

  protected function _renderSubmenu($parent_path, $inline = FALSE) {

    $html = '';
    $submenu = @$this->_submenus[$parent_path];

    if (is_object($submenu)) {
      return $submenu->render($this);
    }

    $items_html = '';
    if (is_array($submenu)) {

      if (!$inline && isset($this->_inline[$parent_path])) {
        $inline_path = $this->_inline[$parent_path];
        $inline_item = $this->_items[$inline_path];
        $inline_items_html = $this->_renderSubmenuItem($inline_path, $inline_item, TRUE);
        unset($submenu[$inline_path]);
      }
      else {
        $inline_items_html = '';
      }

      if (!empty($inline_items_html) || !$this->_submenuCanBeSkipped($submenu)) {
        $items_html .= $this->_renderSubmenuItems($submenu, $inline);
        $items_html .= $inline_items_html;
      }
    }

    // TODO: This check was not necessary in D6. Why?
    if (empty($this->_items[$parent_path])) {
      return;
    }
    $ul_attr = dqx_adminmenu_extract_attributes($this->_items[$parent_path], 'submenu_attributes');
    return $inline ? $items_html : $ul_attr->UL($items_html, TRUE, '');
  }

  /**
   * We want to skip lonely items such as user/%/edit/account,
   * if these show the same as the parent.
   */
  protected function _submenuCanBeSkipped($submenu) {
    if (count($submenu) > 1) {
      return FALSE;
    }
    foreach ($submenu as $k => $path) {
      if (!empty($this->_submenus[$path])) {
        return FALSE;
      }
      if ($item = @$this->_items[$path]) {
        if (!($item['type'] & MENU_LINKS_TO_PARENT)) {
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  protected function _renderSubmenuItems($submenu, $inline = FALSE) {
    $sort = array();
    $pieces = array();
    foreach ($submenu as $k => $path) {
      if ($item = $this->_items[$path]) {
        $item_html = $this->_renderSubmenuItem($path, $item, FALSE, $inline);
        if (strlen($item_html)) {
          $pieces[$k] = $item_html;
          $sort[$k] = (50000 + @$item['weight']) . ' ' . $item['title'];
        }
      }
    }
    array_multisort($sort, $pieces);
    return implode('', $pieces);
  }

  protected function _renderSubmenuItem($path, $item, $inline_parent = FALSE, $inline_child = FALSE) {

    // late access check
    if (!isset($item['access'])) {
      $router_item = menu_get_item($item['link_path']);
      $item['access'] = $router_item['access'];
    }

    $subtree_html = $this->_renderSubmenu($path, $inline_parent);

    $options = !empty($item['localized_options']) ? $item['localized_options'] : array();

    $link_attr = dqx_adminmenu_link_attributes($item['link_path'], $options);
    $li_attr = dqx_adminmenu_extract_attributes($item, 'item_attributes');

    if ($inline_child) {
      $li_attr->addClass('inline-child');
    }

    if (!$item['access']) {
      if (!$subtree_html) {
        return;
      }
      // no access, but still want to reveal some of the subtree items
      $li_attr->addClass('no-access');
      $link_attr->addClass('no-access');
      $link_attr->removeAttribute('href');
    }

    if ($subtree_html && !$inline_parent) {
      $li_attr->addClass('expandable');
      $link_attr->addClass('expandable');
    }

    $link_html = $link_attr->A($item['title'], @$options['html']);
    if ($inline_parent) {
      $li_attr->addClass('inline-parent');
      $li_attr->removeClass('expandable');
      return $li_attr->LI($link_html) . $subtree_html;
    }
    else {
      return $li_attr->LI($link_html . $subtree_html);
    }
  }
}
