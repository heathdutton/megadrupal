<?php
/**
 * @file
 */

/**
 * Implements hook_workers_info().
 */
function microservices_workers_workers_info() {
  $result = array();
  $queue_types = array(
    MICROSERVICES_QUEUE_REPLY,
    MICROSERVICES_QUEUE_REQUEST,
  );
  foreach (microservices_workers_info() as $worker => $info) {
    $item = array(
      'title' => isset($info['title']) ? $info['title'] : $worker,
      'actions' => array(),
    );
    foreach ($info['services'] as $service) {
      foreach ($queue_types as $type) {
        $item['actions']["$service $type"] = array(
          'callback'  => 'microservices_workers_execute',
          'params' => array(
            'service' => $service,
            'queue type' => $type,
          ),
        );
      }
    }
    $result[$worker] = $item;
  }
  return $result;
}
