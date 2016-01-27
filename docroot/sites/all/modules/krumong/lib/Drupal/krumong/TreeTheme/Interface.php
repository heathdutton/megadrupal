<?php

namespace Drupal\krumong;


interface TreeTheme_Interface {

  /**
   * @param array $array
   *   The array at this position.
   * @param TreePosition $position
   *   Object representing the current position in the tree.
   * @param array $children
   *   The rendered array values.
   *
   * @return string
   *   The rendered array.
   */
  function renderArray($array, $position, $children);

  /**
   * @param object $object
   *   The object at this position.
   * @param TreePosition $position
   *   Object representing the current position in the tree.
   * @param array $children
   *   The rendered object attribute values.
   *
   * @return string
   *   The rendered object.
   */
  function renderObject($object, $position, $children);

  /**
   * @param mixed $value
   *   A string or scalar value.
   * @param TreePosition $position
   *   Object representing the current position in the tree.
   *
   * @return string
   *   The rendered value.
   */
  function renderValue($value, $position);

  /**
   * @param array $value
   *   The value at this position
   * @param TreePosition $position
   *   Object representing the current position in the tree.
   * @param TreePosition $original_position
   *   Object representing the tree position where this object/array previously
   *   occured.
   *
   * @return string
   *   A rendered symbole for recursion.
   */
  function renderRecursion($value, $position, $original_position);

  /**
   * This method can be used to 
   *
   * @param string $string
   *   The rendered tree.
   * @param array $called_from
   *   File and line number of the call to dpm() or similar.
   * @param string $name
   *   Label for the data.
   *
   * @return string
   *   The rendered and wrapped tree.
   */
  function wrap($string, $called_from, $name = NULL);
}
