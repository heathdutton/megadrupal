<?php

/**
 * @file
 * Implements display handler for checkout links.
 */

class ec_checkout_handler_field_checkout_links extends views_handler_field {
  function render($values) {
    $node = node_load($values->{$this->field_alias});

    if ($links = module_invoke_all('link', 'checkout', $node)) {
      drupal_alter('link', $links, $node);
    }

    return theme('links', array('links' => $links));
  }
}
