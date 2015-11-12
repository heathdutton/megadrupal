<?php


class dqx_adminmenu_InjectedAPI_itemsAlter extends dqx_adminmenu_InjectedAPI_abstract {

  protected $_parent_map;

  function __construct(&$items, &$parent_map, &$inline_children) {
    $this->_items = &$items;
    $this->_parent_map = &$parent_map;
    $this->_inline = &$inline_children;
  }

  function addItem($key, $item) {
    $this->_items[$key] = $item;
  }

  function moveItem($keyOld, $keyNew) {
    if (isset($this->_items[$keyOld])) {
      $this->_items[$keyNew] = $this->_items[$keyOld];
      unset($this->_items[$keyOld]);
    }
  }

  function removeItem($key) {
    unset($this->_items[$key]);
  }

  function parentMap($k_parent, $k_parent_replaced) {
    $this->_parent_map[$k_parent] = $k_parent_replaced;
  }
}
