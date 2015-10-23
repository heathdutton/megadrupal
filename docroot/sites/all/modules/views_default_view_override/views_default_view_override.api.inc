<?php

/**
 * Announce a default view that should be overridden by another view.
 *
 * @return An associative array keyed by machine names for default views to
 * override with values representing machine names of views to override with.
 */
function hook_default_views_overrider_views_to_override() {
  return array(
    'commerce_cart_block' => 'my_module_commerce_cart_block_override',
  );
}
