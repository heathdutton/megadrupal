<?php

/**
 * @file
 * Implements display handler for checkout.
 */

class ec_checkout_views_handler_field_checkout extends views_handler_field {
  function render($values) {
    $node = node_load($values->{$this->field_alias});

    // FIXME: move this if statement into the the ec_checkout_product_form(), so it doesn't need to be replicated everywhere
    if (isset($node->ptype) && ec_product_can_purchase($node, 'ec_checkout_form') &&
        (!variable_get('ec_product_cart_addition_by_link', TRUE) ||
        ec_product_attributes_get($node, 'use_product_cart_form'))) {
      return drupal_get_form('ec_checkout_product_form', $node, TRUE);
    }
    elseif (isset($node->ptype)) {
      if ($links = module_invoke_all('link', 'checkout', $node)) {
        drupal_alter('link', $links, $node);
      }

      return theme('links', array('links' => $links));
    }
  }
}
