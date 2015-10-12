<?php
/**
 * @file
 * courseplanner.pm_existing_pages.inc
 */

/**
 * Implements hook_pm_existing_pages_info().
 */
function courseplanner_pm_existing_pages_info() {
  $export = array();

  $pm_existing_page = new stdClass();
  $pm_existing_page->api_version = 1;
  $pm_existing_page->name = 'cp_lesson';
  $pm_existing_page->label = 'Lesson';
  $pm_existing_page->context = 'entity|field_collection_item|item_id';
  $pm_existing_page->paths = 'field-collection/cp-offering-lessons/%field_collection_item';
  $export['cp_lesson'] = $pm_existing_page;

  return $export;
}
