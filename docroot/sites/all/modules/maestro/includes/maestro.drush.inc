<?php


/**
 * @file
 * Drush commands for the maestro module
 */

/**
 * Implementation of hook_drush_command().
 */
function maestro_drush_command() {
  $items['orchestrator'] = array(
    'callback' => 'maestro_execute_orchestrator',
    'description' => dt('Execute the maestro workflow orchestrator.'),
    'aliases' => array('orch'),
    'examples' => array(
      'drush orchestrator' => 'Run the orchestrator to execute and complete any scheduled maestro workflow tasks.',
    ),
  );
  $items['maestro-purge'] = array(
    'callback' => 'maestro_purge_tables',
    'description' => dt('Execute the maestro workflow orchestrator.'),
    'aliases' => array('maestropurge'),
    'examples' => array(
      'drush maestro-purge' => 'Purge all the maestro execution history, all project tracking, orchestrator queue and process records.',
    ),
    'options' => array(
      'yes' => 'Skip confirmation',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function maestro_drush_help($section) {
  switch ($section) {
  case 'drush:orchestrator':
    return dt("Run the orchestrator to execute and complete any scheduled maestro workflow tasks.");
    break;
  case 'drush:maestro-purge':
    return dt('Purge all the maestro execution history, all project tracking, orchestrator queue and process records.');
    break;
  }
}

/**
 * Execute the orchestrator
 */
function maestro_execute_orchestrator() {
  if (!lock_acquire('maestro_orchestrator', variable_get('maestro_orchestrator_lock_delay', 512.0))) {
    watchdog('maestro', 'Attempting to re-run orchestrator while it is already running.', array(), WATCHDOG_WARNING);
  }
  else {
    $maestro = Maestro::createMaestroObject(1);
    $maestro->engine()->cleanQueue();
    lock_release('maestro_orchestrator');
  }
  drush_print(dt('Maestro Orchestrator executed successfully.'));
}


/**
 * Truncate and Purge all maestro related history and tracking data
 */
function maestro_purge_tables() {

  drush_print(dt("Purging will delete all maestro related execution and tracking history and cannot be undone.\nALWAYS TEST YOUR BACKUPS ON A NON-PRODUCTION SERVER!"));
  if (!drush_confirm(dt('Are you sure you want purge these records from the database?'))) {
    return drush_user_abort();
  }

  $completion_message = '';
  $tables = array(
    'maestro_process',
    'maestro_process_variables',
    'maestro_queue',
    'maestro_queue_from',
    'maestro_projects',
    'maestro_production_assignments',
    'maestro_project_comments',
    'maestro_project_content'
  );

  foreach ($tables as $table) {
    $result = db_truncate($table)->execute();
    $completion_message .= t("Succesfully truncated or purged @rec_count records from @table table\n", array('@rec_count' => $result, '@table' => $table));
  }

  drush_print(dt($completion_message));
}

