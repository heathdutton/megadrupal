<?php
/**
 * @file
 * nm_announcements.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_announcements_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_announcements_pages';
  $context->description = 'Blocks for Announcements pages except node';
  $context->tag = 'announcements';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'announcements*' => 'announcements*',
        '~announcements/*/*/*' => '~announcements/*/*/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_announcements-block' => array(
          'module' => 'views',
          'delta' => 'nm_announcements-block',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
        'views-nm_announcements-block_2' => array(
          'module' => 'views',
          'delta' => 'nm_announcements-block_2',
          'region' => 'sidebar_first',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for Announcements pages except node');
  t('announcements');
  $export['nm_announcements_pages'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_announcements_landing';
  $context->description = 'Blocks for Announcements landing page';
  $context->tag = 'announcements';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'landing/blog' => 'landing/blog',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_announcements-recent' => array(
          'module' => 'views',
          'delta' => 'nm_announcements-recent',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  
  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for Announcements landing page');
  t('announcements');

  $export['nm_announcements_landing'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_announcements_node';
  $context->description = 'Blocks for Announcements node';
  $context->tag = 'announcements';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'nm_announcement' => 'nm_announcement',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_announcements-block' => array(
          'module' => 'views',
          'delta' => 'nm_announcements-block',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
        'views-nm_announcements-block_1' => array(
          'module' => 'views',
          'delta' => 'nm_announcements-block_1',
          'region' => 'sidebar_first',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for Announcements node');
  t('announcements');
  $export['nm_announcements_node'] = $context;

  return $export;
}
