<?php

/**
 * @file
 * batch operations.
 */

/**
 * Represents a batch task.
 */
interface TumblerConnectBatchInterface {

  /**
   * Sets multiple properties.
   *
   * @param int $id
   *   Unique identifier of task.
   * @param int $nid
   *   Node identifier.
   * @param int $vid
   *   Node revision identifier.
   * @param int $post_id
   *   Tumblr remote service unique content identifier.
   * @param string $operation
   *   Operation property (add,modify,delete)..
   */
  public function init($id = NULL, $nid = NULL, $vid = NULL, $post_id = NULL, $operation = NULL);

   /**
   * Gets the task unique identifier.
   *
   * @return integer
   *   The unique identifier for this task.
   */
  public function getId();

  /**
   * Sets the task unique identifier.
   *
   * @param $id
   *   The unique identifier for this task.
   */
  public function setId($id);

  /**
   * Gets the node identifier.
   *
   * @return integer
   *   The node nid identifier related to this task.
   */
  public function getNid();

  /**
   * Sets the node identifier.
   *
   * @param $nid
   *   The node nid identifier related to this task.
   */
  public function setNid($nid);

  /**
   * Gets the node revision identifier.
   *
   * @return integer
   *   The node vid revision identifier related to this task.
   */
  public function getVid();

  /**
   * Sets the node revision identifier.
   *
   * @param $vid
   *   The node vid revision identifier related to this task.
   */
  public function setVid($vid);


  /**
   * Gets the tumblr remote content identifier.
   *
   * @return integer
   *   The tumblr content post id identifier related to this task.
   */
  public function getPostId();

  /**
   * Sets the tumblr remote content identifier.
   *
   * @param $post_id
   *   The tumblr content post id identifier related to this task.
   */
  public function setPostId($post_id);

  /**
   * Gets the operation property for this task.
   *
   * @return string
   *   The operation property (add, modify, delete).
   */
  public function getOperation();

  /**
   * Sets the operation property for this task.
   *
   * @param $operation
   *   The operation property (add, modify, delete).
   */
  public function setOperation($operation);

  /**
   * Gets the properties on this object as an array.
   *
   * @return array
   *   The properties on this object as an array.
   */
  public function toArray();
}

/**
 * Default implementation of TumblerConnectBatchInterface.
 *
 * Represents a batch task.
 */
class TumblrConnectBatch implements TumblerConnectBatchInterface {

  const ADD = 'add';
  const UPDATE = 'mod';
  const DELETE = 'del';

  const STATE_ADD_ACTION = 700;      // add the batch record as requested
  const STATE_REPLACE_ACTION = 701;  // replace the  current batch record instead of add
  const STATE_REMOVE_ALL_RECORDS_FOR_NODE = 702; // delete all batch records from queue related to this node

  const STATE_NO_ACTION_NEEDED = 705;
  const STATE_NO_CURRENT_RECORD = 706;

  /**
   * Unique identifier for batch request instance.
   *
   * @var integer.
   */
  protected $id;

  /**
   * Node nid identifier.
   *
   * @var integer.
   */
  protected $nid;

  /**
   * Node vid revision identifier.
   *
   * @var integer.
   */
  protected $vid;

  /**
   * Tumblr remote content unique identifier.
   *
   * @var integer.
   */
  protected $post_id;

  /**
   * Command operation type.
   *
   * @var string.
   *  add, mod, or del
   */
  protected $operation;

  /**
   * Default constructor.
   */
  public function __construct() {

  }

  /**
   * Sets multiple properties.
   *
   * Implements TumblerConnectBatchInterface::init().
   */
  public function init($id = NULL, $nid = NULL, $vid = NULL, $post_id = NULL, $operation = NULL) {

    if (isset($id)) {
      $this->id = $id;
    }

    if (isset($nid)) {
      $this->nid = $nid;
    }

    if (isset($vid)) {
      $this->vid = $vid;
    }

    if (isset($post_id)) {
      $this->post_id = $post_id;
    }

    if (isset($operation)) {
      $this->operation = $operation;
    }
  }

  public function getId() {
    return $this->id;
  }

  public function setId($id) {
    $this->id = $id;
  }

  public function getNid() {
    return $this->nid;
  }

  public function setNid($nid) {
    $this->nid = $nid;
  }

  public function getVid() {
    return $this->vid;
  }

  public function setVid($vid) {
    $this->vid = $vid;
  }

