<?php
/**
 * @file
 * Block template functions.
 */

/**
 * Implements hook_preprocess_block().
 */
function center_preprocess_block(&$vars) {

  // Set shortcut variables. Hooray for less typing!
  $block_id = $vars['block']->module . '-' . $vars['block']->delta;
  $classes = &$vars['classes_array'];
  $title_classes = &$vars['title_attributes_array']['class'];
  $content_classes = &$vars['content_attributes_array']['class'];
  $suggestions = &$vars['theme_hook_suggestions'];

  // Add global classes to all blocks.
  $title_classes[] = 'block-title';
  $content_classes[] = 'block-content';

  // Create a development print variable.
  $vars['dev_deets'] = theme_get_setting('dev_mode') ?
    '<!-- Block ID: ' . $block_id . ' -->' : '';

  // Uncomment the lines below to see variables you can use to target a block.
  // This requires http://drupal.org/project/devel to be installed.
  // dpm($block_id, 'Block ID:');

  // Add classes based on the block delta.
  switch ($block_id) {
    case 'system-main':
      $suggestions[] = 'block__nuked';
      break;
  }
}

/**
 * Returns the render array for a block.
 *
 * @param string $block_module
 *   The machine name of the module providing the block.
 * @param string $block_name
 *   The machine name of the block name.
 *
 * @return array
 *   The block's render array
 */
function center_block_array($block_module, $block_name) {
  return module_invoke($block_module, 'block_view', $block_name);
}
