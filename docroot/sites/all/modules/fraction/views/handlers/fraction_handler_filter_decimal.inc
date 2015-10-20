<?php

/**
 * @file
 * Contains the Fraction Views filter handler.
 */

/**
 * Filter handler for Fraction fields.
 */
class fraction_handler_filter_decimal extends views_handler_filter_numeric {

  // Add formula to the query.
  function query() {

    // Ensure the main table for this field is included.
    $this->ensure_my_table();

    // Formula for calculating the final value, by dividing numerator by denominator.
    // These are added as additional fields in hook_field_views_data_alter().
    $formula = $this->table_alias . '.' . $this->definition['additional fields'][0] . ' / ' . $this->table_alias . '.' . $this->definition['additional fields'][1];

    // Perform the filter using the selected operator and the formula.
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($formula);
    }
  }

  // Override operator functions (op_between, op_simple, and op_regex).
  // Alter the add_where method call to use the formula.
  // Note that op_empty() is not included because we are not setting 'allow empty'.
  function op_between($field) {
    if ($this->operator == 'between') {
      $expression = $field . ' BETWEEN :min AND :max';
      $this->query->add_where_expression($this->options['group'], $expression, array(':min' => $this->value['min'], ':max' => $this->value['max']));
    }
    else {
      $expression = $field . ' <= :min OR ' . $field . ' >= :max';
      $this->query->add_where_expression($this->options['group'], $expression, array(':min' => $this->value['min'], ':max' => $this->value['max']));
    }
  }

  function op_simple($field) {
    $expression = $field . ' ' . $this->operator . ' :value';
    $this->query->add_where_expression($this->options['group'], $expression, array(':value' => $this->value['value']));
  }

  function op_regex($field) {
    $expression = $field . ' RLIKE :value';
    $this->query->add_where_expression($this->options['group'], $expression, array(':value' => $this->value['value']));
  }
}

