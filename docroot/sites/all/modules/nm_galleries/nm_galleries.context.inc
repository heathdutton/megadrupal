<?php
/**
 * @file
 * nm_galleries.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_galleries_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_galleries_landing';
  $context->description = '';
  $context->tag = 'galleries';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'landing/gallery' => 'landing/nm_galleries',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-nm_galleries_featured_gallery' => array(
          'module' => 'boxes',
          'delta' => 'nm_galleries_featured_gallery',
          'region' => 'preface_first',
          'weight' => '-10',
        ),
        'views-nm_blog-recent' => array(
          'module' => 'views',
          'delta' => 'nm_blog-recent',
          'region' => 'postscript_second',
          'weight' => '-10',
        ),
        'views-nm_events-upcoming' => array(
          'module' => 'views',
          'delta' => 'nm_events-upcoming',
          'region' => 'postscript_third',
          'weight' => '-10',
        ),
        'views-nm_announcements-recent' => array(
          'module' => 'views',
          'delta' => 'nm_announcements-recent',
          'region' => 'postscript_third',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('galleries');
  $export['nm_galleries_home'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_galleries_node';
  $context->description = 'On Gallery nodes.';
  $context->tag = 'galleries';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'nm_gallery' => 'nm_gallery',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_galleries-recent' => array(
          'module' => 'views',
          'delta' => 'nm_galleries-recent',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
    'menu' => 'galleries',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('On Gallery nodes.');
  t('galleries');
  $export['nm_galleries_node'] = $context;

  return $export;
}
