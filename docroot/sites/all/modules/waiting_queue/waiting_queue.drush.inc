<?php

/**
 * Implements hook_drush_help().
 */
function waiting_queue_drush_help($section) {
  switch ($section) {
    case 'drush:process-waiting-queue':
     return dt('Run the named, waiting Drupal queue. Allows for processing queues that block indefinitely waiting for new jobs without holding up other queues.');
  }
}

/**
 * Implements hook_drush_command().
 */
function waiting_queue_drush_command() {
  $items['process-waiting-queue'] = array(
    'callback' => 'waiting_queue_process_queue',
    'description' => 'Run the named, waiting Drupal queue.',
    'arguments' => array(
      'queue_to_process' => 'The name of the queue to run.',
    ),
  );
  return $items;
}

