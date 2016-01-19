<?php

/**
 * Stock lifetime
 */
class commerce_fulfilment_oms_handler_field_omsstocklevel extends views_handler_field_custom {
  function pre_render(&$values) {
    if (!isset($this->oms_stock_levels)) {
      $skus = array();
      foreach ($values as $value) {
        $skus[] = $value->commerce_product_sku;
      }
      $this->oms_stock_levels = _commerce_fulfilment_oms_get_warehouse_quantity($skus);
    }

    foreach ($values as &$value) {
      $sku = $value->commerce_product_sku;
      $value->oms_stock_value = isset($this->oms_stock_levels[$sku])
        ? $this->oms_stock_levels[$sku]
        : 0;
    }
  }

  function render($value) {
    return $value->oms_stock_value;
  }
}
