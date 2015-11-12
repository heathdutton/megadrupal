<?php
/**
 * @file
 * atom_resources_feature.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function atom_resources_feature_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'atom_resources_navigation';
  $context->description = 'Keeps the resources link active when viewing resources.';
  $context->tag = 'navigation';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'resource' => 'resource',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => array(
      0 => 'main-menu:resources',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Keeps the resources link active when viewing resources.');
  t('navigation');
  $export['atom_resources_navigation'] = $context;

  return $export;
}
