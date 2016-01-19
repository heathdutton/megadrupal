<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Order state'),
  'icon' => 'icon_order.png',
  'description' => t('The order state.'),
  'required context' => new ctools_context_required(t('Commerce Order'), 'commerce_order'),
  'category' => t('Commerce Order'),
);

/**
 * Render the custom content type.
 */
function commerce_page_manager_commerce_order_state_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $commerce_order = clone $context->data;

  $status = commerce_order_status_load($commerce_order->status);
  $state = commerce_order_state_load($status['state']);

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'commerce_order_state';
  $block->title   = t('Order state');
  $block->content = $state['title'];
  $block->delta   = 'commerce_order_state';

  return $block;
}

/**
 * Returns the administrative title for a type.
 */
function commerce_page_manager_commerce_order_state_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" order state', array('@s' => $context->identifier));
}
