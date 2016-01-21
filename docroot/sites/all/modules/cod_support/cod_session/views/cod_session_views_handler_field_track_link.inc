<?php

/**
 * @file
 * Definition of cod_sponsors_commerce_views_handler_field_order_link.
 */

/**
 * Field handler to present a link to the order.
 *
 * @ingroup views_field_handlers
 */
class cod_session_views_handler_field_track_link extends views_handler_field {
  function construct() {
    parent::construct();
  }

  function option_definition() {
    return parent::option_definition();
  }

  function access() {
    return TRUE;
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /*
   * This render array is specifically designed to work with cod_sessions_tracks
   * View. Eventually it'd be good to not hard code the arguments and get them
   * from context.
   */
  function render($values) {
    if ($values === TRUE) {
      return;
    }

    // Generate as text.
    //$this->options['alter']['make_link'] = TRUE;

    // Get the host (event) entity.
    $attributes = array();
    // Get the current taxonomy term, if its set
    $path = current_path();
    if (isset($this->view->args[2]) && !empty($values)) {
      $path = substr(current_path(), 0, strrpos(current_path(), '/'));
      if ($this->view->args[2] == str_replace('_', '-', $values->taxonomy_term_data_machine_name)) {
        $attributes = array('attributes' => array('class' => array('clicked')));
        $active = TRUE;
      }
    }
     // Making a global more link requires some changes
    if ($this->real_field == 'sessions_more_link' && $values === FALSE) {
      $values = new stdClass();
      $values->taxonomy_term_data_machine_name = isset($this->view->args[2]) ? $this->view->args[2] : '';
      $values->taxonomy_term_data_name = isset($this->view->argument['tid']->validated_title) ? $this->view->argument['tid']->validated_title : '';
    }
    if (!isset($active)) {
      $path = $path . '/' . str_replace('_', '-', $values->taxonomy_term_data_machine_name);
    }

    return l($values->taxonomy_term_data_name, $path, $attributes);
  }
}
