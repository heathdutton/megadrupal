<?php
/**
 * @file
 * json_article.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function json_article_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'pop';
  $context->description = '';
  $context->tag = 'po';
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
        'myreactjs-myreactjs_piegraph2' => array(
          'module' => 'myreactjs',
          'delta' => 'myreactjs_piegraph2',
          'region' => 'content',
          'weight' => '-10',
        ),
        'myreactjs-myreactjs_articles' => array(
          'module' => 'myreactjs',
          'delta' => 'myreactjs_articles',
          'region' => 'content',
          'weight' => '-9',
        ),
        'myreactjs-myreactjs_piegraph1' => array(
          'module' => 'myreactjs',
          'delta' => 'myreactjs_piegraph1',
          'region' => 'sidebar_first',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('po');
  $export['pop'] = $context;

  return $export;
}
