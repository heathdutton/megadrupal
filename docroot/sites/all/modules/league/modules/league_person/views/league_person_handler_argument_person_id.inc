<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_person_handler_argument_person_id extends views_handler_argument_numeric {
/**
* Override the behavior of title(). Get the title of the node.
*/
  function title_query() {
    $titles = array();
    $result = db_query("SELECT p.name FROM {league_person} p WHERE p.league_person_id IN (:pids)", array(':pids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->name);
    }
    return $titles;
  }
} 