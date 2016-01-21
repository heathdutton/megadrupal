<?php
/**
 * @file
 * recruiter_menu.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_menu_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'applicant_menu';
  $context->description = 'Defines pages for applicant menu.';
  $context->tag = 'recruiter_menu';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
        'resume' => 'resume',
        'search/job*' => 'search/job*',
        'job-bookmarks' => 'job-bookmarks',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Defines pages for applicant menu.');
  t('recruiter_menu');
  $export['applicant_menu'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'applicant_menu_block';
  $context->description = 'Add applicant menu block if applicant menu context is active';
  $context->tag = 'recruiter_menu';
  $context->conditions = array(
    'context' => array(
      'values' => array(
        'applicant_menu' => 'applicant_menu',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-applicant-menu' => array(
          'module' => 'menu',
          'delta' => 'menu-applicant-menu',
          'region' => 'navigation',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Add applicant menu block if applicant menu context is active');
  t('recruiter_menu');
  $export['applicant_menu_block'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'login_menu_block';
  $context->description = '';
  $context->tag = 'recruiter_menu';
  $context->conditions = array(
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-login-menu' => array(
          'module' => 'menu',
          'delta' => 'menu-login-menu',
          'region' => 'navigation',
          'weight' => '-26',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('recruiter_menu');
  $export['login_menu_block'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'recruiter_menu';
  $context->description = 'Defines pages for recruiter menu';
  $context->tag = 'recruiter_menu';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'recruiter' => 'recruiter',
        'recruiter/*' => 'recruiter/*',
        'manage/job*' => 'manage/job*',
        'search/resume*' => 'search/resume*',
        'resume-bookmarks' => 'resume-bookmarks',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Defines pages for recruiter menu');
  t('recruiter_menu');
  $export['recruiter_menu'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'recruiter_menu_block';
  $context->description = 'Add recruiter menu block if recruiter menu context is active';
  $context->tag = 'recruiter_menu';
  $context->conditions = array(
    'context' => array(
      'values' => array(
        'recruiter_menu' => 'recruiter_menu',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-recruiter-menu' => array(
          'module' => 'menu',
          'delta' => 'menu-recruiter-menu',
          'region' => 'navigation',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Add recruiter menu block if recruiter menu context is active');
  t('recruiter_menu');
  $export['recruiter_menu_block'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user_menu_block';
  $context->description = '';
  $context->tag = 'recruiter_menu';
  $context->conditions = array(
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'system-user-menu' => array(
          'module' => 'system',
          'delta' => 'user-menu',
          'region' => 'navigation',
          'weight' => '-26',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('recruiter_menu');
  $export['user_menu_block'] = $context;

  return $export;
}
