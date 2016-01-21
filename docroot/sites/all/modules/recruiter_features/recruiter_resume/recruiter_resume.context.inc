<?php
/**
 * @file
 * recruiter_resume.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_resume_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'resume_view';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'resume/*' => 'resume/*',
        'resume' => 'resume',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'recruiter_resume-resume_state_info' => array(
          'module' => 'recruiter_resume',
          'delta' => 'resume_state_info',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  $export['resume_view'] = $context;

  return $export;
}
