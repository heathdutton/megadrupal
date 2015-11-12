<?php
/**
 * @file
 * Search module integration.
 */

/**
 * Implements hook_search_info().
 */
function page_manager_search_search_info() {
  return array(
    'title' => 'Pages',
    'path' => 'page_manager',
  );
}

/**
 * Implements hook_search_access().
 */
function page_manager_search_search_access() {
  return user_access('access content');
}

/**
 * Implements hook_search_reset().
 */
function page_manager_search_search_reset() {
  db_update('search_dataset')
    ->fields(array('reindex' => REQUEST_TIME))
    ->condition('type', 'page_manager')
    ->execute();
}

/**
 * Implements hook_search_status().
 */
function page_manager_search_search_status() {
  module_load_include('inc', 'page_manager', 'plugins/tasks/page');
  $pages = page_manager_page_load_all();

  $total = count($pages);
  $remaining = $total - db_query("SELECT COUNT(*) FROM {search_dataset} d WHERE d.type = 'page_manager' AND d.reindex = 0")->fetchField();
  return array('remaining' => $remaining, 'total' => $total);
}

/**
 * Implements hook_search_execute().
 */
function page_manager_search_search_execute($keys = NULL, $conditions = NULL) {
  // @TODO - Add some form of access control.

  // Build matching conditions
  $query = db_select('search_index', 'i', array('target' => 'slave'))
    ->extend('SearchQuery')
    ->extend('PagerDefault')
    ->searchExpression($keys, 'page_manager');

  // Only continue if the first pass query matches.
  if (!$query->executeFirstPass()) {
    return array();
  }

  // Load results.
  $find = $query
    ->limit(10)
    ->execute();

  $results = array();
  foreach ($find as $item) {
    $page = entity_load('page_manager_search', array($item->sid), array('length' => 10));

    $results[] = array(
      'link' => url($page[$item->sid]->path, array('absolute' => TRUE)),
      'title' => $page[$item->sid]->title,
      'score' => $item->calculated_score,
      'snippet' => search_excerpt($keys, $page[$item->sid]->content),
    );
  }
  return $results;
}

/**
 * Implements hook_update_index().
 */
function page_manager_search_update_index() {
  // @TODO - Implement some for of limiting of the index.
  // $limit = (int) variable_get('search_cron_limit', 100);

  module_load_include('inc', 'page_manager', 'plugins/tasks/page');
  foreach (page_manager_page_load_all() as $subtask_id => $subtask) {
    $sid = page_manager_search_machine_name_to_dec($subtask_id);
    search_index($sid, 'page_manager', page_manager_page_execute($subtask_id));
  }
}
