<?php
/**
 * @file
 * Defines the add to cart form handler class.
 */

/**
 * Class views_handler_field_shopify_product_add_to_cart_form
 *
 * Provides a custom views field for the add to cart form.
 */
class views_handler_field_shopify_product_add_to_cart_form extends views_handler_field_entity {

  /**
   * Renders the add to cart form.
   */
  function render($data) {
    if ($entity = $this->get_value($data)) {
      return drupal_get_form('shopify_add_to_cart_form', $entity);
    }
  }

}
