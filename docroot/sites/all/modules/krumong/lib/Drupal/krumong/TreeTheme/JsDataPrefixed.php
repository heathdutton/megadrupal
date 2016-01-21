<?php

namespace Drupal\krumong;


/**
 * Render the tree as nested javascript value/object.
 * This is not a simple json_encode(), and it is not valid json, so only works
 * if run as full javascript.
 *
 * Will prefix every array or object key, to avoid name clashes with reserved
 * names such as "__proto__".
 * Additional keys for an object's class.
 */
class TreeTheme_JsDataPrefixed implements TreeTheme_Interface {

  function renderArray($array, $position, $children) {
    return $this->renderArrayOrObject($children);
  }

  function renderObject($object, $position, $children) {
    $class = json_encode(get_class($object));
    return $this->renderArrayOrObject($children, array("  class: $class"));
  }

  protected function renderArrayOrObject($children, $parts = array()) {
    foreach ($children as $k => $v) {
      $k = json_encode(".$k");
      $parts[] = "  $k: $v";
    }
    return "{\n" . implode(",\n", $parts) . "\n}";
  }

  function renderValue($value, $position) {
    return json_encode($value);
  }

  function renderRecursion($value, $position, $original_position) {
    return json_encode(array('recursion' => $original_position));
  }

  function wrap($string, $called_from, $name = NULL) {
    $called_from = json_encode($called_from);
    $name = json_encode($name);
    return "{\n  data: $string,\n  calledFrom: $called_from, name: $name\n}";
  }
}
