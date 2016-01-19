<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Order number'),
  'icon' => 'icon_node.png',
  'description' => t('The order number.'),
  'required context' => new ctools_context_required(t('Commerce Order'), 'commerce_order'),
  'category' => t('Commerce Order'),
);

/**
 * Render the custom content type.
 */
function commerce_page_manager_commerce_order_order_number_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $commerce_order = clone $context->data;

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'commerce_order_order_number';
  $block->title   = t('Order number');
  $block->content = $commerce_order->order_number;
  $block->delta   = $commerce_order->order_number;

  return $block;
}

/**
 * Returns the administrative title for a type.
 */
function commerce_page_manager_commerce_order_order_number_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" order number', array('@s' => $context->identifier));
}
