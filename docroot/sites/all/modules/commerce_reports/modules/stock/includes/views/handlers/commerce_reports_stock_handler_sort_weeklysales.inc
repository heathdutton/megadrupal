<?php

/**
 * @file
 * Views handler to sort by stock weekly sales.
 */

/**
 * Class commerce_reports_stock_handler_sort_weeklysales.
 */
class commerce_reports_stock_handler_sort_weeklysales extends views_handler_sort {
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
