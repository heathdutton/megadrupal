<?php

/**
 * @file
 * Views handler to sort by stock monthly sales.
 */

/**
 * Class commerce_reports_stock_handler_sort_monthlysales.
 */
class commerce_reports_stock_handler_sort_monthlysales extends views_handler_sort {
  /**
   * @inheritdoc
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * @inheritdoc
   */
  function query() {
    return FALSE;
  }

}
