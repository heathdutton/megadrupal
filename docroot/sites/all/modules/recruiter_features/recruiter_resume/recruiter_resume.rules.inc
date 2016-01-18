<?php

/**
 * @file
 * Rules integration for Recruiter Resume.
 */

/**
 * Implements hook_rules_condition_info().
 */
function recruiter_resume_rules_condition_info() {
  return array(
    'recruiter_resume_complete' => array(
      'label' => t('The resume is complete'),
      'group' => t('Recruiter resume'),
      'parameter' => array(
        'profile' => array(
          'label' => t('Resume'),
          'description' => t('The resume that will be checked.'),
          'type' => 'profile2',
        ),
      ),
      'base' => 'recruiter_resume_is_complete',
    ),
  );
}
