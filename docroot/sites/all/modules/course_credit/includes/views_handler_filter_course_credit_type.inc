<?php

class views_handler_filter_course_credit_type extends views_handler_filter_in_operator {

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Credit type');
      $types = course_credit_get_credit_types();
      foreach ($types as $type => $info) {
        $options[$type] = t($info->title);
      }
      $this->value_options = $options;
    }
  }

  function option_definition() {
    $definition = parent::option_definition();
    $definition['course_credit_active']['default'] = 1;
    return $definition;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if ($this->table == 'course_credit') {
      $form['course_credit_active'] = array(
        '#title' => t('Active credit'),
        '#type' => 'checkbox',
        '#description' => t('When this filter is used, also filter inactive credit.'),
        '#default_value' => $this->options['course_credit_active'],
      );
    }
  }

  function query() {
    parent::query();
    if ($this->table == 'course_credit') {
      // If we are filtering on the course credit table, only show active
      // credit.
      if ($this->options['course_credit_active']) {
        $this->query->add_where($this->options['group'], "{$this->table_alias}.active", 1);
      }
    }
  }

}
