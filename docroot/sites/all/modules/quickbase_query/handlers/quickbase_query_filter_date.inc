<?php
/**
 * @file
 *
 * Based heavily on views_handler_filter_string so we can take advantage of
 * its nice administrative UI and look like the usual Views string filter.
 */

class quickbase_query_filter_date extends views_handler_filter_string {
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->definition['allow empty'] = FALSE;
  }

  /**
   * Provide QuickBase text field predicates
   */
  function operators() {
    $op_base = array(
      'method' => 'op_add',
      'values' => 1,
    );
    $operators = array(
      'EX' => $op_base + array(
        'title' => t('Is equal to'),
        'short' => t('EX'),
      ),
      'XEX' => $op_base + array(
        'title' => t('Is not equal to'),
        'short' => t('XEX'),
      ),
      'BF' => $op_base + array(
        'title' => t('Is before'),
        'short' => t('BF'),
      ),
      'OBF' => $op_base + array(
        'title' => t('Is on or before'),
        'short' => t('OBF'),
      ),
      'AF' => $op_base + array(
        'title' => t('Is after'),
        'short' => t('AF'),
      ),
      'OAF' => $op_base + array(
        'title' => t('Is on or after'),
        'short' => t('OAF'),
      ),
    );

    return $operators;
  }

  function op_add($table, $field) {
    $this->query->add_where($this->options['group'], $field, strtotime($this->value) * 1000, $this->operator);
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
      $this->{$info[$this->operator]['method']}($this->table, $this->real_field);
    }
  }


  /**
   * Ensure the main table for this handler is in the query. This is used
   * a lot.
   */
  function ensure_my_table() {
  }
}
