<?php
// $Id$

/**
 * Callback function to supply a list of content types.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Ubercart: SKU#'),
  'icon' => 'icon_node.png',
  'description' => t('Render the SKU# for the related product node'),
  'required context' => new ctools_context_required(t('Ubercart Product'), 'uc'),
  'category' => t('Ubercart'),
  'content type' => 'uc_sku',
  'edit form' => 'uc_panels_uc_sku_content_type_edit_form',
  'render callback' => 'uc_panels_uc_sku_content_type_render',
);

/**
 * Render the description
 */
function uc_panels_uc_sku_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }


  // Get a shortcut to the node.
  $node = $context->data;

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'uc_sku';
  $block->title   = t('Product SKU#');
  $block->content = theme('uc_product_model', array('model' => $node->model));
  $block->delta   = $node->nid;

  return $block;
}

/**
 * Edit form for the description content type.
 */
function uc_panels_uc_sku_content_type_edit_form($form, &$form_state) {
  // Empty form to ensure we have the override title + context gadgets.
  return $form;
}


/**
 * Returns the administrative title for a type.
 */
function uc_panels_uc_sku_content_type_admin_title($subtype, $conf, $context) {
  return t('Product SKU#');
}
