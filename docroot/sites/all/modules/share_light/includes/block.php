<?php

/**
 * @file
 * Block related functions.
 */

use \Drupal\share_light\Block;

/**
 * Implements hook_block_info().
 */
function share_light_block_info() {
  $blocks = array();

  $blocks['current_page'] = array(
    'info' => t('Share this page'),
    'cache' => DRUPAL_CACHE_PER_PAGE,
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function share_light_block_view($id) {
  if ($block = Block::from_current_path()) {
    return $block->render();
  }
}
