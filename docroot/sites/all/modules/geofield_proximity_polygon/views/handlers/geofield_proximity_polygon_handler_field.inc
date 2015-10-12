<?php

/**
 * @file
 * Distance field implementation for Geofield polygons.
 */

class geofield_proximity_polygon_handler_field extends geofield_handler_field {

  // No override, keep the base query() for distance calculations on points.
  //function query() {
  //  parent::query();
  //}

  /**
   * Modifies View results set upon return from the database.
   *
   * @global object $user
   * @param array $values
   *
   * The base handler geofield_handler_field passes to the database a query to
   * return distances from the origin point to the centroid of the polygon.
   * This version differs and returns instead:
   * o zero, if the source of origin is inside the polygon or one of the
   *   polygons in a multipolygon or multi-valued geofield of polygons
   * o the distance to the nearest corner point of the polygon or polygons, when
   *   outside all of the polygons
   */
  function post_execute(&$values) {
    // Sanity checks. None of these should ever return.
    if (empty($values) || !isset($this->definition['field_name']) || $this->field !== 'field_geofield_distance') {
      return;
    }
    $proximity_plugin = geofield_proximity_load_plugin($this->options['source']);
    if (!$proximity_plugin) {
      gpp_debug(t('GPP: no active Geofield proximity plugin'), 'error');
      return;
    }
    $options = $proximity_plugin->getSourceValue($this);
    if (!$options || !isset($options['latitude'])) {
      // This happens for geofieldProximityExposedFilter etc. and is normal.
      return;
    }
    $reference_point = array(
      'lat' => $options['latitude'],
      'lon' => $options['longitude']
    );
    $earth_radius = empty($this->options['radius_of_earth']) ? NULL : $this->options['radius_of_earth'];

    $field_name  = geofield_proximity_polygon_views_field_name($this);
    $result_name = geofield_proximity_polygon_views_result_name($this);

    $this->debug($values, $field_name, $result_name);

    $is_debug = gpp_debug_flag();
    foreach ($values as $r => $row) {
      if (!empty($row->$field_name)) {
        $polygons = array();
        // This loop deals with single-valued as well as multi-valued Geofields.
        foreach ($row->$field_name as $geofield_value) {
          // Support WKT polygons and WKT multipolygons. The next call requires:
          //  $geofield_value['raw']['geo_type'] e.g., 'polygon' or 'multipolygon'
          //  $geofield_value['raw']['geom'] e.g., 'POLYGON((145 -37, 146 -38))'
          $polygons = array_merge($polygons, geofield_proximity_polygon_extract($geofield_value['raw']));
        }
        // For single points $polygons is empty and no action will be taken.
        if (!empty($polygons)) {
          // Apply point-in-polygon algorithm, calculate distance when outside.
          if ($is_debug && ($id = reset($row))) {
            $title = isset($row->taxonomy_term_data_name) ? filter_xss_admin($row->taxonomy_term_data_name) :
              (isset($row->node_title) ? filter_xss_admin($row->node_title) : '');
            drupal_set_message(t('GPP: id #') . $id . ($title ? ', "' . $title . '"' : '') . ':');
          }
          $distance = geofield_proximity_polygon_distance_to_polygon($reference_point, $polygons, $earth_radius);
          $values[$r]->$result_name = $distance;
        }
      }
    }
  }

  private function debug($values, $field_name, $result_name) {
    global $user;

    if (gpp_debug_flag() || $user->uid === 1) {

      // Pull field names out of first view result row (other rows will be same).
      $field_names = array_keys((array) reset($values));

      gpp_debug(t('GPP: the View contains these result names:<br/>%names', array('%names' => implode(', ', $field_names))));
      if (in_array($field_name, $field_names)) {
        gpp_debug(t('GPP: geofield %name found amongst View results.', array('%name' => $field_name)));
      }
      else {
        drupal_set_message(t('GPP: geofield field %name could NOT be found.', array('%name' => $field_name)), 'warning');
      }
      if (in_array($result_name, $field_names)) {
        gpp_debug(t('GPP: Calculated proximities will be written to field %name', array('%name' => $result_name)));
      }
      else {
        drupal_set_message(t('GPP: expected field %name was not found to write calculated proximities to.', array('%name' => $result_name)), 'warning');
      }
    }
  }
}
