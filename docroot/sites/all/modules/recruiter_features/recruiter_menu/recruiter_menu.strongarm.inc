<?php
/**
 * @file
 * recruiter_menu.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function recruiter_menu_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pm_existing_pages_disabled_manage_jobs';
  $strongarm->value = FALSE;
  $export['pm_existing_pages_disabled_manage_jobs'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pm_existing_pages_disabled_resume';
  $strongarm->value = FALSE;
  $export['pm_existing_pages_disabled_resume'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pm_existing_pages_disabled_resume_search';
  $strongarm->value = FALSE;
  $export['pm_existing_pages_disabled_resume_search'] = $strongarm;

  return $export;
}
