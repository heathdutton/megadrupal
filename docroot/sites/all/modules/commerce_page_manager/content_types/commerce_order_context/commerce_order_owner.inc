<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 * @todo: Expand this to not be single, pick a property.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Order owner'),
  'icon' => 'icon_order.png',
  'description' => t('The order owner.'),
  'required context' => new ctools_context_required(t('Commerce Order'), 'commerce_order'),
  'category' => t('Commerce Order'),
);

/**
 * Render the custom content type.
 */
function commerce_page_manager_commerce_order_owner_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $commerce_order = clone $context->data;

  $wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
  $user = $wrapper->{'owner'}->value();

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'commerce_order_owner';
  $block->title   = t('Order owner');
  $block->content = $user->name;
  $block->delta   = 'commerce_order_owner';

  return $block;
}

/**
 * Returns the administrative title for a type.
 */
function commerce_page_manager_commerce_order_owner_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" order owner', array('@s' => $context->identifier));
}
