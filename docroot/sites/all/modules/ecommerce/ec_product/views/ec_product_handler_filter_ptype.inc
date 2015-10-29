<?php

/**
 * @file
 * Handles filtering by product types.
 */

class ec_product_handler_filter_ptype extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Product type');
      $ptypes = ec_product_get_types('names');
      foreach ($ptypes as $ptype => $name) {
        $options[$ptype] = $name;
      }
      $this->value_options = $options;
    }
  }
}
