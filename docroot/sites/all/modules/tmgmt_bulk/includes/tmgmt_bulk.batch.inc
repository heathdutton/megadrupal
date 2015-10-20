<?php

/**
 * @file
 * Batch related functions.
 */

/**
 * Batch operation callback: searches for items and stores them in the session.
 *
 * @param string $plugin
 *   Selector plugin name.
 * @param string $item_type
 *   Source plugin item type.
 * @param array $filters
 *   An associative array with filter values. Each item represents an argument
 *   to be passed to TMGMTBulkSelectorInterface::searchItems().
 */
function _tmgmt_bulk_search_and_store($plugin, $item_type, array $filters) {
  $translated_to = $filters['translated_to'];
  $not_exists_in_jobs = $filters['not_exists_in_jobs'];
  $specific = isset($filters['specific']) ? $filters['specific'] : array();
  $_SESSION['tmgmt_bulk_storage'][$plugin][$item_type] = tmgmt_bulk_selector_plugin_controller($plugin)->searchItems($item_type, $translated_to, $not_exists_in_jobs, $specific);
}

/**
 * Batch operation callback: request translation for items.
 *
 * @param array $items
 *   An array with the following keys: "plugin", "item_type", "item_id".
 */
function _tmgmt_bulk_request_translation(array $items) {
  if (!isset($_SESSION['tmgmt_bulk_jobs_by_source_language'])) {
    $_SESSION['tmgmt_bulk_jobs_by_source_language'] = array();
  }
  $jobs_by_source_language = array();
  $languages = array();
  foreach (language_list() as $langcode => $language) {
    $languages[$langcode] = $language->name;
  }
  $target_language = $_SESSION['tmgmt_bulk_target_language'];
  foreach ($items as $item) {
    $job_item = tmgmt_job_item_create($item['plugin'], $item['item_type'], $item['item_id']);
    $job_item->save();
    $source_language = $job_item->getSourceLangCode();
    if (isset($_SESSION['tmgmt_bulk_jobs_by_source_language'][$source_language])) {
      if (isset($jobs_by_source_language[$source_language])) {
        $job = $jobs_by_source_language[$source_language];
      }
      else {
        $job = $jobs_by_source_language[$source_language] = tmgmt_job_load($_SESSION['tmgmt_bulk_jobs_by_source_language'][$source_language]);
      }
    }
    else {
      $job = $jobs_by_source_language[$source_language] = tmgmt_job_create($source_language, $target_language);
      $job->save();
      $uri = $job->uri();
      drupal_set_message(t('Created <a href="!url">translation job</a> with %language source language', array(
        '!url' => url($uri['path']),
        '%language' => $languages[$source_language],
      )));
      $_SESSION['tmgmt_bulk_jobs_by_source_language'][$source_language] = $job->tjid;
    }
    $job->addExistingItem($job_item);
  }
}
