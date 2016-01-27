<?php

/**
 * @file
 * Fortissimmo pand "City" field handler
 */

class views_handler_field_city extends views_handler_field {
  function construct() {
    parent::construct();
    $cities = fortissimmo_get_property_cities();
    $this->city_mapping = $cities;
  }

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['address_visible'] = array('table' => $view->base_table, 'field' => 'address_visible');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['use_access_control'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Adds access control option
   */
  function options_form(&$form, &$form_state) {
    $form['use_access_control'] = array(
      '#title' => t("Don't show value if address is marked invisible"),
      '#type' => 'checkbox',
      '#default_value' => $this->options['use_access_control'],
    );
    parent::options_form($form, $form_state);
  }

  function render($values) {
    if ($this->options['use_access_control'] == TRUE) {
      if (isset($this->aliases['address_visible'])) {
        $address_visible = $values->{$this->aliases['address_visible']};
        if ($address_visible) {
          return $this->city_mapping[$values->{$this->field_alias}];
        }
      }
    }
    else {
      return $this->city_mapping[$values->{$this->field_alias}];
    }
  }
}