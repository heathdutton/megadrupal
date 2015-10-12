<?php


abstract class dqx_adminmenu_InjectedAPI_abstract {

  protected $_items;
  protected $_inline;

  function itemSetValue($k_item, $key, $value) {
    if (isset($this->_items[$k_item])) {
      $this->_items[$k_item][$key] = $value;
    }
  }

  function itemSetValues($k_item, array $values) {
    if (isset($this->_items[$k_item])) {
      foreach ($values as $k => $v) {
        $this->_items[$k_item][$k] = $v;
      }
    }
  }

  function itemSetNestedValue($k_item, $k1, $k2_or_value) {
    $args = func_get_args();
    $value = array_pop($args);
    $k_item = array_shift($args);
    if (isset($this->_items[$k_item])) {
      $this->_arraySetNestedValue($this->_items[$k_item], $args, $value);
    }
  }

  protected function _arraySetNestedValue(array &$arr, array $keys, $value) {
    if (!count($keys)) {
      // ouch
    }
    else {
      $key = array_shift($keys);
      if (!count($keys)) {
        $arr[$key] = $value;
      }
      else {
        if (!isset($arr[$key])) {
          $arr[$key] = array();
        }
        $this->_arraySetNestedValue($arr[$key], $keys, $value);
      }
    }
  }

  function itemSetWeight($k_item, $weight) {
    if (isset($this->_items[$k_item])) {
      $this->_items[$k_item]['weight'] = $weight;
    }
  }

  function itemSetTitle($k_item, $title) {
    if (isset($this->_items[$k_item])) {
      $this->_items[$k_item]['title'] = $title;
    }
  }

  function itemSetAttribute($k_item, $attr_key, $attr_value) {
    if (isset($this->_items[$k_item])) {
      $this->_items[$k_item]['item_attributes'][$attr_key] = $attr_value;
    }
  }

  function itemAddClass($k_item, $class) {
    if (isset($this->_items[$k_item])) {
      $this->_items[$k_item]['item_attributes']['class'] .= ' ' . $class;
    }
  }

  function getItem($k_item) {
    return $this->_items[$k_item];
  }

  function submenuSetInlineChild($k_parent, $k_child) {
    $this->_inline[$k_parent] = $k_child;
  }
}
