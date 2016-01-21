<?php

/**
 * @file
 * Filter on province.
 */

// @codingStandardsIgnoreStart
class location_region_handler_filter_location_province extends views_handler_filter {

  public $location_country = FALSE;
  public $location_country_identifier = FALSE;

  public $location_region = FALSE;
  public $location_region_identifier = FALSE;

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['operator'] = array('default' => 'is');
    $options['type'] = array('default' => 'textfield');

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function admin_summary() {
    return '';
  }

  /**
   * {@inheritdoc}
   */
  public function has_extra_options() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function extra_options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array('select' => t('Dropdown'), 'textfield' => t('Autocomplete')),
      '#default_value' => $this->options['type'],
    );
  }

  /**
   * Provide a simple textfield for equality.
   */
  public function value_form(&$form, &$form_state) {
    $region = $this->grovel_region();
    if ($region) {
      $provinces = location_region_get_provinces($region);
    }
    else {
      $country = $this->grovel_country();
      $ac = $country;
      if (is_array($ac)) {
        $ac = implode(',', $ac);
      }

      $provinces = location_get_provinces($ac);
    }

    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('State/Province'),
      '#options' => $provinces,
      '#default_value' => -1,
      '#multiple' => isset($this->options['expose']['multiple']) ? $this->options['expose']['multiple'] : FALSE,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function operator_options() {
    if ($this->options['expose']['single']) {
      return array(
        'is' => t('Is'),
        'is not' => t('Is not'),
      );
    }
    else {
      return array(
        'is' => t('Is one of'),
        'is not' => t('Is not one of'),
      );
    }
  }

  /**
   * Grovel country.
   */
  public function grovel_country() {
    $country = variable_get('location_default_country', 'us');
    if (!empty($this->view->filter)) {
      foreach ($this->view->filter as $k => $v) {
        if ($v->table == 'location' && $v->options['relationship'] == $this->options['relationship']) {
          if ($v->field == 'country' || $v->field == 'distance') {
            $country = ($v->field == 'distance') ? $v->value['country'] : $v->value;
            if (!empty($v->options['expose']['identifier'])) {
              if (isset($this->view->exposed_input[$v->options['expose']['identifier']])) {
                $country = $this->view->exposed_input[$v->options['expose']['identifier']];
              }
              $this->location_country_identifier = $v->options['expose']['identifier'];
            }
          }
        }
      }
    }

    if ($country == '' || $country == 'All' || $country == '  ' || $country == 'xx') {
      // It's set to something nonsensical, reset to the default to prevent malfunctions.
      $country = variable_get('location_default_country', 'us');
    }
    $this->location_country = $country;

    return $country;
  }

  /**
   * Grovel Region.
   */
  public function grovel_region() {
    $region = '';
    if (!empty($this->view->filter)) {
      foreach ($this->view->filter as $k => $v) {
        if ($v->table == 'location' && $v->options['relationship'] == $this->options['relationship']) {
          if ($v->field == 'region') {

            $region = '';
            if (!empty($v->options['expose']['identifier'])) {
              if (isset($this->view->exposed_input[$v->options['expose']['identifier']])) {
                $region = $this->view->exposed_input[$v->options['expose']['identifier']];
              }
              $this->location_region_identifier = $v->options['expose']['identifier'];
            }
          }
        }
      }
    }

    if ($region == '' || $region == 'All' || $region == '  ' || $region == 'xx') {
      // It's set to something nonsensical, reset to the default to prevent malfunctions.
      $region = '';
    }
    $this->location_region = $region;

    return $region;
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // Normalize values.
    $value = $this->value;
    if (is_array($value)) {
      // At one point during development, provinces was a select box.
      // Right now it's an autocomplete textfield.
      // @@@ Investigate correct fix sometime.
      if (count($value) == 1) {
        // If multiple is allowed but only one was chosen, use a string instead.
        $value = reset($value);
      }
    }
    if (empty($value)) {
      return;
    }

    $country = $this->grovel_country();

    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    if (is_array($value)) {
      // Multiple values.
      foreach ($value as $k => $v) {
        // Convert to province codes.
        // $value[$k] = location_province_code($country, $v);
        $value[$k] = $v;
      }
      $operator = ($this->operator == 'is') ? 'IN' : 'NOT IN';
      $this->query->add_where($this->options['group'], $field, $value, $operator);
    }
    else {
      // Single value
      // Convert to province code.
      // $value = location_province_code($country, $value);

      $operator = ($this->operator == 'is') ? '=' : '!=';
      $this->query->add_where($this->options['group'], $field, $value, $operator);
    }
  }
}
// @codingStandardsIgnoreEnd
