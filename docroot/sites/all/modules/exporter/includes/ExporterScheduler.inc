<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 14:49
 */

class ExporterScheduler {

  /**
   * Create a single instance of ExportScheduler.
   */
  public static function instance() {
    static $instance;
    if (!isset($instance)) {
      $instance = new self();
    }
    return $instance;
  }

  /**
   * Protect constructor.
   */
  protected function __construct() {
  }

  public function cron() {
    if (!$this->cronIsActive()) {
      $this->cronStart();
      foreach (exporter_get_exporters() as $exporter) {
        // There is unfinished job, don't run anything
        if ($count = $exporter->queue->numberOfItems()) {
          exporter_log(WATCHDOG_INFO, '@name queue: processing, @count operations left.', array('@name' => $exporter->name, '@count' => $count));
        }
        // Wait some time after last run was finished
        elseif (REQUEST_TIME < ($exporter->last_finish + $exporter->period)) {
          $next_run = ($exporter->last_finish + $exporter->period) - REQUEST_TIME;
          exporter_log(WATCHDOG_INFO, "@name queue: waiting, next run in @interval.", array('@name' => $exporter->name, '@interval' => format_interval($next_run)));
        }
        // If everything is ok build the queue.
        else {
          $this->buildQueue($exporter);
        }
      }
      $this->cronFinish();
    }
  }

  public function batchRun(Exporter $exporter) {
    batch_set($this->getBatchQueue($exporter));
  }

  protected function getOperations(Exporter $exporter) {
    $operations = array();
    $count = $exporter->getCount();
    $limit = $exporter->limit;
    $max_offset = ceil($count / $limit) * $limit;

    $operations[] = array('exporter_worker', array($exporter->id, 'exportStart'));

    for ($offset = 0; $offset <= $max_offset; $offset += $limit ) {
      $operations[] = array('exporter_worker', array($exporter->id, 'exportItems', $offset, $limit));
    }

    $operations[] = array('exporter_worker', array($exporter->id, 'exportFinish'));

    return $operations;
  }

  protected function getBatchQueue($exporter) {
    return array(
      'operations' => $this->getOperations($exporter),
      'title' => t('Exporting items with @name', array('@name' => $exporter->name)),
      'init_message' => t('Initializing'),
      'error_message' => t('Error during @name (@id) export.', array('@name' => $exporter->name, '@id' => $exporter->id)),
      'finished' => 'exporter_batch_finished',
    );
  }

  /**
   * Create the queue, load batch definition and fill queue with new jobs again.
   */
  protected function buildQueue(Exporter $exporter) {
    $operations = $this->getOperations($exporter);
    foreach ($operations as $operation) {
      $exporter->queue->createItem($operation);
    }
    exporter_log(WATCHDOG_INFO, 'Scheduler queued @count operations.', array('@count' => count($operations)));
  }

  protected function cronStart() {
    exporter_log(WATCHDOG_INFO, 'Scheduler cron operation started.');
    variable_set('exporter_scheduler', REQUEST_TIME);
  }

  protected function cronFinish() {
    exporter_log(WATCHDOG_INFO, 'Scheduler cron operation finished.');
    variable_del('exporter_scheduler');
  }

  protected function cronIsActive() {
    $active_since = variable_get('exporter_scheduler', FALSE);
    if ($active_since) {
      exporter_log(WATCHDOG_INFO, 'Scheduler is active since @time.', array('@time' => format_date($active_since)));
      if ((REQUEST_TIME - $active_since > 3600)) {
        $this->cronFinish();
        exporter_log(WATCHDOG_WARNING, 'Scheduler cron process was stucked since @time and has been unblocked.', array('@time' => format_date($active_since)));
        $active_since = FALSE;
      }
    }
    return $active_since;
  }
}



