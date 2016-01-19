<?php

/**
 * @file
 * View handler to display stock lifetime.
 */

/**
 * Class commerce_reports_stock_handler_field_stocklifetime.
 */
class commerce_reports_stock_handler_field_stocklifetime extends views_handler_field_custom {
  /**
   * @inheritdoc
   */
  function render($values) {
    $sku = $values->commerce_product_sku;
    return _commerce_reports_stock_calculate_dataset($sku, 'lifetime');
  }

}
