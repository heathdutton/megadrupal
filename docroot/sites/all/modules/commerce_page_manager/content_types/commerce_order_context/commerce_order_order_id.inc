<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Order ID'),
  'icon' => 'icon_order.png',
  'description' => t('The order ID.'),
  'required context' => new ctools_context_required(t('Commerce Order'), 'commerce_order'),
  'category' => t('Commerce Order'),
);

/**
 * Render the custom content type.
 */
function commerce_page_manager_commerce_order_order_id_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $commerce_order = clone $context->data;

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'commerce_order_order_id';
  $block->title   = t('Order ID');
  $block->content = $commerce_order->order_id;
  $block->delta   = $commerce_order->order_id;

  return $block;
}

/**
 * Returns the administrative title for a type.
 */
function commerce_page_manager_commerce_order_order_id_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" order ID', array('@s' => $context->identifier));
}
