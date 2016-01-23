<?php

/**
 * This field handler aggregates operations that can be done on a things_ids
 * under a single field providing a more flexible way to present them in a view
 */
class things_ids_handler_things_ids_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['things_ids_id'] = 'things_ids_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('things_ids', 'admin/content/things_ids/things_ids', array($this->get_value($values, 'things_ids_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
