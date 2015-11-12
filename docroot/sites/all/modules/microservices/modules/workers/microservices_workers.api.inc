<?php
/**
 * @file
 */

/*
 * Implements hook_microservices_workers_info().
 */
function hook_microservices_workers_info() {
  return array(
    'main' => array(
      'title' => t('Main worker'),
      'services' => array(
        MICROSERVICES_SERVICE,
      ),
    ),
    'worker1' => array(
      'title' => t('Worker 1'),
      'services' => array(
        'foo',
        'bar',
      ),
    ),
  );
}
