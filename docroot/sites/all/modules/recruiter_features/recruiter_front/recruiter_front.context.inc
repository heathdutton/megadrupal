<?php
/**
 * @file
 * recruiter_front.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_front_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'recruiter_front';
  $context->description = 'Adds blocks to the frontpage.';
  $context->tag = 'recruiter_front';
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
        'recruiter_job_search-job_total_count' => array(
          'module' => 'recruiter_job_search',
          'delta' => 'job_total_count',
          'region' => 'search',
          'weight' => '-24',
        ),
        'views--exp-job_search-search_page' => array(
          'module' => 'views',
          'delta' => '-exp-job_search-search_page',
          'region' => 'search',
          'weight' => '-10',
        ),
        'views-front_page_job_search-block_1' => array(
          'module' => 'views',
          'delta' => 'front_page_job_search-block_1',
          'region' => 'content',
          'weight' => '-27',
        ),
        'facetapi-v3NKwi4cgrJI5BhGVl3K6y1dktYkhuo8' => array(
          'module' => 'facetapi',
          'delta' => 'v3NKwi4cgrJI5BhGVl3K6y1dktYkhuo8',
          'region' => 'content',
          'weight' => '-26',
        ),
        'facetapi-SZ9qoxuugBBvUZ39HNC99AUUIkZtp592' => array(
          'module' => 'facetapi',
          'delta' => 'SZ9qoxuugBBvUZ39HNC99AUUIkZtp592',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds blocks to the frontpage.');
  t('recruiter_front');
  $export['recruiter_front'] = $context;

  return $export;
}
