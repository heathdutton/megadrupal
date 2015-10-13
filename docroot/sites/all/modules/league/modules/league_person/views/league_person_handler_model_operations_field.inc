<?php

/**
 * This field handler aggregates operations that can be done on a league_person
 * under a single field providing a more flexible way to present them in a view
 */
class league_person_handler_league_person_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['league_person_id'] = 'league_person_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('league_person', 'league_person', array($this->get_value($values, 'league_person_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
