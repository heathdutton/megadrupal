<?php

/**
 * Displays the shopping cart form and associated information.
 * @see commerce_cart_view()
 * - changes embedded view to customized view
 */
function commerce_cart_view_override_commerce_cart_view() {
  global $user;

  // Default to displaying an empty message.
  $content = theme('commerce_cart_empty_page');

  // First check to make sure we have a valid order.
  if ($order = commerce_cart_order_load($user->uid)) {
    $view_name = variable_get('commerce_cart_view_override_page_view', 'commerce_cart_form');
    $enabled_views = views_get_enabled_views();
    if (isset($enabled_views[$view_name])) {
      $wrapper = entity_metadata_wrapper('commerce_order', $order);

      // Only show the cart form if we found product line items.
      if (commerce_line_items_quantity($wrapper->commerce_line_items, commerce_product_line_item_types()) > 0) {

        // Add the form for editing the cart contents.
        $content = commerce_embed_view($view_name, 'default', array($order->order_id), 'cart');
        if (!empty($content)) {
          drupal_add_css(drupal_get_path('module', 'commerce_cart') . '/theme/commerce_cart.theme.css');
        }
      }
    }
    else {
      // Fallback to checkout
      drupal_goto('checkout');
    }
  }

  return $content;
}
