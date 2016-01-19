<?php
/**
 * @file
 * API file.
 */

/**
 * Implements hook_stock_calculation_alter().
 *
 * Useful to add extra contextual information to new plugins.
 */
function hook_stock_calculation_alter($plugin_type, &$contexts) {

  if ($plugin_type == 'field_value') {

    $product = $contexts['product'];
    $instance = field_info_instance('commerce_product', 'commerce_stock', $product->type);

    $context['field_instance'] = $instance;

  }

}
