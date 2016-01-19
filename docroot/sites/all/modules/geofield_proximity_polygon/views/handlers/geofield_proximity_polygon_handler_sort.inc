<?php

/**
 * @file
 * Distance sort implementation for Geofield polygons.
 */

class geofield_proximity_polygon_handler_sort extends geofield_handler_sort {

  /**
   * Required because unlike single points, distance calculation for polygons
   * does NOT happen via a database query.
   * So we do it here, through PHP code.
   */
  function post_execute(&$values) {

    if (!isset($this->definition['field_name'])) {
      return;
    }
    $is_asc = !isset($this->query->orderby[0]) || $this->query->orderby[0]['direction'] != "DESC";
    $compare_func = $is_asc ? 'compare_distance_asc' : 'compare_distance_desc';
    uasort($values, array($this, $compare_func));
  }

  /**
   * Compares values of the distance field on two View result rows.
   *
   * @param object $row1, Views result row object
   * @param object $row2  Views result row object
   *
   * @return int, 0, 1, -1 representing =, >, <
   */
  function compare_distance_asc($row1, $row2) {

    $result_name = geofield_proximity_polygon_views_result_name($this);

    // If a row does not have a distance field, treat it as zero.
    if (!isset($row1->$result_name)) {
      return -1;
    }
    if (!isset($row2->$result_name)) {
      return 1;
    }
    $d1 = $row1->$result_name;
    $d2 = $row2->$result_name;
    return ($d1 == $d2) ? 0 : ($d1 > $d2 ? 1 : -1);
  }

  /**
   * Compares values of the distance field on two View result rows.
   *
   * @param object $row1, Views result row object
   * @param object $row2  Views result row object
   *
   * @return int, 0, -1, 1 representing =, >, <
   */
  function compare_distance_desc($row1, $row2) {
    return -$this->compare_distance_asc($row1, $row2);
  }
}
