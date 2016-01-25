<?php
/**
 * @file
 * Based heavily on views_handler_filter_string so we can take advantage of
 * its nice administrative UI and look like the usual Views string filter.
 */

class socrata_views_handler_filter_text extends views_handler_filter_string {
  /**
   * This kind of construct makes it relatively easy for a child class
   * to add or remove functionality by overriding this function and
   * adding/removing items from this array.
   */
  function operators() {
    $operators = array(
      '=' => array(
        'title' => t('Is equal to'),
        'method' => 'op_simple',
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'method' => 'op_simple',
        'short' => t('!='),
        'values' => 1,
      ),
/*
      'contains' => array(
        'title' => t('Contains'),
        'short' => t('contains'),
        'method' => 'op_comp_function',
        'values' => 1,
      ),
      'not contains' => array(
        'title' => t('Does not contain'),
        'short' => t('!contains'),
        'method' => 'op_comp_function',
        'values' => 1,
      ),
*/
      'starts_with' => array(
        'title' => t('Starts with'),
        'short' => t('starts_with'),
        'method' => 'op_comp_function',
        'values' => 1,
      ),
/*
      'not starts_with' => array(
        'title' => t('Does not start with'),
        'short' => t('!starts_with'),
        'method' => 'op_comp_function',
        'values' => 1,
      ),
*/
      'is null' => array(
        'title' => t('Is empty (NULL)'),
        'method' => 'op_comp_null',
        'short' => t('empty'),
        'values' => 0,
      ),
      'is not null' => array(
        'title' => t('Is not empty (NOT NULL)'),
        'method' => 'op_comp_null',
        'short' => t('not empty'),
        'values' => 0,
      ),
    );

    return $operators;
  }

  /**
   * Add this filter to the query.
   *
   * Due to the nature of fapi, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator
   * and $this->value respectively.
   */
  function query() {
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($this->real_field);
    }
  }

  // Provide this method so we can wrap the value in single quotes for SoSQL where needed.
  function op_simple($field) {
    $this->query->add_where($this->options['group'], $field, "'{$this->value}'", $this->operator);
  }

  function op_comp_function($field) {
    $this->query->add_where_expression($this->options['group'], "{$this->operator}({$field}, '{$this->value}')");
  }

  function op_comp_null($field) {
    $this->query->add_where_expression($this->options['group'], "{$field} {$this->operator}");
  }
}
