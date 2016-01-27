<?php
/**
 * @file
 * recruiter_job.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function recruiter_job_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-job-add-links:node/add/job-per-file
  $menu_links['menu-job-add-links:node/add/job-per-file'] = array(
    'menu_name' => 'menu-job-add-links',
    'link_path' => 'node/add/job-per-file',
    'router_path' => 'node/add/job-per-file',
    'link_title' => 'Add job per file upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'query' => array(
        'destination' => 'manage/job',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '2',
  );
  // Exported menu link: menu-job-add-links:node/add/job-per-link
  $menu_links['menu-job-add-links:node/add/job-per-link'] = array(
    'menu_name' => 'menu-job-add-links',
    'link_path' => 'node/add/job-per-link',
    'router_path' => 'node/add/job-per-link',
    'link_title' => 'Add job per link',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'query' => array(
        'destination' => 'manage/job',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: menu-job-add-links:node/add/job-per-template
  $menu_links['menu-job-add-links:node/add/job-per-template'] = array(
    'menu_name' => 'menu-job-add-links',
    'link_path' => 'node/add/job-per-template',
    'router_path' => 'node/add/job-per-template',
    'link_title' => 'Add job per template',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'query' => array(
        'destination' => 'manage/job',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '4',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Add job per file upload');
  t('Add job per link');
  t('Add job per template');


  return $menu_links;
}
