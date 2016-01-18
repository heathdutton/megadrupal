<?php
/**
 * @file
 * recruiter_job.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_job_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'job_menu_add_links';
  $context->description = '';
  $context->tag = 'recruiter_job';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'manage/job' => 'manage/job',
        'manage/job/unpublished' => 'manage/job/unpublished',
      ),
    ),
    'user' => array(
      'values' => array(
        'administrator' => 'administrator',
        'content admin' => 'content admin',
        'job admin' => 'job admin',
        'recruiter' => 'recruiter',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-job-add-links' => array(
          'module' => 'menu',
          'delta' => 'menu-job-add-links',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('recruiter_job');
  $export['job_menu_add_links'] = $context;

  return $export;
}
