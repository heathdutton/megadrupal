<?php
// $Id$

/**
 * Tell ctools about the content type.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Ubercart: Product Price'),
  'icon' => 'icon_node.png',
  'description' => t('Render the price for the related product node'),
  'required context' => new ctools_context_required(t('Ubercart Product'), 'uc'),
  'category' => t('Ubercart'),
  'content type' => 'uc_price',
  'edit form' => 'uc_panels_uc_price_content_type_edit_form',
  'render callback' => 'uc_panels_uc_price_content_type_render',
);

/**
 * Render the description
 */
function uc_panels_uc_price_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }


  // Get a shortcut to the node.
  $node = $context->data;

  $context = array(
    'revision' => 'themed',
    'type' => 'product',
    'class' => array(
      'product',
    ),
    'subject' => array(
      'node' => $node,
    ),
  );
  $uc_context['class'][1] = 'display';

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'uc_price';
  $block->title   = t('Price');
  $block->content = theme('uc_product_price', array('element' => array('#value'=>$node->sell_price)));
  $block->delta   = $node->nid;

  return $block;
}

/**
 * Edit form for the description content type.
 */
function uc_panels_uc_price_content_type_edit_form($form, &$form_state) {
  // Empty form to ensure we have the override title + context gadgets.
  return $form;
}


/**
 * Returns the administrative title for a type.
 */
function uc_panels_uc_price_content_type_admin_title($subtype, $conf, $context) {
  return t('Price');
}
