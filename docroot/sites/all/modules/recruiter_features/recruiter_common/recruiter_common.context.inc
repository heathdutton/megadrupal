<?php
/**
 * @file
 * recruiter_common.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_common_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'recruiter_render_slim';
  $context->description = 'Render in slim mode';
  $context->tag = 'recruiter';
  $context->conditions = array(
    'recruiter_render_slim' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Render in slim mode');
  t('recruiter');
  $export['recruiter_render_slim'] = $context;

  return $export;
}
