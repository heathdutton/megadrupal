<?php
/**
 * Simple filter to filter out the last period of a term
 */
class userpoints_ranking_handler_filter_last_period extends views_handler_filter_boolean_operator {
  function init(&$view, $options) {
    parent::init($view, $options);
      if (!isset($this->value)) {
        $this->value = 'last';
      }
      
      $this->value_value = t('Limit to:');
      $this->value_options = array(
      'last' => t('Last Period'),
      'not_last' => t('Not Last Period'),
      );    
  }

  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
    if (empty($this->value_options)) {
      $this->get_value_options();
    }
    if (isset($this->value_options[$this->value])) {
      return $this->value_options[$this->value];
    }
    else {
      return t('Not Last Period');
    }
  }
    
  /**
   * Provide simple equality operator
   */
  function operator_options() {
    return false;
  }  
    
  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    
    if ($this->value == 'not_last') {
      $operator = 'NOT IN';
    }
    else {
      $operator = 'IN';
    }
    
    $periods = array_keys(userpoints_ranking_get_last_periods());
    
    $this->query->add_where($this->options['group'], $field, array_values($periods), $operator); 
  }
  
}
