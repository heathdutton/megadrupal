<?php

/**
 * @file
 * Rules integration for Recruiter Job Application.
 */

/**
 * Implements hook_rules_condition_info().
 */
function recruiter_job_application_rules_condition_info() {
  return array(
    'recruiter_job_application_exists_job' => array(
      'label' => t('Job application exists for job'),
      'group' => t('Recruiter job application'),
      'parameter' => array(
        'applicant' => array(
          'label' => t('Applicant'),
          'description' => t('The applicant which sent the application.'),
          'type' => 'user',
        ),
        'job' => array(
          'label' => t('Resume'),
          'description' => t('The job for which the application should be.'),
          'type' => 'node',
        ),
      ),
      'base' => 'recruiter_job_application_exisits_for_job',
    ),
  );
}
