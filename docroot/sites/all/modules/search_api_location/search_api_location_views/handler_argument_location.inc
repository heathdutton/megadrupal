<?php

/**
 * @file
 * Provides the views argument handler for location fields.
 */

/**
 * Base class for Views location seperate point radius arguments.
 */
class SearchApiViewsHandlerArgumentLocation extends SearchApiViewsHandlerArgument {

  /**
   * Internal function to merge options into search options for field.
   */
  protected function _add_field_options(&$original_options, $add_options, $field) {
    foreach ($original_options as $key => &$field_options) {
      if ($field_options['field'] == $field) {
        // Found add options to array.
        $field_options += $add_options;
        return;
      }
    }
    // Field not yet in options, create new element.
    $add_options['field'] = $field;
    $original_options[] = $add_options;
    return;
  }

  /**
   * Internal function to parse a string argument to point to lat lon.
   */
  protected function _parse_point($argument) {
    $point = array();

    if (module_exists('geophp')) {
      // Try to use geoPHP to read type.
      try {
        geophp_load();
        $format = geoPHP::detectFormat($argument);
        if ($format) {
          $args = explode(':', $format);
          array_unshift($args, $argument);
          $location = call_user_func_array(array('geoPHP', 'load'), $args);
          $point['lat'] = $location->y();
          $point['lon'] = $location->x();
        }
      }
      catch (Exception $e) {
        // GeoPHP couldn't handle type. Treat as invalid/no argument, silently.
      }
    }

    if (empty($point)) {
      // Try Solr LatLonType.
      if (preg_match("/^([+-]?[0-9]+(?:\\.[0-9]+)?),([+-]?[0-9]+(?:\\.[0-9]+)?)$/", $argument, $match)) {
        $point['lat'] = $match[1];
        $point['lon'] = $match[2];
      }
    }

    return empty($point) ? FALSE : $point;
  }

}

/**
 * Handler class for a Views location arguments.
 */
class SearchApiViewsHandlerArgumentLocationPoint extends SearchApiViewsHandlerArgumentLocation {

  /**
   * Accepts a $this->argument of any value that geoPHP can load.
   */
  public function query($group_by = FALSE) {
   if ($point = $this->_parse_point($this->argument)) {
      // Add lat lon to the options array for this field.
      $location_options = (array) $this->query->getOption('search_api_location');
      $this->_add_field_options($location_options, $point, $this->real_field);
      $this->query->setOption('search_api_location', $location_options);
    }
  }

}

/**
 * Handler class for views location geofilt.
 *
 * Central point set by the argument, radius set by as a filter option.
 */
class SearchApiViewsHandlerArgumentLocationGeofilt extends SearchApiViewsHandlerArgumentLocation {

  function option_definition() {
    $options = parent::option_definition();
    $options['radius'] = array('default' => 10);
    return $options;
  }

  function options_validate(&$form, &$form_state) {
    if (! is_numeric($form_state['values']['options']['radius']) ||
          $form_state['values']['options']['radius'] <= 0) {
      form_error($form['radius'], t('You have to enter a numeric radius greater than 0.'));
    }
    parent::options_validate($form, $form_state);
  }

  function options_form(&$form, &$form_state) {
    $form['radius'] = array(
      '#type' => 'textfield',
      '#title' => t('Radius'),
      '#default_value' => $this->options['radius'],
      '#description' => t('The radius (in km) around the argument point to set the distance filter.'),
    );
    parent::options_form($form, $form_state);
  }

  function query($group_by = FALSE) {
    if ($geofilt = $this->_parse_point($this->argument)) {
      // add radius from options
      $geofilt['radius'] = $this->options['radius'];
      $location_options = (array) $this->query->getOption('search_api_location');
      $this->_add_field_options($location_options, $geofilt, $this->real_field);
      $this->query->setOption('search_api_location', $location_options);
    }
  }

}

/**
 * Handler class for a Views location argument radius.
 */
class SearchApiViewsHandlerArgumentLocationRadius extends SearchApiViewsHandlerArgumentLocation {

  /**
   * Accepts a decimal $this->argument representing the radius in km.
   */
   function query($group_by = FALSE) {
    // must be single; must be a decimal
    if (is_numeric($this->argument) && $this->argument > 0) {
      $location_options = (array) $this->query->getOption('search_api_location');
      $add_options = array(
        'radius' => $this->argument,
      );
      $this->_add_field_options($location_options, $add_options, $this->real_field);
      $this->query->setOption('search_api_location', $location_options);
    }
  }

}
