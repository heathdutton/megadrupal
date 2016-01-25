<?php

/**
 * @file
 * Field handler to present a link to delete a product key.
 */
class commerce_product_key_handler_field_product_key_link_delete extends commerce_product_key_handler_field_product_key {
  function construct() {
    parent::construct();

    $this->additional_fields['type'] = 'type';
    $this->additional_fields['uid'] = 'uid';
  }

  function render($values) {
    // Ensure the user has access to delete this product.
    $product_key = commerce_product_key();
    $product_key->product_key_id = $this->get_value($values, 'product_key_id');
    $product_key->type = $this->get_value($values, 'type');
    $product_key->uid = $this->get_value($values, 'uid');

    if (!commerce_product_access('delete', $product_key)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');

    return l($text, 'admin/commerce/product-keys/' . $product_key->product_key_id . '/delete', array('query' => drupal_get_destination()));
  }
}
