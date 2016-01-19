<?php
// $Id$

/**
 * Tell ctools about the content type.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Ubercart: Add to Cart Link(s)'),
  'icon' => 'icon_node.png',
  'description' => t('Render the form for Add to Cart button(s) for the related product node'),
  'required context' => new ctools_context_required(t('Ubercart Product'), 'uc'),
  'category' => t('Ubercart'),
  'content type' => 'uc_add_to_cart',
  'edit form' => 'uc_panels_uc_add_to_cart_content_type_edit_form',
  'render callback' => 'uc_panels_uc_add_to_cart_content_type_render',
);

/**
 * Render the description
 */
function uc_panels_uc_add_to_cart_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $node = $context->data;

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'uc_add_to_cart';
  $block->title   = t('Add to Cart');
  $form_state = array();
  $form = drupal_get_form('uc_product_add_to_cart_form', $node);
  $block->content = theme('uc_product_add_to_cart', array('form' => $form));
  $block->delta   = $node->nid;

  return $block;
}

/**
 * Edit form for the description content type.
 */
function uc_panels_uc_add_to_cart_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}


/**
 * Returns the administrative title for a type.
 */
function uc_panels_uc_add_to_cart_content_type_admin_title($subtype, $conf, $context) {
  return t('Add to Cart Link(s)');
}
