<?php
/**
 * @file
 * recruiter_cloudy.features.inc
 */

/**
 * Implements hook_block_class_features_default_class().
 */
function recruiter_cloudy_block_class_features_default_class() {
  return array(
    'menu:menu-applicant-menu' => array(
      'module' => 'menu',
      'delta' => 'menu-applicant-menu',
      'css_classes' => 'block-main-menu block-applicant-menu',
    ),
    'menu:menu-login-menu' => array(
      'module' => 'menu',
      'delta' => 'menu-login-menu',
      'css_classes' => 'block-user-menu',
    ),
    'menu:menu-recruiter-menu' => array(
      'module' => 'menu',
      'delta' => 'menu-recruiter-menu',
      'css_classes' => 'block-main-menu block-recruiter-menu',
    ),
    'search_api_saved_searches:job_search' => array(
      'module' => 'search_api_saved_searches',
      'delta' => 'job_search',
      'css_classes' => 'agent-target',
    ),
    'search_api_saved_searches:resume_search' => array(
      'module' => 'search_api_saved_searches',
      'delta' => 'resume_search',
      'css_classes' => 'agent-target',
    ),
    'system:user-menu' => array(
      'module' => 'system',
      'delta' => 'user-menu',
      'css_classes' => 'block-user-menu',
    ),
    'user:login' => array(
      'module' => 'user',
      'delta' => 'login',
      'css_classes' => 'block-user-menu',
    ),
    'views:-exp-job_search-search_page' => array(
      'module' => 'views',
      'delta' => '-exp-job_search-search_page',
      'css_classes' => 'block-search block-job-search',
    ),
    'views:-exp-resume_search-page_1' => array(
      'module' => 'views',
      'delta' => '-exp-resume_search-page_1',
      'css_classes' => 'block-search block-resume-search',
    ),
  );
}
