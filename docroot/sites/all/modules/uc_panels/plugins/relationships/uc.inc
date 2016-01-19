<?php
// $Id$

/**
 * @file plugins/relationships/uc.inc
 *
 * Plugin to provide an relationship handler for nodes posted as products.
 */
function uc_panels_uc_ctools_relationships() {
  $args['uc'] = array(
    'title' => t('Ubercart Product from node'),
    'keyword' => 'uc',
    'description' => t('Adds a product from a node context; if multiple groups are associated with a node, this will get the "first" group only.'),
    'required context' => new ctools_context_required(t('Node'), 'node'),
    'context' => 'uc_panels_group_from_node_context',
  );
  return $args;
}

/**
 * Return a new context based on an existing context
 */
function uc_panels_group_from_node_context($context = NULL, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL
  if (empty($context->data)) {
    return ctools_context_create_empty('uc');
  }

  if (isset($context->data->type) && uc_product_is_product($context->data->type)) {
    return ctools_context_create('uc', $context->data);
  }
}
