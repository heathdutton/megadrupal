<?php

/**
 * @file
 * Implements display handler for checkout form.
 */

class ec_checkout_handler_field_checkout_form extends views_handler_field {
  function render($values) {
    $node = node_load($values->{$this->field_alias});

    if (isset($node->ptype) && ec_product_can_purchase($node)) {
      return drupal_get_form('ec_checkout_product_form', $node, TRUE);
    }
  }
}
