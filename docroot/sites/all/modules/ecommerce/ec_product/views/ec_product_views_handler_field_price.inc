<?php

/**
 * @file
 * Handles the display of product price.
 */

class ec_product_views_handler_field_price extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array(
      'table' => 'ec_product',
      'field' => 'nid',
    );
  }

  function render($value) {
    $node = node_load($value->{$this->aliases['nid']});
    if (!empty($node->ptype) && !ec_product_attributes_get($node, 'hide_product_price')) {
      $price = ec_product_get_final_price($node, 'product');
      return format_currency($price);
    }
    else {
      return;
    }
  }
}

