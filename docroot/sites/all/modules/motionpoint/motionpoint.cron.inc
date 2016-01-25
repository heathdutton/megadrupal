<?php
/**
 * @file
 * Cron configuration for module motionpoint.
 */

/**
 * Implements hook_cron().
 */
function motionpoint_cron() {
  $resend_time = variable_get('motionpoint_settings_resend_time');
  $query = db_select(MOTIONPOINT_JOBS_TABLE, 'mt');
  $query->fields('mt', array());
  $query->condition('translated_date', REQUEST_TIME - $resend_time, '<=');
  $query->leftJoin('locales_source', 'ls', 'ls.lid = mt.lid');
  $query->fields('ls', array('source'));
  $jobs = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  foreach ($jobs as $job) {
    if ($job['status'] != MOTIONPOINT_STATUS_TRANSLATED) {
      $res = motionpoint_translate_job($job, $job['source']);
      if (!$res) {
        watchdog('error', "Error on translation request(mtid {$job['mtid']})");
        break;
      }
    }
  }
}
