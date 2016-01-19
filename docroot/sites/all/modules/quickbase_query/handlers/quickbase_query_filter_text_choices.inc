<?php
/**
 * @file
 *
 * Handles text fields with a list of selectable values
 */

class quickbase_query_filter_text_choices extends views_handler_filter_in_operator {
  /**
   * Override parent class method
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    if (!empty($this->definition['qbfield']['choices'])) {
      $this->value_options = drupal_map_assoc($this->definition['qbfield']['choices']);
    }

    return $this->value_options;
  }

  /**
   * This kind of construct makes it relatively easy for a child class
   * to add or remove functionality by overriding this function and
   * adding/removing items from this array.
   */
  function operators() {
    $operators = array(
      'EX' => array(
        'title' => t('Is one of'),
        'short' => t('in'),
        'short_single' => t('EX'),
        'method' => 'op_simple',
        'values' => 1,
      ),
      'XEX' => array(
        'title' => t('Is not one of'),
        'short' => t('not in'),
        'short_single' => t('XEX'),
        'method' => 'op_simple',
        'values' => 1,
      ),
    );

    return $operators;
  }

  function op_simple() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->add_where($this->options['group'], $this->real_field, array_values($this->value), $this->operator);
  }

  /**
   * Ensure the main table for this handler is in the query. This is used
   * a lot.
   */
  function ensure_my_table() {
  }
}