  public function getPostId() {
    return $this->post_id;
  }

  public function setPostId($post_id) {
    $this->post_id = $post_id;
  }

  public function getOperation() {
    return $this->operation;
  }

  public function setOperation($operation) {
    $this->operation = $operation;
  }

  /**
   * Implements TumblerConnectBatchInterface::toArray().
   *
   * Convert to an array.
   */
  public function toArray() {
    return array('id' => $this->id,
        'nid' => $this->nid,
        'vid' => $this->vid,
        'post_id' => $this->post_id,
        'operation' => $this->operation,
    );
  }

}

/**
 * Represents batch task with related node.
 */
interface TumblrConnectNodeBatchInterface {

  /**
   * Gets the node object property.
   *
   * @return
   *   Node object related to this task.
   */
  public function getNode();

  /**
   * Sets the node object property.
   *
   * @param $node
   *   The node object property of this task.
   */
  public function setNode($node);

   /**
   * Gets the batch task property.
   *
   * @return
   *   TumblerConnectBatchInterface.
   */
  public function getTask();

  /**
   * Sets the batch task property.
   *
   * @param $task
   *   TumblerConnectBatchInterface.
   */
  public function setTask(TumblerConnectBatchInterface $task);
}

/**
 * Represents batch task with related node.
 *
 * Default implementation of TumblrConnectNodeBatchInterface.
 */
class TumblrConnectNodeBatch implements TumblrConnectNodeBatchInterface {

  /**
   * Related node object data..
   *
   * @var object.
   */
  protected $node;

  /**
   * Batch command task.
   *
   * @var TumblrConnectNodeBatch.
   */
  protected $task;

  /**
   * Default constructor.
   */
  function __construct() {

  }

  public function getNode() {
    return $this->node;
  }

  public function setNode($node) {
    $this->node = $node;
  }

  public function getTask() {
    return $this->task;
  }

  public function setTask(TumblerConnectBatchInterface $task) {
    $this->task = $task;
  }

}

/**
 * Represents database access for batch tasks.
 */
class DbTumblerConnectBatch {

  /**
   * Default constructor.
   */
  function __construct() {

  }

  /**
   * Inserts one batch task into database.
   *
   * @param $batch_command
   *   batch data object
   * @return
   *   batch object with id field populated (from database).
    */
  public function schedule(TumblerConnectBatchInterface $batch_command) {
    $state = NULL;
    try {
      $state = $this->determineScheduleState($batch_command);
      switch ($state) {
        case TumblrConnectBatch::STATE_ADD_ACTION :
          $this->add($batch_command);
          break;
        case TumblrConnectBatch::STATE_NO_ACTION_NEEDED :

          break;
        case TumblrConnectBatch::STATE_REPLACE_ACTION :
          $this->update($batch_command);
          break;
        case TumblrConnectBatch::STATE_REMOVE_ALL_RECORDS_FOR_NODE :
          $this->delete($batch_command->getNid());
          break;
      }
    } catch (Exception $e) {
      watchdog('ERROR', 'An unexpected exception occured during tumblr connect batch schedule. Details: ' . $e->getMessage());
    }
  }

  /**
   * Inserts one batch task into database .
   *
   * @param $batch_command
   *   batch data object
   *
   * @return
   *   batch object with id field populated (from database).
   *
   * @throws TumblrConnectDatabaseException
   */
  public function add(TumblerConnectBatchInterface $batch_command) {
    // drupal write needs access to all attributes
    // object fields are protected
    // thus we convert the object to an array
    $obj_array = $batch_command->toArray();
    try {
      $id = db_insert('tumblr_connect_batch')
        ->fields($batch_command->toArray())
        ->execute();
      $batch_command->setId($id);
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to add tumblr connect batch record'));
    }
    return $batch_command;
  }

  /**
   * Updates one batch task in database.
   *
   * @param $batch_command
   *   batch data object
   *
   * @return
   *   batch object
   *
   * @throws TumblrConnectDatabaseException
   */
  public function update(TumblerConnectBatchInterface $batch_command) {
    // drupal write needs access to all attributes
    // object fields are protected
    // thus we convert the object to an array
    $obj_array = $batch_command->toArray();
    try {
      db_update('tumblr_connect_batch')
        ->fields($obj_array)
        ->condition('id', $batch_command->getId())
        ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to update tumblr connect batch record'));
    }
    return $batch_command;
  }

