<?php
/**
 * @file
 * Handle cron management and data syncing.
 */
/**
 * Cron task interface
 */
interface LyrisCronTask {
  /**
   * Constructor
   *
   * @abstract
   *
   * Needs to pass a unique cron task name to the parent.
   * Ex. parent::__construct('lyris_sync');
   */
  public function __construct();
  
  /**
   * Procedure to run.
   *
   * @abstract
   */
  public function run();
}

/**
 * Base cron class to handle variables and timers.
 */
class Cron {
  // The name of the cron task to run.  Associates with field settings in the
  // lyris cron admin form.
  private $name;
  
  // The timestamp of the last run, in seconds.
  private $last_run;
  
  // Interval between cron runs (in seconds)
  private $interval;
  
  // The variable storing the timestamp of the last run.
  private $semaphore;
  
  // Timers for measuring efficiency.
  private $start_time;
  private $stop_time;
  
  // Log successes and failures per process.
  protected $processed = array('success' => array(), 'failure' => array());
  
  /**
   * Constructor
   */
  public function Cron($name) {
    $this->semaphore = "{$name}_cron_last_run";
    $this->interval  = (int) variable_get("{$name}_cron_interval") * 60;
    $this->last_run  = (int) variable_get($this->semaphore, 0);
  }
  /**
   * Destructor
   *
   * Catch the timer if the process bails out.
   */
  public function __destruct() {
    if (!$this->stop_time) {
      $this->stop_time = time();
      $this->writeLogSummary();
    }
  }
  
  /**
   * Reset the timestamp on a successful cron run.
   */
  private function setCronTimeStamp() {
    variable_set($this->semaphore, REQUEST_TIME);
  }
  
  /**
   * Determine whether it is time for cron to run again.
   */
  public function shouldRun() {
    return ($this->last_run <= REQUEST_TIME - $this->interval);
  }
  
  /**
   * Start the timer.
   */
  public function startTimer() {
    $this->start_time = time();
  }
  
  /**
   * Stop the timer.
   */
  public function stopTimer() {
    $this->stop_time = time();
  }
  
  /**
   * Write a summary of the task to watchdog.
   */
  public function writeLogSummary() {
    $success = count($this->processed['success']);
    $failure = count($this->processed['failure']);
    
    $diff  = $this->stop_time - $this->start_time;
    $msg   = 'Successfully processed @success of @total records in @time at @rate records/second.';
    $total = $success + $failure;
    $rate  = ($total ? round($total/$diff) : 0);
    $vars  = array('@success' => $success, '@total' => $total, '@time' => format_interval($diff), '@rate' => $rate);
    watchdog('lyris_cron', $msg, $vars, WATCHDOG_INFO);
  }
}

/**
 * Cron task to handle syncing entities to Lyris.
 */
class BatchSync extends Cron {
  /**
   * Constructor
   */
  public function BatchSync() {
    parent::__construct('lyris_sync');
  }
  
  public function run() {
    $batch = variable_get('lyris_sync_cron_batch_count', 100);
    
    // Load up to $batch number of unsynced records, ordered oldest to most
    // recent by change timestamp.
    foreach (lyris_get_unsynced(array(), $batch) as $record) {
      // Load the entity
      $entity = lyris_entity_load_entity($record['entity_type'], $record['eid']);

      if (!method_exists($entity, 'pushToLyris')) {
        continue;
      }
            
      // Populate the struct
      $entity->populateStruct();
      
      // Push it to Lyris
      if ($entity->pushToLyris()) {
        $this->processed['success'][] = $entity->getDisplayName();
      }
      else {
        $this->processed['failure'][] = $entity->getDisplayName();
      }
    }
      
    // Log the detailed summary somewhere.
  }
}