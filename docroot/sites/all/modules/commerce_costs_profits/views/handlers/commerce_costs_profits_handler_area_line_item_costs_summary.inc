<?php

/**
 * @file
 * Contains commerce_costs_profits_handler_area_line_item_costs_summary class.
 */

/**
 * Defines a line item summary area handler to be plugged into the View itself.
 */
class commerce_costs_profits_handler_area_line_item_costs_summary extends views_handler_area {

  /**
   * Get a value used for rendering.
   *
   * @param object $values
   *   An object containing all retrieved values.
   *
   * @return string
   *   Line_item_id value.
   */
  protected function getValue($values) {
    $field_alias = '';

    foreach ($this->view->query->fields as $value) {
      if ($value['field'] == 'line_item_id') {
        $field_alias = $value['alias'];
        break;
      }
    }

    if (isset($values->{$field_alias})) {
      return $values->{$field_alias};
    }

    return '';
  }

  /**
   * Renders summary.
   *
   * @param bool $empty
   *   If there's nothing to render.
   *
   * @return string
   *   Rendered summary.
   */
  public function render($empty = FALSE) {
    if ($empty) {
      return '';
    }

    $line_item_ids = array();
    foreach ($this->view->result as $result) {
      $line_item_id = $this->getValue($result);
      if ($line_item_id) {
        $line_item_ids[] = $line_item_id;
      }
    }
    $line_items = commerce_line_item_load_multiple($line_item_ids);

    array_walk($line_items, function (&$line_item) {
      $line_item = entity_metadata_wrapper('commerce_line_item', $line_item);
    });

    $quantity = commerce_line_items_quantity($line_items);
    $currency_code = commerce_default_currency();
    $currency = commerce_currency_load($currency_code);

    $total_cost = array_reduce($line_items, function ($total_cost, $line_item) {
      return $total_cost + $line_item->quantity->value() * $line_item->field_cost->amount->value();
    });

    $variables = array(
      'view' => $this->view,
      'links' => '',
      'quantity_raw' => $quantity,
      'quantity_label' => format_plural($quantity, 'item', 'items', array(), array('context' => 'product count on a Commerce order')),
      'quantity' => format_plural($quantity, '1 item', '@count items', array(), array('context' => 'product count on a Commerce order')),
      'total_raw' => number_format(commerce_currency_round($total_cost, $currency), $currency['decimals']),
      'total_label' => t('Cost') . ':',
      'total' => commerce_currency_format($total_cost, $currency_code, $this->view),
    );

    return theme('commerce_line_item_summary', $variables);
  }

}
