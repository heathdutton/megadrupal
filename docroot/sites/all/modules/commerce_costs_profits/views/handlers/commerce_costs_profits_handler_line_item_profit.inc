<?php

/**
 * @file
 * Contains commerce_costs_profits_handler_line_item_profit class.
 */

/**
 * Defines a line item handler to be plugged into the View itself.
 */
class commerce_costs_profits_handler_line_item_profit extends views_handler_field {

  /**
   * Constructor.
   */
  public function construct() {
    parent::construct();

    $this->real_field = 'line_item_id';
    $this->additional_fields['commerce_total_amount'] = array(
      'table' => 'field_data_commerce_total',
      'field' => 'commerce_total_amount',
    );
    $this->additional_fields['field_cost_amount'] = array(
      'table' => 'field_data_field_cost',
      'field' => 'field_cost_amount',
    );
  }

  /**
   * This method contains a bit of a hack to get arythmetics working in a query.
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();

    $group_params = array();
    if ($this->options['group_type'] != 'group') {
      $group_params = array(
        'function' => $this->options['group_type'],
      );
    }
    $this->field_alias = $this->view->query->add_field(NULL, "(field_data_commerce_total.commerce_total_amount - commerce_line_item.quantity * field_data_field_cost.field_cost_amount)", 'profit', $group_params);
  }

  /**
   * Renders profit.
   *
   * @param object $values
   *    Values to be rendered.
   *
   * @return string
   *    Rendered line item profit
   */
  public function render($values) {
    $currency_code = $this->get_currency_code($values);
    $profit = $this->get_value($values);

    return commerce_currency_format($profit, $currency_code);
  }

  /**
   * Finds currency code (using commerce_total field settings).
   *
   * @param object $values
   *    Query result values.
   *
   * @return string
   *    Currency code
   */
  protected function get_currency_code($values) {
    $entity = $values->_field_data['line_item_id']['entity'];
    $entity_type = $values->_field_data['line_item_id']['entity_type'];
    $langcode = field_language($entity_type, $entity, 'commerce_total');

    $delta = key($entity->commerce_total[$langcode]);

    if (!empty($entity->commerce_total[$langcode][$delta]['currency_code'])) {
      $currency_code = $entity->commerce_total[$langcode][$delta]['currency_code'];
    }
    else {
      $currency_code = commerce_default_currency();
    }

    return $currency_code;
  }

}
