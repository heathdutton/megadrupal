<?php

/**
 * @file
 * Field handler to translate a product key owner into its readable form.
 */
class commerce_product_key_handler_field_product_key_owner extends commerce_product_key_handler_field_product_key {
  function render($values) {
    $owner = $this->get_value($values);
    $value = commerce_product_key_load_by_owner($owner);

    return $this->render_link($this->sanitize_value($owner), $values);
    //return $this->sanitize_value($owner);
  }
}
