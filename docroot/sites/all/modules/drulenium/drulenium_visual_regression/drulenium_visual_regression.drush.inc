<?php

/**
 * @file
 * Generate & compare snapshots via drush framework.
 */

/**
 * Implements hook_drush_command().
 */
function drulenium_visual_regression_drush_command() {
  $items['vr-create'] = array(
    'callback' => 'drush_drulenium_visual_regression_create',
    'description' => 'Create release and generate snapshots.',
    'drupal dependencies' => array('drulenium_visual_regression'),
    'arguments' => array(
      'base_url' => 'Base URL of the site to test.',
      'release_name' => 'Name of the release to create.',
    ),
    'options' => array(
      'role' => 'A user role to test the site as. Defaults to anonymous.',
    ),
    'aliases' => array('vrc'),
  );
  $items['vr-diff'] = array(
    'callback' => 'drush_drulenium_visual_regression_diff',
    'description' => 'Generate comparision report for 2 releases.',
    'drupal dependencies' => array('drulenium_visual_regression'),
    'arguments' => array(),
    'options' => array(),
    'aliases' => array('vrd'),
  );
  return $items;
}

/**
 * Command callback. Create a release & generate snapshots.
 */
function drush_drulenium_visual_regression_create($vr_base_url = "", $release_name = "") {
  $drulenium_vr_path_custom = variable_get("drulenium_vr_path_custom");
  $drulenium_vr_user_pass = variable_get("drulenium_vr_release_user_pass");
  $drulenium_vr_user_name = variable_get("drulenium_vr_release_user_name");
  //$drulenium_vr_path_custom = str_replace("&lt;","<",$drulenium_vr_path_custom);
  //$drulenium_vr_path_custom = str_replace("&gt;",">",$drulenium_vr_path_custom);
  $secureid = drupal_hash_base64(drupal_random_bytes(55));
  if (empty($release_name)) {
    $release_name = format_date(REQUEST_TIME);
  }
  $pages_array = array();
  if (empty($vr_base_url)) {
    global $base_url;
    $vr_base_url = $base_url."/?q=";
  }
  $re_id = drulenium_visual_regression_create_release_in_db($secureid, $release_name, $vr_base_url, $pages_array, $drulenium_vr_user_name, $drulenium_vr_user_pass, $specific_node_types = array(), $specific_term_types = array(), $specific_pages = $drulenium_vr_path_custom);

  // Process the queue using Batch API.
  $batch = array(
    'file' => drupal_get_path('module', 'drulenium_visual_regression') . '/drulenium_visual_regression.module',
    'operations' => array(),
    'finished' => 'drulenium_visual_regression_release_batch_finished',
    'title' => t('Performing %operation on the selected items...', array('%operation' => "snapshot generation")),
  );
  $progress = 0; // where to start
  $limit = 3; // how many to process for each run
 
  $query = db_select('drulenium_vr_runs', 'ru')
    ->fields('ru')
    ->condition('re_id', $re_id,'=')
    ->condition('snapshot', '','=');
  $query->addExpression('COUNT(rid)', 'run_count');
  $result = $query->execute();
  $run = $result->fetchAssoc();
  $max = $run['run_count'];

  while ($progress <= $max) {
    $batch['operations'][] = array('drulenium_visual_regression_release_batch_op', array($vr_base_url, $re_id, $progress, $limit));
    $progress = $progress + $limit;
  }
  batch_set($batch);
  drush_backend_batch_process();
  
  drush_log(t('Took snapshots of @vrpages pages.', array('@vrpages' => count($drulenium_vr_path_custom))), 'success');
}

/**
 * Command callback. Generate comparision report for 2 releases.
 */
function drush_drulenium_visual_regression_diff() {
  //Get the last two releases
  $result = db_select('drulenium_vr_releases', 'vr_re')
    ->fields('vr_re', array('rid'))
    ->orderBy('rid', 'DESC')
    ->range(0,2)
    ->execute();
  $row1 = $result->fetchAssoc();
  $new_re_id = $row1['rid'];
  $row2 = $result->fetchAssoc();
  $old_re_id = $row2['rid'];
  if ($new_re_id < $old_re_id) {
    $tmp = $new_re_id;
    $new_re_id = $old_re_id;
    $old_re_id = $tmp;
  }
  drulenium_visual_regression_create_initial_comparision_report_in_db($old_re_id, $new_re_id);
 
  // Process the queue using Batch API.
  $batch = array(
    'file' => drupal_get_path('module', 'drulenium_visual_regression') . '/drulenium_visual_regression.module',
    'operations' => array(),
    'finished' => 'drulenium_visual_regression_compare_batch_finished',
    'title' => t('Performing %operation on the selected items...', array('%operation' => "snapshot comparision")),
  );
  $progress = 0; // where to start
  $limit = 1; // how many to process for each run
  
  // Get unprocessed runs of specified compare
  // where snapshot is empty.
  $query = db_select('drulenium_vr_results', 'vr_re');
  $query
    ->fields('vr_re')
    ->condition('vr_re.release_base_id', $old_re_id)
    ->condition('vr_re.release_cmp_id', $new_re_id)
    ->isNull('vr_re.diff_percent');
  $query->addExpression('COUNT(rid)', 'run_count');
  $result = $query->execute();
  $row = $result->fetchAssoc();
  $max = $row['run_count'];

  while ($progress <= $max) {
    $batch['operations'][] = array('drulenium_visual_regression_compare_batch_op', array($old_re_id, $new_re_id, $progress, $limit));
    $progress = $progress + $limit;
  }
  
  batch_set($batch);
  drush_backend_batch_process();
}
