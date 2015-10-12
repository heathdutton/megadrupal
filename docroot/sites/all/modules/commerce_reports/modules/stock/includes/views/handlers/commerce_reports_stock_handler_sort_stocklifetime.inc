<?php

/**
 * @file
 * Views handler to sort by stock lifetime.
 */

/**
 * Class commerce_reports_stock_handler_sort_stocklifetime.
 */
class commerce_reports_stock_handler_sort_stocklifetime extends views_handler_sort {
  /**
   * @inheritdoc
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * @inheritdoc
   */
  function post_execute(&$values) {
    if (!isset($this->view->commerce_reports_stock_data)) {
      return FALSE;
    }

    // Chuck all the rows into our already sorted array.
    foreach ($this->view->result as $row) {
      $this->view->commerce_reports_stock_data[$row->commerce_product_sku]['view_row'] = $row;
    }

    // Read the rows out of our sorted array.
    // There's probably a better way to do this but it's getting late.
    $result = array();
    foreach ($this->view->commerce_reports_stock_data as $row) {
      if (isset($row['view_row'])) {
        $result[] = $row['view_row'];
      }
    }

    $this->view->result = $result;
  }

}
