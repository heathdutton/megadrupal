<?php
/**
 * @file
 * iati_feature.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function iati_feature_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'activity_edit_form';
  $context->description = 'Adds blocks to the activity edit form';
  $context->tag = 'IATI';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'iati_activity' => 'iati_activity',
      ),
      'options' => array(
        'node_form' => '2',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'relation_add-block' => array(
          'module' => 'relation_add',
          'delta' => 'block',
          'region' => 'content',
          'weight' => '1',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds blocks to the activity edit form');
  t('IATI');
  $export['activity_edit_form'] = $context;

  return $export;
}
