<?php
/**
 * @file
 * Block hooks and block functionality.
 */
/**
 * Implements hook_block_info().
 */
function social_counters_block_info() {
  $blocks['counters'] = array(
    'info' => t('Social Counters'),
    'cache' => DRUPAL_CACHE_GLOBAL,
  );

  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function social_counters_block_view($delta = '') {
  $block = array();

  switch ($delta) {
    case 'counters':
      $block['subject'] = t('Social Counters');
      $block['content'] = array(
        '#theme' => 'social_counters_block',
        '#items' => social_counters_counters_output(),
      );
      break;
  }

  return $block;
}
