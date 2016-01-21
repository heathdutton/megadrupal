<?php

/**
 * @file
 * Definition of views_handler_argument_commune_user_uid.
 */

/**
 * Argument handler to accept a user id to check for nodes that
 * user was referred to directly or via @mention.
 *
 * @ingroup views_argument_handlers
 */
class views_handler_argument_commune_user_uid extends views_handler_argument {
  function title() {
    if (!$this->argument) {
      $title = variable_get('anonymous', t('Anonymous'));
    }
    else {
      $title = db_query('SELECT u.name FROM {users} u WHERE u.uid = :uid', array(':uid' => $this->argument))->fetchField();
    }
    if (empty($title)) {
      return t('No user');
    }

    return check_plain($title);
  }

  function default_actions($which = NULL) {
    // Disallow summary views on this argument.
    if (!$which) {
      $actions = parent::default_actions();
      unset($actions['summary asc']);
      unset($actions['summary desc']);
      return $actions;
    }

    if ($which != 'summary asc' && $which != 'summary desc') {
      return parent::default_actions($which);
    }
  }

  function query($group_by = FALSE) {
    $this->ensure_my_table();

    $subselect = db_select('commune_context', 'c');
    $subselect->addField('c', 'context_id');
    $subselect->addField('c', 'type');
    $subselect->condition('c.context_id', $this->argument);
    $subselect->where("c.pid = $this->table_alias.nid");

    $condition = db_or()
      ->condition("$this->table_alias.uid", $this->argument, '=')
      ->exists($subselect);

    $this->query->add_where(0, $condition);
  }

  function get_sort_name() {
    return t('Numerical', array(), array('context' => 'Sort order'));
  }
}
