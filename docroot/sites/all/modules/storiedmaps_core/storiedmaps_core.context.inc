<?php
/**
 * @file
 * storiedmaps_core.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function storiedmaps_core_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'location_listing';
  $context->description = 'Add the filter to the location listing view';
  $context->tag = '';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'location_listing:page' => 'location_listing:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-location_listing-page' => array(
          'module' => 'views',
          'delta' => '-exp-location_listing-page',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Add the filter to the location listing view');
  $export['location_listing'] = $context;

  return $export;
}
