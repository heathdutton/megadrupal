<?php

/**
 * @file
 * View handler to display monthly sales.
 */

/**
 * Class commerce_reports_stock_handler_field_monthlysales.
 */
class commerce_reports_stock_handler_field_monthlysales extends views_handler_field_custom {

  /**
   * @inheritdoc
   */
  function render($values) {
    $sku = $values->commerce_product_sku;
    return _commerce_reports_stock_calculate_dataset($sku, 'monthlysales');
  }

}
