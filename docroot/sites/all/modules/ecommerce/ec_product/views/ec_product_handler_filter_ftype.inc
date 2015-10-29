<?php

/**
 * @file
 * Handles the filtering by product features.
 */

class ec_product_handler_filter_ftype extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Product feature');
      $ftypes = ec_product_feature_get('names');
      foreach ($ftypes as $ftype => $name) {
        $options[$ftype] = $name;
      }
      $this->value_options = $options;
    }
  }
}
