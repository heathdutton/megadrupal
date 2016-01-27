<?php

/**
 * @file
 * Definition of views_handler_filter_history_user_timestamp.
 */

/**
 * @file
 * Definition of views_handler_filter_equality.
 */

/**
 * Simple filter to handle equal to / not equal to filters
 *
 * @ingroup views_filter_handlers
 */
class cloudwords_handler_filter_project_user extends views_handler_filter {
  // exposed filter options
  var $always_multiple = TRUE;

  /**
   * Provide simple equality operator
   */
  function operator_options() {
    return array(
      '=' => t('The current user'),
      // '!=' => t('Is not equal to'),
    );
  }

  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Value'),
      '#size' => 30,
      '#options' => array('yes' => t('Yes'), 'no' => t('No')),
      '#default_value' => $this->value,
      '#multiple' => FALSE,
    );
  }

  function query() {
    global $user;
    // This can only work if we're logged in.
    if (!$user || !$user->uid) {
      return;
    }

    // Don't filter if we're exposed and the checkbox isn't selected.
    if ((!empty($this->options['exposed'])) && empty($this->value)) {
      return;
    }

    $uid = $user->uid;

    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    $translatable = $this->query->ensure_table('cloudwords_translatable', $this->relationship);

    if ($this->value == 'yes') {
      $op = '=';
    }
    if ($this->value == 'no') {
      $op = '!=';
    }

    // NULL means a history record doesn't exist. That's clearly new content.
    // Unless it's very very old content. Everything in the query is already
    // type safe cause none of it is coming from outside here.
    $this->query->add_where_expression($this->options['group'], "$translatable.uid $op $uid");
  }
}
