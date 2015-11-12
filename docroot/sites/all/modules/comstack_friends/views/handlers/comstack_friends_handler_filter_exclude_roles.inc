<?php

/**
 * @file
 * Contains comstack_friends_handler_filter_exclude_roles
 */

class comstack_friends_handler_filter_exclude_roles extends views_handler_filter_many_to_one {
  var $helper = NULL;

  function init(&$view, &$options) {
    parent::init($view, $options);

    $all_roles = user_roles(TRUE);
    $excluded_roles = array_filter(variable_get('comstack_friends_exclude_roles', array()));
    $roles = array();

    if (!empty($excluded_roles)) {
      foreach ($all_roles as $rid => $name) {
        if (in_array($name, $excluded_roles)) {
          $roles[] = $rid;
        }
      }
    }

    $this->value = $roles;
    $this->helper = new views_many_to_one_helper($this);
  }

  function operators() {
    $operators = array(
      'not' => array(
        'title' => t('Is none of'),
        'short' => t('not'),
        'short_single' => t('<>'),
        'method' => 'op_helper',
        'values' => 1,
        'ensure_my_table' => 'helper',
      ),
    );

    return $operators;
  }

  function get_value_options() {
    $this->value_options = array();
  }

  function can_expose() {
    return FALSE;
  }

  function admin_summary() {
    return '';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['operator']['default'] = 'not';
    $options['value']['default'] = array();

    if (isset($this->helper)) {
      $this->helper->option_definition($options);
    }
    else {
      $helper = new views_many_to_one_helper($this);
      $helper->option_definition($options);
    }

    return $options;
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    // Hide the fields.
    $form['operator']['#type'] = 'value';
    $form['value']['#type'] = 'value';
  }

  function validate() {
  }

  function query() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", $this->value, 'IS NULL');
  }
}
