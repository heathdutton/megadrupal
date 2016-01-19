<?php
/**
 * @file
 * Views handler for the registration available filter.
 */
class views_handler_filter_field_registration_available extends views_handler_filter_numeric {
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

    $join = new views_join();
    $join->construct('registration', $this->table_alias, 'nid', 'nid', array(), 'LEFT');
    $alias = $this->query->ensure_table('registration_count', $this->relationship, $join);

    $field = "(IF(SUM(IFNULL($this->table_alias.capacity, 0)) = 0, 9999, SUM($this->table_alias.capacity)) - IFNULL(SUM($alias.count), 0))";
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
    $this->query->add_having_expression($this->options['group'], $snippet, $placeholders);
  }

}
