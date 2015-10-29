<?php

/**
 * @file
 * Argument handler to accept a user id to check for nodes that
 * user posted.
 */
class views_handler_argument_wishlist_giftee_user_uid extends views_handler_argument {

  function title() {
    if (!$this->argument) {
      $title = variable_get('anonymous', t('Anonymous'));
    }
    else {
      $title = db_query('SELECT u.name FROM {users} u WHERE u.uid = :u.uid', array(':u.uid' => $this->argument))->fetchField();
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
}
