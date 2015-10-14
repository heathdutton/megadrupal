<?php

class views_optional_argument_numeric extends views_handler_argument_numeric {

  function option_definition() {
    $options = parent::option_definition();

    $options['pass_null'] = array('default' => FALSE, 'bool' => TRUE);
    $options['pass_null_for_argument'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['pass_null'] = array(
      '#type' => 'checkbox',
      '#title' => t('Pass NULL values'),
      '#description' => t('If selected, the filter will pass NULL values even if "Exclude" option is not set.'),
      '#default_value' => !empty($this->options['pass_null']),
      '#fieldset' => 'more',
    );

    $form['pass_null_for_argument'] = array(
      '#type' => 'textfield',
      '#title' => t('Pass NULL values only for this argument'),
      '#default_value' => isset($this->options['pass_null_for_argument']) ? $this->options['pass_null_for_argument'] : '',
      '#description' => t('If set, the filter will pass NULLs only if argument equals this value.'),
      '#fieldset' => 'more',
    );
  }

  function query($group_by = FALSE) {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    $null_check = '';
    if (!empty($this->options['not']) || !empty($this->options['pass_null'])) {
      $null_check = "OR $this->table_alias.$this->real_field IS NULL";
      if (isset($this->options['pass_null_for_argument']) && (strlen($this->options['pass_null_for_argument']) > 0)) {
        if (strcmp($this->argument, $this->options['pass_null_for_argument']) !== 0) {
          $null_check = '';
        }
      }
    }

    $placeholder = $this->placeholder();

    if (count($this->value) > 1) {
      $operator = empty($this->options['not']) ? 'IN' : 'NOT IN';
      $this->query->add_where_expression(0, "$this->table_alias.$this->real_field $operator($placeholder) $null_check", array($placeholder => $this->value));
    }
    else {
      $operator = empty($this->options['not']) ? '=' : '!=';
      $this->query->add_where_expression(0, "$this->table_alias.$this->real_field $operator $placeholder $null_check", array($placeholder => $this->argument));
    }
  }

}
