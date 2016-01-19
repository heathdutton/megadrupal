<?php
/**
 * @file
 * openlucius_news.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function openlucius_news_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'news_items';
  $context->description = '';
  $context->tag = 'News';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'openlucius_news-promoted_news_block' => array(
          'module' => 'openlucius_news',
          'delta' => 'promoted_news_block',
          'region' => 'content',
          'weight' => '-10',
        ),
        'openlucius_news-news_block' => array(
          'module' => 'openlucius_news',
          'delta' => 'news_block',
          'region' => 'content',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('News');
  $export['news_items'] = $context;

  return $export;
}
