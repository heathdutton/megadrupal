<?php

/**
 * Implements hook_drush_command().
 */
function parse_drush_command() {
  $items = array();
  $items['parse-run-schedule'] = array(
    'description' => t('Iterate through the current unsent automated rules and execute any that are scheduled to run.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('psr'),
  );
  return $items;
}

/**
 * Run through the scheduled rules. Essentially - run the cron.
 */
function drush_parse_run_schedule() {
  parse_cron();
}