  /**
   * Deletes batch task from database based on nid field.
   *
   * @param $nid
   *   node id
   *
   * @return
   *   true (1) if successful
   *
   * @throws TumblrConnectDatabaseException
   */
  public function delete($nid) {
    try {
      $result = db_delete('tumblr_connect_batch')
        ->condition('nid', $nid)
        ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to delete tumblr connect batch record'));
    }
    return $result;
  }
  
  /**
   * Deletes batch task from database based on id field.
   *
   * @param $nid
   *   node id
   *
   * @return
   *   true (1) if successful
   *
   * @throws TumblrConnectDatabaseException
   */
  public function deleteByTaskId($id) {
    try {
      $result = db_delete('tumblr_connect_batch')
              ->condition('id', $id)
              ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to delete tumblr connect batch record'));
    }
    return $result;
  }

  /**
   * Get batch record. Adds new record when none exists.
   *
   * @param TumblerConnectBatchInterface $batch
   *
   * @throws TumblrConnectDatabaseException
   */

  public function getOrAdd(TumblerConnectBatchInterface $batch) {

    $return_batch = NULL;
    try {
      $count = db_query("SELECT COUNT(id) FROM {tumblr_connect_batch} where nid = :nid and vid = :vid", array(':nid' => $batch->getNid(), ':vid' => $batch->getVid()))->fetchCol();
      if ($count[0] == 0) {
        $return_batch = $this->add($batch);
      }
      else {
        $return_batch = $batch;
        $result = db_query("SELECT id, operation FROM {tumblr_connect_batch} where nid = :nid and vid = :vid", array(':nid' => $batch->getNid(), ':vid' => $batch->getVid()))->fetchObject();
        $return_batch->setOperation($result->operation);
        $return_batch->setId($result->id);
      }
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to process (Get-Add) batch record'));
    }
    return $return_batch;
  }


   /**
   * Gets all instances of tumblr batch command tasks.
   *
   * @throws TumblrConnectDatabaseException
   */
  public function getAllScheduled() {
    $list = NULL;

    $sql = 'SELECT id, nid, vid, post_id, operation FROM {tumblr_connect_batch} ';
    $result = db_query($sql);

    if ($result) {
      $list = array();
      while ($obj = $result->fetchObject()) {
        $node_task = new TumblrConnectNodeBatch();
        $node = node_load($obj->nid, $obj->vid);

        $node_task->setNode($node);

        $task = new TumblrConnectBatch();
        $task->setId($obj->id);
        $task->setNid($obj->nid);
        $task->setVid($obj->vid);
        $task->setOperation($obj->operation);
        $task->setPostId($obj->post_id);

        $node_task->setTask($task);
        $list[] = $node_task;
      }
      // add the delete tasks.
      $list = $this->getScheduledDeletes($list);
    }
    else {
      throw new TumblrConnectDatabaseException(t('Unable to get scheduled tumblr connect batch records'));
    }
    return $list;
  }

  /**
   * Gets all instances of tumblr batch delete command tasks.
   *
   * @param array $list
   *
   * @return array
   *  List with any addional delete command tasks.
   *
   * @throws TumblrConnectDatabaseException
   */
  public function getScheduledDeletes($list) {
    $result = db_query('SELECT id,  nid, vid, post_id, operation FROM {tumblr_connect_batch} WHERE operation = :operation', array(':operation' => TumblrConnectBatch::DELETE));

    if ($result) {
      while ($obj = $result->fetchObject()) {
        $node_task = new TumblrConnectNodeBatch();

        $node = new stdClass;
        $node->nid = $obj->nid;
        $node->vid = $obj->vid;


        $node_task->setNode($node);

        $task = new TumblrConnectBatch();
        $task->setId($obj->id);
        $task->setNid($obj->nid);
        $task->setVid($obj->vid);
        $task->setOperation($obj->operation);
        $task->setPostId($obj->post_id);

        $node_task->setTask($task);
        $list[] = $node_task;
      }
    }
    else {
      throw new TumblrConnectDatabaseException(t('Unable to get scheduled tumblr connect batch records -- delete command'));
    }
    return $list;
  }


