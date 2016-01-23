<?php

/**
 * Destination handler for subqueues.
 */
class NodequeueSubqueueDestination extends MigrateDestinationTable {
  public function __construct() {
    parent::__construct('nodequeue_subqueue');
  }

  /**
   * Get schema of {nodequeue_subqueue}.
   */
  public static function getKeySchema() {
    return parent::getKeySchema('nodequeue_subqueue');
  }

  /**
   * @override
   *
   * Use nodequeue API.
   */
  public function import(stdClass $d7subqueue, stdclass $d6subqueue) {
    $queue = nodequeue_load($d7subqueue->qid);
    if (empty($queue)) {
      throw new MigrateException(t('Unknown queue id  @qid', array('@qid' => $d7subqueue->qid)));
    }
    $subqueue = nodequeue_add_subqueue($queue, $d7subqueue->title, $d7subqueue->reference);
    $this->numCreated++;
    return array($subqueue->sqid);
  }

  /**
   * @override
   *
   * Use nodequeue API.
   */
  public function rollback(array $key) {
    $sqid = $key['destid1'];
    nodequeue_remove_subqueue($sqid);
  }

}

