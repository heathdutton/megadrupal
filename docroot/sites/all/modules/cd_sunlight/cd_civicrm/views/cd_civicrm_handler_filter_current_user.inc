<?php

/**
 * @file
 * Views handlers for cd_civicrm.
 *
 * @todo Change the block/page title appropriately.
 */

/**
 * Filter handler for the current user's State.
 *
 * @ingroup views_field_handlers
 */
class cd_civicrm_handler_filter_state_current_user extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t("Is the logged in user's state");
  }

  function query() {
    $this->ensure_my_table();

    $field = $this->table_alias . '.' . $this->real_field;
    $or = db_or();

    if (empty($this->value)) {
      $or->condition($field, '***CURRENT_USERS_STATE***', '<>');
      if ($this->accept_null) {
        $or->isNull($field);
      }
    }
    else {
      $or->condition($field, '***CURRENT_USERS_STATE***', '=');
    }
    $this->query->add_where($this->options['group'], $or);
  }

}

/**
 * Filter handler for the current user's District.
 *
 * @ingroup views_field_handlers
 */
class cd_civicrm_handler_filter_district_current_user extends views_handler_filter {

  function value_form(&$form, &$form_state) { }

  function can_expose() {
    return FALSE;
  }

  function query() {
    $this->ensure_my_table();

    // Remember that senators have a CD of 0.
    $or = db_or();
    $or->condition($this->table_alias . '.' . $this->real_field, array('***CURRENT_USERS_DISTRICT***', 0), 'IN');

    $this->query->add_where($this->options['group'], $or);
  }
}
