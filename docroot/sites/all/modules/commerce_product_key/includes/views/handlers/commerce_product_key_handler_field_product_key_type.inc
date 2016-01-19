<?php

/**
 * @file
 * Field handler to translate a product type into its readable form.
 */
class commerce_product_key_handler_field_product_key_type extends commerce_product_key_handler_field_product_key {
  function render($values) {
    $type = $this->get_value($values);
    $value = commerce_product_key_type_get_name($type);

    return $this->render_link($this->sanitize_value($value), $values);
  }
}
