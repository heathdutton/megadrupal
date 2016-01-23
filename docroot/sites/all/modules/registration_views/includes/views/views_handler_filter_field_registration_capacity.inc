<?php
/**
 * @file
 * Views handler for the registration capacity filter.
 */
class views_handler_filter_field_registration_capacity extends views_handler_filter_numeric {
  function operators() {
    $operators = array(
      '<' => array(
        'title' => t('Is less than'),
        'method' => 'op_simple',
        'short' => t('<'),
        'values' => 1,
      ),
      '<=' => array(
        'title' => t('Is less than or equal to'),
        'method' => 'op_simple',
        'short' => t('<='),
        'values' => 1,
      ),
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
      '>=' => array(
        'title' => t('Is greater than or equal to'),
        'method' => 'op_simple',
        'short' => t('>='),
        'values' => 1,
      ),
      '>' => array(
        'title' => t('Is greater than'),
        'method' => 'op_simple',
        'short' => t('>'),
        'values' => 1,
      ),
    );

    return $operators;
  }

  function query() {
    $this->ensure_my_table();

    $field = "IF($this->table_alias.capacity = 0, 9999, $this->table_alias.capacity)";
    $operator = $this->operator;
    $placeholder = $this->placeholder();

    $snippet = "$field $operator $placeholder";
    $placeholders = array(
      $placeholder => $this->value['value'],
    );

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($snippet, $placeholders);
    }
  }

  function op_simple($snippet, $placeholders) {
    $this->query->add_where_expression($this->options['group'], $snippet, $placeholders);
  }

}
