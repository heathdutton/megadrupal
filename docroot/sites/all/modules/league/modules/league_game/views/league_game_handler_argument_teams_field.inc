<?php

/**
 * @file
 * Contains a Views field argument handler to allow filtering
 * by team A or B
 */

class league_game_handler_argument_teams_field extends views_handler_argument_numeric {
 function query($group_by = FALSE) {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    $placeholder = $this->placeholder();
    $null_check = empty($this->options['not']) ? '' : "OR $this->table_alias.team_a IS NULL";

    $and_or = empty($this->options['not']) ? 'OR' : 'AND';
    if (count($this->value) > 1) {
      $operator = empty($this->options['not']) ? 'IN' : 'NOT IN';
      $this->query->add_where_expression(0, "$this->table_alias.team_a $operator($placeholder) $and_or $this->table_alias.team_b $operator($placeholder) $null_check", array($placeholder => $this->value));
    }
    else {
      $operator = empty($this->options['not']) ? '=' : '!=';
      $this->query->add_where_expression(0, "$this->table_alias.team_a $operator $placeholder $and_or $this->table_alias.team_b $operator $placeholder $null_check", array($placeholder => $this->argument));
    }
  }
}
