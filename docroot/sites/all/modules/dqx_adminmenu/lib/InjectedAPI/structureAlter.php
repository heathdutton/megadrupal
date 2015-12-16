<?php


class dqx_adminmenu_InjectedAPI_structureAlter extends dqx_adminmenu_InjectedAPI_abstract {

  protected $_submenus;

  function __construct(&$items, &$submenus, &$inline_children) {
    $this->_items = &$items;
    $this->_submenus = &$submenus;
    $this->_inline = &$inline_children;
  }

  function hasSubmenu($k_submenu) {
    return is_array($this->_submenus[$k_submenu]);
  }

  function setSubmenu($k_submenu, $submenu) {
    $this->_submenus[$k_submenu] = $submenu;
  }

  function submenuSetAttribute($k_submenu, $attr_name, $attr_value) {
    if (isset($this->_submenus[$k_submenu])) {
      $this->_items[$k_submenu]['submenu_attributes'][$attr_name] = $attr_value;
    }
  }

  function submenuAddClass($k_submenu, $class) {
    if (isset($this->_submenus[$k_submenu])) {
      $this->_items[$k_submenu]['submenu_attributes']['class'] .= ' ' . $class;
    }
  }

  function submenuGetPaths($k_submenu) {
    if (isset($this->_submenus[$k_submenu])) {
      return $this->_submenus[$k_submenu];
    }
    return array();
  }

  function submenuCountChildren($k_submenu) {
    if (isset($this->_submenus[$k_submenu])) {
      return count($this->_submenus[$k_submenu]);
    }
    return 0;
  }

  function submenuAddChild($k_submenu, $path) {
    if (isset($this->_submenus[$k_submenu]) || TRUE) {
      $this->_submenus[$k_submenu][$path] = $path;
    }
  }

  function submenuRemoveChild($k_submenu, $path) {
    unset($this->_submenus[$k_submenu][$path]);
  }
}
