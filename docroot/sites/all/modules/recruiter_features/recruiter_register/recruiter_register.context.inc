<?php
/**
 * @file
 * recruiter_register.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function recruiter_register_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'register_applicant_info';
  $context->description = 'Adds registration information for applicants.';
  $context->tag = 'recruiter_register';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user/register' => 'user/register',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'recruiter_register-applicant_info' => array(
          'module' => 'recruiter_register',
          'delta' => 'applicant_info',
          'region' => 'featured',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds registration information for applicants.');
  t('recruiter_register');
  $export['register_applicant_info'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'register_recruiter_info';
  $context->description = 'Adds registration information for recruiters.';
  $context->tag = 'recruiter_register';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user/register/recruiter' => 'user/register/recruiter',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'recruiter_register-recruiter_info' => array(
          'module' => 'recruiter_register',
          'delta' => 'recruiter_info',
          'region' => 'featured',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds registration information for recruiters.');
  t('recruiter_register');
  $export['register_recruiter_info'] = $context;

  return $export;
}
