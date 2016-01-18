<?php
/**
 * @file
 * recruiter_menu.pm_existing_pages.inc
 */

/**
 * Implements hook_pm_existing_pages_info().
 */
function recruiter_menu_pm_existing_pages_info() {
  $export = array();

  $pm_existing_page = new stdClass();
  $pm_existing_page->api_version = 1;
  $pm_existing_page->name = 'manage_jobs';
  $pm_existing_page->label = 'Manage jobs';
  $pm_existing_page->context = '';
  $pm_existing_page->paths = 'manage/job';
  $export['manage_jobs'] = $pm_existing_page;

  $pm_existing_page = new stdClass();
  $pm_existing_page->api_version = 1;
  $pm_existing_page->name = 'resume';
  $pm_existing_page->label = 'Resume';
  $pm_existing_page->context = '';
  $pm_existing_page->paths = 'resume';
  $export['resume'] = $pm_existing_page;

  $pm_existing_page = new stdClass();
  $pm_existing_page->api_version = 1;
  $pm_existing_page->name = 'resume_search';
  $pm_existing_page->label = 'Resume search';
  $pm_existing_page->context = '';
  $pm_existing_page->paths = 'search/resume';
  $export['resume_search'] = $pm_existing_page;

  return $export;
}
