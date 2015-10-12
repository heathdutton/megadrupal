<?php
/**
 * @file
 */

/**
 * Implements hook_workers_info_alter().
 */
function microservices_cron_workers_info_alter(&$items) {
  foreach ($items as $worker_name => $worker_info) {
    foreach ($worker_info['actions'] as $action_name => $action_info) {
      if ($action_name == (MICROSERVICES_CRON  . ' ' . MICROSERVICES_QUEUE_REQUEST)) {
        $items[$worker_name]['actions'][$action_name]['callback'] = 'microservices_cron_tick_callback';
        break;
      }
    }
  }
}

function microservices_cron_tick_callback($params, $name, $worker) {
  static $last_cron_time;
  static $last_tick_time;

  if (!isset($last_cron_time)) {
    $last_cron_time = time();
  }

  $now = time();

  $result = FALSE;

  // Generate cron each minutes.
  if (($now - $last_cron_time) >= 60) {
    $last_cron_time = $last_cron_time = $now;
    $date = new DateTime();
    $date->setTimestamp($now);
    $expressions = array();
    foreach (microservices_cron_info() as $name => $expression) {
      $cron = Cron\CronExpression::factory($expression);
      if ($cron->isDue($date)) {
        $expressions[] = $name;
      }
    }
    if ($expressions) {
      watchdog('microservices_cron', 'Cron \'%exp\'', array('%exp' => json_encode($expressions)), WATCHDOG_INFO);
      microservices_send_event(
        MICROSERVICES_CRON_EVENT_CRON,
        array(
          'time' => $now,
          'date' => date('r', $now),
          'expressions' => array_unique($expressions),
        )
      );

      $result = TRUE;
    }
  }
  // Generate cron tick each 5 seconds.
  else if (!$last_tick_time || ($now - $last_tick_time) >= 5) {
    $last_tick_time = $last_tick_time = $now;
    microservices_send_event(
      MICROSERVICES_CRON_EVENT_TICK,
      array(
        'time' => $last_tick_time,
        'date' => date('r', $last_tick_time),
      )
    );

    $result = TRUE;
  }

  return $result;
}
