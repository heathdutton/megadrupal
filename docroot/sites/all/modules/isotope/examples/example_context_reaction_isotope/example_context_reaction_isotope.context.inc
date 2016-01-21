<?php
/**
 * @file
 * example_context_reaction_isotope.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function example_context_reaction_isotope_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'isotope_example_context';
  $context->description = 'Example context reaction setting the Isotope masonry layout on /node';
  $context->tag = 'Isotope';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'node' => 'node',
      ),
    ),
  );
  $context->reactions = array(
    'isotope' => array(
      'container_selector' => '#block-system-main > .content',
      'item_selector' => '#block-system-main > .content .node',
      'layout_mode' => 'masonry',
      'column_width' => '220',
      'row_height' => '200',
      'resizable' => 1,
      'resize_container' => 1,
      'animation_engine' => 'best-available',
      'apply_css' => 1,
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Example context reaction setting the Isotope masonry layout on /node');
  t('Isotope');
  $export['isotope_example_context'] = $context;

  return $export;
}
