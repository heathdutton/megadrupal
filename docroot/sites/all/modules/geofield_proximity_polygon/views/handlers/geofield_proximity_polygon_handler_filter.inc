<?php

/**
 * @file
 * Distance filter implementation for Geofield polygons.
 */

class geofield_proximity_polygon_handler_filter extends geofield_handler_filter {

  /**
   * Override geofield_handler_filter to NOT add a WHERE clause.
   */
  function query() {
    return;
  }

  /**
   * Required because unlike single points, distance calculation for polygons
   * does NOT happen via a database query.
   * So we do it through PHP code.
   */
  function post_execute(&$values) {

    if (!isset($this->definition['field_name']) || !empty($this->no_operator)) {
      return;
    }
    if (!$result_name = geofield_proximity_polygon_views_result_name($this)) {
      return;
    }

    foreach ($values as $r => $row) {
      // If the row does not contain a distance or is to be filtered out,
      // remove it from the Views result set.
      if (!isset($row->$result_name) || !$this->filter($row->$result_name)) {
        unset($values[$r]);
      }
    }
  }

  /**
   * Returns whether the supplied result should remain part of the result set.
   *
   * @param float $result, Views result
   *
   * @return boolean, TRUE if the result passes the filter condition
   */
  function filter($result) {
    if (!isset($this->value['distance']) || !isset($this->operator)) {
      // Corrupt handler? Give benefit of doubt.
      return TRUE;
    }
    $distance = $this->value['distance'];
    switch ($this->operator) {
      case '<':
        return $result < $distance;
      case '<=':
        return $result <= $distance;
      case '>':
        return $result > $distance;
      case '>=':
        return $result >= $distance;
      case '=':
        return $result == $distance;
      case '!=':
        return $result != $distance;
      case 'between':
        return $result >= $distance && $result <= $this->value['distance2'];
      case 'not between':
        return $result < $distance || $result > $this->value['distance2'];
      default:
        // What would this be?
        return TRUE;
    }
  }
}
