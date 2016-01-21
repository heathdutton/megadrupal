<?php
/**
 * @file
 * recruiter_job.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function recruiter_job_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-job-add-links.
  $menus['menu-job-add-links'] = array(
    'menu_name' => 'menu-job-add-links',
    'title' => 'Job add links',
    'description' => 'Contains links adding jobs.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Contains links adding jobs.');
  t('Job add links');


  return $menus;
}
