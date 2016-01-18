<?php
/**
 * Filter by category
 */
class userpoints_ranking_handler_filter_category extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Category');
      $this->value_options = userpoints_get_categories(NULL);
      $this->value_options['total'] = t('Total');
    }
  }
  
  function op_simple() {
    if (empty($this->value)) {
      return;
    }
    
    $table = $this->ensure_my_table();
    if (isset($this->value['total'])) {
      unset($this->value['total']);
      $args = array();
      
      if (strtoupper($this->operator) == 'IN') {
	    $snipped = "$this->table_alias.$this->real_field IS NULL";
	  }
	  elseif(strtoupper($this->operator) == 'NOT IN') {
	    $snipped = "$this->table_alias.$this->real_field IS NOT NULL";
	  }
	  else {
	    // Return we should not anything, because we cannot 
	    // expect what it is.
	  	return;
	  }
	  
      if (count($this->value) > 0) {
	    $snipped .= ' OR ' . $this->table_alias.'.'.$this->real_field;
      	if (strtoupper($this->operator) == 'IN') {
	      $snipped .= ' IN (';
	    }
	    elseif(strtoupper($this->operator) == 'NOT IN') {
	      $snipped .= ' NOT IN (';
	    }
	    else {
	      // Return we should not anything, because we cannot 
	      // expect what it is.
	      return;
	    }
	    foreach ($this->value as $value) {
	      $placeholder = $this->query->placeholder($table.'_in');
	      $snipped .= $placeholder . ', ';
	      $args[$placeholder] = $value;
	    }
	    $snipped = substr($snipped, 0, -2);
	    $snipped .= ')';
	    
      }
      
      $this->view->query->add_where_expression($this->options['group'], $snipped, $args);
    }
    else {
      // We use array_values() because the checkboxes keep keys and that can cause
      // array addition problems.
      $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", array_values($this->value), $this->operator); 
    }
    
  }  
  
}