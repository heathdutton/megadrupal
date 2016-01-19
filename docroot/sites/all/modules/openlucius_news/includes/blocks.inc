<?php
/**
 * @file
 * This file contains all blocks for the news.
 */

/**
 * Implements hook_block_info().
 */
function openlucius_news_block_info() {
  return array(
    'news_block' => array(
      'info' => t('News items'),
    ),
    'promoted_news_block' => array(
      'info' => t('Promoted news items'),
    ),
  );
}

/**
 * Implements hook_block_view().
 */
function openlucius_news_block_view($delta = '') {

  // Load the functions for the block.
  module_load_include('inc', 'openlucius_news', 'includes/functions');

  // Initialize array.
  $block = array();

  switch ($delta) {

    // The regular news block.
    case 'news_block':
      $block['subject'] = NULL;
      $block['content'] = openlucius_news_build_news_items_display();
      break;

    // The promoted news block.
    case 'promoted_news_block':
      $block['subject'] = NULL;
      $block['content'] = openlucius_news_build_promoted_news_items_display();
      break;
  }
  // Return.
  return $block;
}
