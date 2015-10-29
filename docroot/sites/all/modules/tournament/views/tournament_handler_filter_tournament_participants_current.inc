<?php

/**
 * Filter handler for entities of the current user.
 *
 * @ingroup views_filter_handlers
 */

class tournament_handler_filter_tournament_participants_current extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('The logged in user is part of a participant');
  }

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

    global $user;
    if ($user->uid) {
      $and1->condition($entity_type, 'user');
      $and2->condition($entity_type, 'team');

      $teams = team_load_user_teams(array($user->uid));
      if (!empty($teams[$user->uid])) {
        $users_teams = $teams[$user->uid];
      }
    }

    if (empty($this->value)) {
      $and1->condition($field, $user->uid, '<>');
      $and2->condition($field, $users_teams, 'NOT IN');
    }
    else {
      $and1->condition($field, $user->uid);
      $and2->condition($field, $users_teams, 'IN');
    }

    $or->condition($and1);
    $or->condition($and2);

    $this->query->add_where($this->options['group'], $or);
  }
}
