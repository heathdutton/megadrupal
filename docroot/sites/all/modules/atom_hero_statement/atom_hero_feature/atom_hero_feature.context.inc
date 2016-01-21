<?php
/**
 * @file
 * atom_hero_feature.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function atom_hero_feature_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'hero_statement_front';
  $context->description = '';
  $context->tag = 'page';
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
        'atom_hero-atom_hero' => array(
          'module' => 'atom_hero_statement',
          'delta' => 'atom_hero',
          'region' => 'highlighted',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('page');
  $export['hero_statement_front'] = $context;

  return $export;
}
