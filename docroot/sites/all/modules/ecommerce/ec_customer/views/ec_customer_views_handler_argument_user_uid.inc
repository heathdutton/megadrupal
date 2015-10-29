<?php

/**
 * @file
 * Provide ability to use an user id as an argument.
 */

class ec_customer_views_handler_argument_user_uid extends views_handler_argument_user_uid {
  function query() {
    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }
    $ecids = array();

    foreach ($this->value as $uid) {
      if (($ecid = ec_customer_get_by_uid($uid)) && isset($ecid->ecid)) {
        $ecids[] = $ecid->ecid;
      }
    }

    $ecids = array_filter($ecids);
    if (empty($ecids)) {
      $ecids[] = 0;
    }
    $operator = isset($this->operator) && $this->operator == 'or' ? ',' : '+';
    $this->argument = implode($operator, $ecids);

    parent::query();
  }
}
