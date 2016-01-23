<?php
/**
 * @file
 * Drush integration for Geckoboard Push.
 */

/**
 * Implements hook_drush_help().
 */
function geckoboard_push_drush_help($section) {
  switch ($section) {
    case 'drush:geckoboard-push':
      return dt('Run Drupal queue workers. As opposed to "drush cron" that can only be run one at a time on a single site, "drush queue-cron" can be invoked as many times as the server load allows. Otherwise, schedule like drush cron.');
  }
}

/**
 * Implements hook_queue_drush_command().
 */
function geckoboard_push_drush_command() {
  $items['geckoboard-push'] = array(
    'callback' => 'geckoboard_push_send_data',
    'description' => dt('Send latest stats to Geckoboard'),
    'aliases' => array('gbp'),
    'drupal dependencies' => array('geckoboard_push'),
  );
  return $items;
}
