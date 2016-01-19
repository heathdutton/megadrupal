<?php
/**
 * @file
 * This file contains processing for the news.
 */

/**
 * Implements hook_preprocess_block().
 */
function openlucius_news_preprocess_block(&$variables) {
  if ($variables['block']->module == 'openlucius_news' && $variables['block']->delta == 'news_block') {
    $variables['block_html_id'] = drupal_html_id('news');
  }
}
