<?php
/**
 * @file
 * recruiter_resume_search_notifications.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_resume_search_notifications_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'resume_search_notifications';
  $context->description = 'Dispalys the saved search block.';
  $context->tag = 'recruiter_resume_search_notifications';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'resume_search:page_1' => 'resume_search:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'search_api_saved_searches-resume_search' => array(
          'module' => 'search_api_saved_searches',
          'delta' => 'resume_search',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Dispalys the saved search block.');
  t('recruiter_resume_search_notifications');
  $export['resume_search_notifications'] = $context;

  return $export;
}
