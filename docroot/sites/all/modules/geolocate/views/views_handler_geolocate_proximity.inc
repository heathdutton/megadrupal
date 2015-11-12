<?php

/**
 * @file
 * Custom views handler for geolocate proximity.
 * (This is mainly boilerplate.)
 */
 
/**
 * Custom handler class.
 *
 * @ingroup views_field_handlers
 */
class views_handler_geolocate_proximity extends views_handler_field {
  /**
   * {@inheritdoc}
   *
   * Not needed for database. (But do need to cache lat/long.)
   */
  function query() {
 
    $this->ensure_my_table();

    $lat_alias = $this->table_alias . '.' . $this->definition['field_name'] . '_lat';
    $lon_alias = $this->table_alias . '.' . $this->definition['field_name'] . '_lon';

    $proximityPlugin = geofield_proximity_load_plugin($this->options['source']);
    $options = $proximityPlugin->getSourceValue($this);

    if ($options != FALSE) {
      $haversine_options = array(
        'origin_latitude' => $options['latitude'],
        'origin_longitude' => $options['longitude'],
        'destination_latitude' => $lat_alias,
        'destination_longitude' => $lon_alias,
        'earth_radius' => $this->options['radius_of_earth'],
      );

      $this->field_alias = $this->query->add_field(NULL, geofield_haversine($haversine_options), $this->table_alias . '_' . $this->field);
    }
  }
 
  /**
   * {@inheritdoc}
   *
   * Modify any end user views settings here. Debug $options to view the field
   * settings you can change.
   */
  function option_definition() {
    $options = parent::option_definition();
  
    // Data sources and info needed.
    $options['source'] = array('default' => 'manual');

    $proximityHandlers = module_invoke_all('proximity_views_handlers');
    foreach ($proximityHandlers as $key => $handler) {
      if (class_exists($handler['class'])) {
        $proximityPlugin = new $handler['class']();
        $proximityPlugin->option_definition($options, $this);
      }
    } 
    
    $options['radius_of_earth'] = array('default' => GEOFIELD_KILOMETERS);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['source'] = array(
      '#type' => 'select',
      '#title' => t('Source of Origin Point'),
      '#description' => t('How do you want to enter your origin point?'),
      '#options' => array(),
      '#default_value' => $this->options['source'],
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'geofield') . '/js/viewsProximityValue.js',
        ),
      ),
    );

    $proximityHandlers = module_invoke_all('proximity_views_handlers');
    foreach ($proximityHandlers as $key => $handler) {
      $form['source']['#options'][$key] = $handler['name'];
      if (class_exists($handler['class'])) {
        $proximityPlugin = new $handler['class']();
        $proximityPlugin->options_form($form, $form_state, $this);
      }
    }

    $form['radius_of_earth'] = array(
      '#type' => 'select',
      '#title' => t('Unit of Measure'),
      '#description' => '',
      '#options' => geofield_radius_options(),
      '#default_value' => $this->options['radius_of_earth'],
    );
    
    return $options;
    
  }

  /**
   * {@inheritdoc}
   *
   * Make changes to the field settings form seen by the end user when adding
   * your field.
   */
  // Another sensibly named function.  
  function views_handler_geolocate_proximity_geolocate_info() { 
        
    $lat = $_SESSION['geolocate']['lat'];
    $long = $_SESSION['geolocate']['long'];
 
    $latlong = array($lat, $long);

    return $latlong;

  }
    
  /**
   * Render callback handler.
   *
   * Return the markup that will appear in the rendered field.
   */
  function render($values) {
    $latlong = $this->views_handler_geolocate_proximity_geolocate_info();

    //@TODO: This should return an array. 
    return implode($latlong, ':');
  }

}



