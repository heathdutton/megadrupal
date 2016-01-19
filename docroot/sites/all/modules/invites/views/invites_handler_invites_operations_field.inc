<?php

/**
 * This field handler aggregates operations that can be done on a invites
 * under a single field providing a more flexible way to present them in a view
 */
class invites_handler_invites_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['invites_id'] = 'invites_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('invites', 'admin/content/invites/invites', array($this->get_value($values, 'invites_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
