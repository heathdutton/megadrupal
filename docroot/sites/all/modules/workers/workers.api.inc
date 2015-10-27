<?php
/**
 * @file
 */

/**
 * Implements hook_workers_info().
 */
function hook_workers_info() {
  return array(
    'worker1' => array(
      'title' => t('Worker 1'),
      'actions' => array(
        'foo_worker' => array(
          'callback' => 'foo_worker_callback',
          'params' => array(514, 340, 'Some text!'),
        ),
        'bar_worker_callback' => array(),
      ),
    ),
  );
}
