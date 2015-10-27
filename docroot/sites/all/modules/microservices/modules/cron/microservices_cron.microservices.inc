<?php
/**
 * @file
 */

/*
 * Implements hook_microservices_info().
 */
function microservices_cron_microservices_info() {
  return array(
    MICROSERVICES_CRON => array(
      'events' => array(
        MICROSERVICES_CRON_EVENT_CRON,
        MICROSERVICES_CRON_EVENT_TICK,
      ),
    ),
  );
}
