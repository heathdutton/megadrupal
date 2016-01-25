<?php

/**
 * @file
 * Contains SearchApiViewsHandlerFieldLocation.
 */

/**
 * Specialized handler for latitude/longitude fields.
 */
class SearchApiViewsHandlerFieldLocation extends entity_views_handler_field_text {

  /**
   * The search query constructed and executed by the current view.
   *
   * @var SearchApiViewsQuery
   */
  public $query;

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['show_distance_to_point'] = array('default' => FALSE, 'bool' => TRUE);
    $options['conversion_factor'] = array('default' => 1);
    $options['set_precision'] = array('default' => FALSE, 'bool' => TRUE);
    $options['precision'] = array('default' => 0);
    $options['decimal'] = array('default' => '.', 'translatable' => TRUE);
    $options['separator'] = array('default' => ',', 'translatable' => TRUE);
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['show_distance_to_point'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show distance instead of coordinates'),
      '#default_value' => $this->options['show_distance_to_point'],
      '#description' => t('Shows the distance to the point from the filters instead of the coordinates of the result.'),
    );
    $form['conversion_factor'] = array(
      '#type' => 'select',
      '#title' => t('Distance unit'),
      '#default_value' => $this->options['conversion_factor'],
      '#options' => array(
        t('SI') => array(
          '1' => t('Kilometre'),
          '0.001' => t('Metre'),
          '0.0001' => t('Decimetre'),
        ),
        t('Imperial / US') => array(
          '1.60926939169617' => t('Mile'),
          '0.0050301810865191' => t('Pole / rod / perch'),
          '0.0009140767824497258' => t('Yard'),
          '0.0003047851264858275' => t('Foot'),
        ),
        t('Nautic') => array(
          '1.853224610822832' => t('US / British nautical mile'),
          '1.851851851851852' => t('International nautical mile'),
        ),
      ),
      '#dependency' => array('edit-options-show-distance-to-point' => array(TRUE)),
    );
    $form['set_precision'] = array(
      '#type' => 'checkbox',
      '#title' => t('Round'),
      '#description' => t('If checked, the number will be rounded.'),
      '#default_value' => $this->options['set_precision'],
      '#dependency' => array('edit-options-show-distance-to-point' => array(TRUE)),
    );
    $form['precision'] = array(
      '#type' => 'textfield',
      '#title' => t('Precision'),
      '#default_value' => $this->options['precision'],
      '#description' => t('Specify how many digits to print after the decimal point.'),
      '#size' => 2,
      '#maxlength' => 1,
      '#dependency' => array('edit-options-set-precision' => array(TRUE)),
    );
    $form['decimal'] = array(
      '#type' => 'textfield',
      '#title' => t('Decimal point'),
      '#default_value' => $this->options['decimal'],
      '#description' => t('What single character to use as a decimal point.'),
      '#size' => 2,
      '#maxlength' => 1,
      '#dependency' => array('edit-options-show-distance-to-point' => array(TRUE)),
    );
    $form['separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Thousands marker'),
      '#default_value' => $this->options['separator'],
      '#description' => t('What single character to use as the thousands separator.'),
      '#size' => 2,
      '#maxlength' => 1,
      '#dependency' => array('edit-options-show-distance-to-point' => array(TRUE)),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // If the distance has to be displayed add the flag to the spatial settings
    // of this field.
    if (!empty($this->options['show_distance_to_point'])) {
      $spatials = (array) $this->query->getOption('search_api_location', array());
      foreach ($spatials as $i => $spatial) {
        if ($spatial['field'] === $this->real_field) {
          $spatials[$i]['distance'] = TRUE;
        }
      }
      $this->query->setOption('search_api_location', $spatials);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function get_value($values, $field = NULL) {
    if ($field || empty($this->options['show_distance_to_point'])) {
      // Return the normal field value.
      return parent::get_value($values, $field);
    }
    // Return the distance.
    $spatials = (array) $this->query->getOption('search_api_location', array());
    // If the spatial information aren't complete to have a distance skip
    // value fetching.
    foreach ($spatials as $spatial) {
      if ($spatial['field'] === $this->real_field) {
        $spatial_filter = $spatial;
        break;
      }
    }
    if (!isset($spatial_filter)) {
      return NULL;
    }

    $id = $values->_entity_properties['search_api_id'];
    $results = $this->query->getSearchApiResults();
    if (isset($results['search_api_location'][$this->real_field][$id]['distance'])) {
      $value = $results['search_api_location'][$this->real_field][$id]['distance'];
    }
    else {
      // If the server wasn't able to calculate the distance do it here.
      $value = NULL;
      $latlon = parent::get_value($values);
      if (is_string($latlon) && strpos($latlon, ',')) {
        $location = array_combine(array('lat', 'lon'), explode(',', $latlon, 2));
        $value = search_api_location_calculate_distance(
          $location,
          array(
            'lat' => $spatial_filter['lat'],
            'lon' => $spatial_filter['lon'],
          )
        );
      }
    }
    // Do the conversion.
    if (is_numeric($value)) {
      return $value / $this->options['conversion_factor'];
    }
    return $value;
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    // Render normally.
    if (empty($this->options['show_distance_to_point'])) {
      return parent::render($values);
    }
    // Render as distance.
    $value = $this->get_value($values);
    if (is_numeric($value)) {
      if (!empty($this->options['set_precision'])) {
        $value = number_format($value, $this->options['precision'], $this->options['decimal'], $this->options['separator']);
      }
      else {
        $remainder = abs($value) - intval(abs($value));
        $value = $value > 0 ? floor($value) : ceil($value);
        $value = number_format($value, 0, '', $this->options['separator']);
        if ($remainder) {
          $value .= $this->options['decimal'] . substr($remainder, 2);
        }
      }
      return $this->sanitize_value($value);
    }
    return '';
  }
}
