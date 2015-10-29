<?php

/**
 * Filter handler for teams of the current user.
 *
 * @ingroup views_filter_handlers
 */
class team_handler_filter_teams_current extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is a team of the logged in user');
  }

  function query() {
    $this->ensure_my_table();

    $field = $this->table_alias . '.' . $this->real_field . ' ';
    $or = db_or();

    // Load users teams
    $users_teams = array();

    global $user;
    if ($user->uid) {
      $teams = team_load_user_teams(array($user->uid));
      if (!empty($teams[$user->uid])) {
        $users_teams = $teams[$user->uid];
      }
    }

    if (empty($this->value)) {
      $or->condition($field, $users_teams, 'NOT IN');
      if ($this->accept_null) {
        $or->isNull($field);
      }
    }
    else {
      $or->condition($field, $users_teams, 'IN');
    }
    $this->query->add_where($this->options['group'], $or);
  }
}
