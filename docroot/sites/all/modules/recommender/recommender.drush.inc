<?php

/**
 * Implements hook_drush_command().
 */
function recommender_drush_command() {
  $items = array();
  $items['recommender-run'] = array(
    'description' => "Process recommender commands sequentially. You need to submit a command in the admin interface first.",
    //'aliases' => array('rr'),
  );
  $items['recommender-cron'] = array(
    'description' => "Process 'cron callback' defined in hook_recommender_data().",
    'options' => array(
      'force' => 'Force run cron callbacks.',
    ),
  );
  $items['recommender-upkeep'] = array(
    'description' => "Process 'upkeep callback' defined in hook_recommender_data().",
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function recommender_drush_help($section) {
  switch ($section) {
    case 'drush:recommender-run':
      return dt("Process recommender commands sequentially.");
  }
}


function drush_recommender_run() {
  $count = 0;
  while ($record = computing_claim('recommender')) {
    drush_log("Processing recommender computation: {$record->id}", 'status');
    recommender_process_record($record);
    computing_update($record);
    $count++;
  };
  drush_log("Processed $count recommender tasks.", 'ok');
}


function drush_recommender_cron() {
  if (variable_get('recommender_cron_strategy', 'drupal') == 'drush' || drush_get_option('force', FALSE)) {
    drush_log("Processing recommender cron callbacks ...", 'status');
    recommender_cron_process(time());
    drush_log("Recommender cron callbacks run complete.", 'ok');
  }
  else {
    drush_log("Recommender cron is setup to run with Drupal. Please set it to run with drush or use the '--force' option.", 'warning');
  }
}


function drush_recommender_upkeep() {
  drush_log("Processing recommender upkeep callbacks ...", 'status');
  recommender_upkeep_process(time());
  drush_log("Recommender upkeep callbacks run complete.", 'ok');
}