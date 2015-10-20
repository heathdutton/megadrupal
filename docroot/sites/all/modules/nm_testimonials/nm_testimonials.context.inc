<?php
/**
 * @file
 * nm_testimonials.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_testimonials_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_testimonials_landing';
  $context->description = 'Block for Testimonials landing page';
  $context->tag = 'testimonials';
  $context->conditions = array();
  $context->reactions = array();
  $context->condition_mode = 0;
  
  // Translatables
  // Included for use with string extractors like potx.
  t('Block for Testimonials landing page');
  t('testimonials');

  $export['nm_testimonials_landing'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_testimonials_node';
  $context->description = 'Blocks on Testimonial nodes';
  $context->tag = 'testimonials';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'nm_testimonial' => 'nm_testimonial',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'testimonials',
    'block' => array(
      'blocks' => array(
        'nm_testimonials-nm_testimonials_node_add' => array(
          'module' => 'nm_testimonials',
          'delta' => 'nm_testimonials_node_add',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks on Testimonial nodes');
  t('testimonials');
  $export['nm_testimonials_node'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_testimonials_pages';
  $context->description = 'Blocks on select pages';
  $context->tag = 'testimonials';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'contact' => 'contact',
        'user/register' => 'user/register',
      ),
    ),
    'node' => array(
      'values' => array(
        'nm_page' => 'nm_page',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_testimonials-block' => array(
          'module' => 'views',
          'delta' => 'nm_testimonials-block',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks on select pages');
  t('testimonials');
  $export['nm_testimonials_pages'] = $context;

  return $export;
}
