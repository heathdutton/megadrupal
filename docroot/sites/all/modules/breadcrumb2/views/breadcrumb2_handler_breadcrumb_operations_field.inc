<?php

/**
 * @file
 * This field handler aggregates operations that can be done on a breadcrumb
 * under a single field providing a more flexible way to present them in a view
 */
class breadcrumb2_handler_breadcrumb_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['bid'] = 'bid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('breadcrumb2', 'breadcrumb', array($this->get_value($values, 'bid')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
