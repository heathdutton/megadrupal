<?php

/**
 * Destionation handler for {nodequeue_nodes}.
 */
class NodequeueNodeDestination extends MigrateDestinationTable {
  public function __construct() {
    parent::__construct('nodequeue_nodes');
  }

  /**
   * {nodequeue_nodes} has no primary key so we define it excplicitely here.
   */
  public static function getKeySchema() {
    return array(
      'qid' => array(
        'description' => 'Queue id',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'sqid' => array(
        'description' => 'Subqueue this node is in',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'nid' => array(
        'description' => 'Node id in this subqueue',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
    );
  }

  /**
   * @override
   *
   * Use nodequeue API.
   */
  public function import(stdClass $d7content, stdclass $d6content) {
    $queue = nodequeue_load($d7content->qid);
    $subqueue = nodequeue_load_subqueue($d7content->sqid);
    nodequeue_subqueue_add($queue, $subqueue, $d7content->nid);
    $this->numCreated++;
    return array($d7content->qid, $d7content->sqid, $d7content->nid);
  }

  /**
   * @override
   *
   * Use nodequeue API.
   */
  public function rollback(array $key) {
    $sqid = $key['destid2'];
    $nid  = $key['destid3'];
    nodequeue_subqueue_remove_node($sqid, $nid);
  }

}

