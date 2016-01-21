<?php
/**
 * @file
 * recruiter_resume_search.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_resume_search_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'resume_search_block';
  $context->description = 'Provides the fulltext search block for resumes';
  $context->tag = 'recruiter_resume_search';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'resume_search' => 'resume_search',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-resume_search-page_1' => array(
          'module' => 'views',
          'delta' => '-exp-resume_search-page_1',
          'region' => 'search',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Provides the fulltext search block for resumes');
  t('recruiter_resume_search');
  $export['resume_search_block'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'resume_search_facets';
  $context->description = 'Display search api facets in resume search';
  $context->tag = 'recruiter_resume_search';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'resume_search' => 'resume_search',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'facetapi-20lwlBykNTHsiVDNAjG1STlcNs5ZOkAd' => array(
          'module' => 'facetapi',
          'delta' => '20lwlBykNTHsiVDNAjG1STlcNs5ZOkAd',
          'region' => 'sidebar_first',
          'weight' => '-9',
        ),
        'facetapi-0E13X9GxANL1YrsKm6GzqUBvUn9dwwKX' => array(
          'module' => 'facetapi',
          'delta' => '0E13X9GxANL1YrsKm6GzqUBvUn9dwwKX',
          'region' => 'sidebar_first',
          'weight' => '-8',
        ),
        'facetapi-Npc9qPmJF0Z7K11E3oY7p0Z1dHoH0Rpr' => array(
          'module' => 'facetapi',
          'delta' => 'Npc9qPmJF0Z7K11E3oY7p0Z1dHoH0Rpr',
          'region' => 'sidebar_first',
          'weight' => '-7',
        ),
        'facetapi-Z1OEEIJTmFLiH0khl0e9VwNxTjr1km0o' => array(
          'module' => 'facetapi',
          'delta' => 'Z1OEEIJTmFLiH0khl0e9VwNxTjr1km0o',
          'region' => 'sidebar_first',
          'weight' => '-6',
        ),
        'facetapi-MCIaDpM84mEXHKio1Bs8HcYiueQECFo8' => array(
          'module' => 'facetapi',
          'delta' => 'MCIaDpM84mEXHKio1Bs8HcYiueQECFo8',
          'region' => 'sidebar_first',
          'weight' => '-5',
        ),
        'facetapi-xETXZbrE5MuFZJhqCQ361R0xPQQIUoDH' => array(
          'module' => 'facetapi',
          'delta' => 'xETXZbrE5MuFZJhqCQ361R0xPQQIUoDH',
          'region' => 'sidebar_first',
          'weight' => '-4',
        ),
        'facetapi-PvSkNRl00z4DVBGE2G0g0wziYXD2YPFO' => array(
          'module' => 'facetapi',
          'delta' => 'PvSkNRl00z4DVBGE2G0g0wziYXD2YPFO',
          'region' => 'sidebar_first',
          'weight' => '-3',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Display search api facets in resume search');
  t('recruiter_resume_search');
  $export['resume_search_facets'] = $context;

  return $export;
}
