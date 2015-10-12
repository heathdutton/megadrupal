<?php

/**
 * @file
 * Contains the Fraction Views sort handler.
 */

/**
 * Sort handler for Fraction fields.
 */
class fraction_handler_sort_decimal extends views_handler_sort {

  // Add formula to the query.
  function query() {

    // Ensure the main table for this field is included.
    $this->ensure_my_table();

    // Formula for calculating the final value, by dividing numerator by denominator.
    // These are added as additional fields in hook_field_views_data_alter().
    $formula = $this->table_alias . '.' . $this->definition['additional fields'][0] . ' / ' . $this->table_alias . '.' . $this->definition['additional fields'][1];

    // Add the orderby.
    $this->query->add_orderby(NULL, $formula, $this->options['order'], $this->table_alias . '_decimal');
  }
}

