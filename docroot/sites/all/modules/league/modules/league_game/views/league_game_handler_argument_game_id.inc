<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a node id.
*/
class league_game_handler_argument_game_id extends views_handler_argument_numeric {
/**
* Override the behavior of title(). Get the title of the node.
*/
  function title_query() {
    $titles = array();
    $result = db_query("SELECT g.name FROM {league_game} g WHERE g.league_game_id IN (:gids)", array(':gids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->name);
    }
    return $titles;
  }
}