  /*
   * Determines which operation (add, modify or delete) must be performed during batch operation.
   *
   * @return
   *    Add, Modify or Delete
   *
   * @throws TumblrConnectDatabaseException
   *
   * Logically we should only have one batch record for each node (revision). For audit puroposes
   * we will allow the database to store duplicates. We'll throw a warning message.
   *
   * The main use case for our batch update subsystem is: The remote tumblr service is unavailable during the
   * creation, modification or deletion of our local Drupal node content. So we need to perform some business logic
   * here.
   *
   * For example, if somone locally creates then modifies a Drupal node while tumblr is unavailable. Logically, when
   * tumblr becomes available, we want to avoid sending 2 requests to tubmlr. In this case, all we need to do is ADD to
   * tumblr. No need to add, then modify the same content.
   *
   * This method compares the current tumblr batch record (if any) with the new command. This methods determines the
   * current state and performs the necessary database operations.
   */

  function determineScheduleState(TumblerConnectBatchInterface $batch_command) {

    $new_operation = $batch_command->getOperation();
    $current_operation = TumblrConnectBatch::STATE_NO_CURRENT_RECORD;

    if ($new_operation != TumblrConnectBatch::ADD) {
      try {
        $existing_record = $this->getOrAdd($batch_command);
        $current_operation = $existing_record->getOperation();
      } catch (TumblrConnectDatabaseException $e) {
        throw $e;
      }
    }

    // Now lets compare the new and current records
    // We will determine which state we are in
    // default to
    $state = TumblrConnectBatch::STATE_ADD_ACTION;
    switch ($new_operation) {
      case TumblrConnectBatch::ADD :
        $state = $this->scheduleAdd($current_operation);
        break;
      case TumblrConnectBatch::UPDATE :
        $state = $this->scheduleMod($current_operation);
        break;
      case TumblrConnectBatch::DELETE :
        $state = $this->scheduleDelete($current_operation);
        break;
    }
    return $state;
  }

  /**
   * Processes a schedule add operation.
   *
   * @param string $current_op
   *   Operation name as defined by the .TumblrConnectBatch constants.
   *
   * @return
   *   Operation name as defined by the .TumblrConnectBatch constants.
   */
  function scheduleAdd($current_op) {
    switch ($current_op) {
      case TumblrConnectBatch::STATE_NO_CURRENT_RECORD :
        $state = TumblrConnectBatch::STATE_ADD_ACTION;
        break;
      case TumblrConnectBatch::ADD :
        $state = TumblrConnectBatch::STATE_NO_ACTION_NEEDED;
        break;
      case TumblrConnectBatch::UPDATE :
        $state = TumblrConnectBatch::STATE_REPLACE_ACTION;
        break;
      case TumblrConnectBatch::DELETE :
        $state = TumblrConnectBatch::STATE_REPLACE_ACTION;
        break;
    }
    return $state;
  }

  /**
   * Processes a schedule modify operation.
   *
   * @param string $current_op
   *   Operation name as defined by the .TumblrConnectBatch constants.
   *
   * @return
   *   Operation name as defined by the .TumblrConnectBatch constants.
   */
  function scheduleMod($current_op) {
    switch ($current_op) {
      case TumblrConnectBatch::STATE_NO_CURRENT_RECORD :
        $state = TumblrConnectBatch::STATE_ADD_ACTION;
        break;
      case TumblrConnectBatch::ADD :
        $state = TumblrConnectBatch::STATE_NO_ACTION_NEEDED;
        break;
      case TumblrConnectBatch::UPDATE :
        $state = TumblrConnectBatch::STATE_REPLACE_ACTION;
        break;
      case TumblrConnectBatch::DELETE :
        $state = TumblrConnectBatch::STATE_REPLACE_ACTION;
        break;
    }
    return $state;
  }

  /**
   * Processes a schedule delete operation.
   *
   * @param string $current_op
   *   Operation name as defined by the .TumblrConnectBatch constants.
   *
   * @return
   *   Operation name as defined by the .TumblrConnectBatch constants.
   */
  function scheduleDelete($current_op) {
    switch ($current_op) {
      case TumblrConnectBatch::STATE_NO_CURRENT_RECORD :
        $state = TumblrConnectBatch::STATE_ADD_ACTION;
        break;
      case TumblrConnectBatch::ADD :
        $state = TumblrConnectBatch::STATE_REMOVE_ALL_RECORDS_FOR_NODE;
        break;
      case TumblrConnectBatch::UPDATE :
        $state = TumblrConnectBatch::STATE_REMOVE_ALL_RECORDS_FOR_NODE;
        break;
      case TumblrConnectBatch::DELETE :
        $state = TumblrConnectBatch::STATE_ADD_ACTION;
        break;
    }
    return $state;
  }

}