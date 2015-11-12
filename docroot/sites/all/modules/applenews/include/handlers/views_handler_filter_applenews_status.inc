<?php

/**
 * @file
 * Definition of views_handler_filter_applenews_status.
 */

/**
 * Apple News entity publish status.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_applenews_status extends views_handler_filter_boolean_operator {

  function construct() {
    parent::construct();
    $this->value_value = t('Apple News Publish Status');
  }

  /**
   * Return the possible options for this filter.
   */
  function get_value_options() {
    parent::get_value_options();
    $this->value_options = [
      1 => t('Published'),
      0 => t('Not published')
    ];
  }

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    if (empty($this->value)) {
      $this->query->add_where($this->options['group'], $field, NULL, 'IS NULL');
    }
    else {
      $this->query->add_where($this->options['group'], $field, NULL, 'IS NOT NULL');
    }
  }

}
