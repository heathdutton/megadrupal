<?php
/**
 * @file
 * nm_blog.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_blog_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_blog_landing';
  $context->description = 'Blocks for a blog focused landing page.';
  $context->tag = 'blog';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'landing/blog' => 'landing/nm_blog',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_events-upcoming' => array(
          'module' => 'views',
          'delta' => 'nm_events-upcoming',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
        'views-nm_galleries-recent' => array(
          'module' => 'views',
          'delta' => 'nm_galleries-recent',
          'region' => 'sidebar_first',
          'weight' => '-9',
        ),
        'views-nm_announcements-recent' => array(
          'module' => 'views',
          'delta' => 'nm_announcements-recent',
          'region' => 'sidebar_first',
          'weight' => '-8',
        ),
        'views-nm_testimonials-block_1' => array(
          'module' => 'views',
          'delta' => 'nm_testimonials-block_1',
          'region' => 'sidebar_first',
          'weight' => '-7',
        ),
        'boxes-nm_galleries_featured_gallery' => array(
          'module' => 'boxes',
          'delta' => 'nm_galleries_featured_gallery',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  
  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for a blog focused landing page.');
  t('blog');

  $export['nm_blog_landing'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_blog_node';
  $context->description = 'Blocks for Blog nodes';
  $context->tag = 'blog';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'nm_blog' => 'nm_blog',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_blog-block' => array(
          'module' => 'views',
          'delta' => 'nm_blog-block',
          'region' => 'sidebar_first',
          'weight' => '-14',
        ),
        'views-nm_blog-block_2' => array(
          'module' => 'views',
          'delta' => 'nm_blog-block_3',
          'region' => 'sidebar_first',
          'weight' => '-13',
        ),
        'views-blog_by_user-block_1' => array(
          'module' => 'views',
          'delta' => 'blog_by_user-block_1',
          'region' => 'sidebar_first',
          'weight' => '-12',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for Blog nodes');
  t('blog');
  $export['nm_blog_node'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_blog_pages';
  $context->description = 'Blocks for Blog pages except node';
  $context->tag = 'blog';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'blog*' => 'blog*',
        '~blog/*/*/*' => '~blog/*/*/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_blog-block' => array(
          'module' => 'views',
          'delta' => 'nm_blog-block',
          'region' => 'sidebar_first',
          'weight' => '-14',
        ),
        'views-nm_blog-block_2' => array(
          'module' => 'views',
          'delta' => 'nm_blog-block_2',
          'region' => 'sidebar_first',
          'weight' => '-13',
        ),
        'views-blog_by_user-block_1' => array(
          'module' => 'views',
          'delta' => 'blog_by_user-block_1',
          'region' => 'sidebar_first',
          'weight' => '-12',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for Blog pages except node');
  t('blog');
  $export['nm_blog_pages'] = $context;

  return $export;
}
