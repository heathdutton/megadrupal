<?php

/**
 * This field handler aggregates operations that can be done on a league_game
 * under a single field providing a more flexible way to present them in a view
 */
class league_game_handler_league_game_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['league_game_id'] = 'league_game_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('league_game', 'league_game', array($this->get_value($values, 'league_game_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
