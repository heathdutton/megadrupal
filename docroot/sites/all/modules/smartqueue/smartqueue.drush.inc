<?php

/**
 * @file
 * Smart Queue drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function smartqueue_drush_command() {
  $items['smartqueue-process'] = array(
    'description' => 'Process items in the SmartQueue.',
    'aliases' => array('sqp'),
  );

  return $items;
}

/**
 * Worker function to process the SmartQueue.
 */
function drush_smartqueue_process() {
  smartqueue_queue_process();
}
