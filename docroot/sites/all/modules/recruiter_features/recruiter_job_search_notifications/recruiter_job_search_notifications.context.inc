<?php
/**
 * @file
 * recruiter_job_search_notifications.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_job_search_notifications_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'job_search_notifications';
  $context->description = 'Dispalys the saved search block';
  $context->tag = 'recruiter_job_search_notifications';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'job_search:search_page' => 'job_search:search_page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'search_api_saved_searches-job_search' => array(
          'module' => 'search_api_saved_searches',
          'delta' => 'job_search',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Dispalys the saved search block');
  t('recruiter_job_search_notifications');
  $export['job_search_notifications'] = $context;

  return $export;
}
