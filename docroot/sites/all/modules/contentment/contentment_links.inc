<?php
/**
 * @file
 * Provides custom panel content for the Contentment module.
 */

$plugin = array(
  'title' => t('Contentment: Links'),
  'description' => t('Add Content links for the various content types.'),
  'render callback' => 'contentment_links_render',
  'single' => TRUE,
  'category' => 'Contentment',
);

/**
 * Render the Contentment: Links content.
 */
function contentment_links_render($subtype, $conf, $args, $context) {
  $block = new stdClass();

  $block->title = t('Add Content');
  $block->content = contentment_node_add_links();

  return $block;
}

