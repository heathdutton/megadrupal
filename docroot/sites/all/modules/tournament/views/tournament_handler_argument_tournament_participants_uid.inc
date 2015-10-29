<?php

/**
 * Argument handler for entities of a given user.
 *
 * @ingroup views_filter_handlers
 */

class tournament_handler_argument_tournament_participants_uid extends views_handler_argument_numeric {
  function query() {
    $this->ensure_my_table();
    $tournament_alias = $this->query->ensure_table('tournament');
    $entity_type = $tournament_alias . '.entity_type';

    $field = $this->table_alias . '.' . $this->real_field . ' ';

    // Set up our conditions
    $or = db_or();
    $and1 = db_and();
    $and2 = db_and();

    // Load users teams
    $users_teams = array();

    $uid = $this->argument;
    $and1->condition($entity_type, 'user');
    $and2->condition($entity_type, 'team');

    $teams = team_load_user_teams(array($uid));
    if (!empty($teams[$uid])) {
      $users_teams = $teams[$uid];
    }
    
    $and1->condition($field, $uid);
    $and2->condition($field, $users_teams, 'IN');      

    $or->condition($and1);
    $or->condition($and2);

    $this->query->add_where(0, $or);
  }
}
