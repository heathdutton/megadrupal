<?php
/**
 * @file
 * easy_blog_layout.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function easy_blog_layout_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'blog_global';
  $context->description = 'General blog context.';
  $context->tag = 'Blog';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'blog' => 'blog',
        'blog/*' => 'blog/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'search-form' => array(
          'module' => 'search',
          'delta' => 'form',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'easy_blog-easy_blog_archive' => array(
          'module' => 'easy_blog',
          'delta' => 'easy_blog_archive',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
        'views-easy_category-block' => array(
          'module' => 'views',
          'delta' => 'easy_category-block',
          'region' => 'sidebar_second',
          'weight' => '-8',
        ),
        'views-easy_category-block_1' => array(
          'module' => 'views',
          'delta' => 'easy_category-block_1',
          'region' => 'sidebar_second',
          'weight' => '-7',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blog');
  t('General blog context.');
  $export['blog_global'] = $context;

  return $export;
}
