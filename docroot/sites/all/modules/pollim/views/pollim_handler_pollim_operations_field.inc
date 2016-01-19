<?php

/**
 * This field handler aggregates operations that can be done on a pollim
 * under a single field providing a more flexible way to present them in a view
 */
class pollim_handler_pollim_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['pollim_id'] = 'pollim_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('pollim', 'admin/content/pollims/pollim', array($this->get_value($values, 'pollim_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
