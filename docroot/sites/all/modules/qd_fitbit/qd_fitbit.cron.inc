<?php
/**
 * @file
 * Cron-related functions
 */

function qd_fitbit_cron() {
  qd_fitbit_process_all_queues();
}
