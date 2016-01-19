<?php

/**
 * Destination handler for nodequeues.
 */
class NodequeueQueueDestination extends MigrateDestinationTable {
  public function __construct() {
    parent::__construct('nodequeue_queue');
  }

  /**
   * Get schema of {nodequeue_queue}.
   */
  public static function getKeySchema() {
    return parent::getKeySchema('nodequeue_queue');
  }

  /**
   * @override
   *
   * Use nodequeue API.
   */
  public function import(stdClass $d7queue, stdclass $d6queue) {
    $this->prepare($d7queue, $d6queue);
    $qid = nodequeue_save($d7queue);
    $this->complete($d7queue, $d6queue);
    $this->numCreated++;
    return array($qid);
  }

  /**
   * @override
   *
   * Use nodequeue API.
   */
  public function rollback(array $key) {
    $qid = $key['destid1'];
    nodequeue_delete($qid);
  }

  /**
   * Give handlers a chance to modify the object before saving it.
   */
  public function prepare($d7queue, $d6queue) {
    $migration = Migration::currentMigration();
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($d7queue, $d6queue);
    }
  }

  /**
   * Give handlers a chance to modify the object after saving it.
   */
  public function complete($d7queue, $d6queue) {
    $migration = Migration::currentMigration();
    if (method_exists($migration, 'complete')) {
      $migration->complete($d7queue, $d6queue);
    }
  }

}

