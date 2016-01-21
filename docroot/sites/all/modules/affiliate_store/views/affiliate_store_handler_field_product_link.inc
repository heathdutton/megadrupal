<?php

/**
 * @file
 * Views field handler for affiliate store product link.
 */

/**
 * Field handler for affiliate store product link.
 *
 * @ingroup views_field_handlers
 */
class affiliate_store_handler_field_product_link extends views_handler_field {
  /**
   * Render a product link.
   */
  function render($values) {
    $text = variable_get(
      'affiliate_store_product_call_to_action', t('Buy now')
    );
    // Make product URL site friendly.
    $url = affiliate_store_convert_product_url($this->get_value($values));
    return theme('affiliate_store_go', array('text' => $text, 'url' => $url));
  }
}
