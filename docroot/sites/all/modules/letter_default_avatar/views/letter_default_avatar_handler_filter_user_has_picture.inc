<?php

/**
 * @file
 * Definition of views_handler_filter_boolean_operator.
 */

/**
 * Simple filter to handle matching of boolean values
 *
 * Definition items:
 * - label: (REQUIRED) The label for the checkbox.
 * - type: For basic 'true false' types, an item can specify the following:
 *    - true-false: True/false (this is the default)
 *    - yes-no: Yes/No
 *    - on-off: On/Off
 *    - enabled-disabled: Enabled/Disabled
 * - accept null: Treat a NULL value as false.
 * - use equal: If you use this flag the query will use = 1 instead of <> 0.
 *   This might be helpful for performance reasons.
 *
 * @ingroup views_filter_handlers
 */
class letter_default_avatar_handler_filter_user_has_picture extends views_handler_filter_boolean_operator {
  // exposed filter options
  var $always_multiple = TRUE;
  // Don't display empty space where the operator would be.
  var $no_operator = TRUE;
  // Whether to accept NULL as a false value or not
  var $accept_null = FALSE;

  function construct() {
    $this->real_field = 'default_avatar';
    parent::construct();
  }

  // We need to reverse the query as default_avatar is stored as TRUE in the DB
  // whereas the filter checks for user has a picture as TRUE, reverse it.
  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    if (empty($this->value)) {
      if ($this->accept_null) {
        $or = db_or()->condition($field, 0, '=')->condition($field, NULL, 'IS NULL');
        $this->query->add_where($this->options['group'], $or);
      }
      else {
        $this->query->add_where($this->options['group'], $field, 1, '=');
      }
    }
    else {
      if (!empty($this->definition['use equal'])) {
        $this->query->add_where($this->options['group'], $field, 0, '=');
      }
      else {
        $this->query->add_where($this->options['group'], $field, 1, '<>');
      }
    }
  }
}
