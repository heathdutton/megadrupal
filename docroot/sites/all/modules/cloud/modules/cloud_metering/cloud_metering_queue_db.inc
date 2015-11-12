<?php

/**
 * @file
 * Contains the DB CRUD methods for Queue handling used by the Cloud Metering module
 * Works with Cloud Metering module
 *
 * Copyright (c) 2011 DOCOMO Innovations, Inc.
 *
 */

/**
 *  Add a task
 */
function _cloud_metering_add_task($cloud_context, $instance_id, $task, $data = array()) {

  $uuid      = _cloud_metering_get_uuid($cloud_context, $instance_id);
  $task_type = _cloud_metering_get_task_type($task);

  if (!_cloud_metering_uuid_in_queue($uuid, $task_type)) {

    $data['cloud_context'] = $cloud_context;
    $data['instance_id'] = $instance_id;

    $result = db_insert(CLOUD_METERING_QUEUE_TABLE) 
      ->fields(array(
      'task' => $task,
      'uuid' => $uuid,
      'created' => time(),
      'updated' => time(),
      'status' => CLOUD_METERING_QUEUE_STATUS_INITIATED,
      'uid' => 1,
      'data' => serialize($data),
    ))
      ->execute();


    if ($result) {
      // Add item to queue
      _cloud_metering_add_queue_item($task);
      _cloud_metering_activity_audit(t('@task task initiated for instance %instance_id', array('@task' => ucfirst($task), '%instance_id' => $instance_id)), 'user_activity');
    }

  }

}

/**
 *  Check if a base task is running on an instance
 *  @param String uuid
 *                      The unique id of associated with a task
 *  @param String type
 *                      The type of task to check - base or checker
 */
function _cloud_metering_uuid_in_queue($uuid, $type = 'base') {

  $tasks_to_check = ($type == 'base' ? _cloud_metering_get_base_tasks() : _cloud_metering_get_checker_tasks());

  $query = "SELECT COUNT(uuid) AS added FROM {" . CLOUD_METERING_QUEUE_TABLE . "}
              WHERE uuid = :uuid AND status IN ('" . CLOUD_METERING_QUEUE_STATUS_INITIATED . "', '" . CLOUD_METERING_QUEUE_STATUS_IN_PROGRESS . "')
              AND task IN ( :tasks )";

  $params = array(
    ':uuid' => $uuid,
    ':tasks' => $tasks_to_check,
  );

  $count  = db_query($query, $params)->fetchField();
  return ($count ? TRUE : FALSE);
}

/**
 *  Get the next queue task to execute
 */
function _cloud_metering_next_queue_task() {

  $query = 'SELECT * FROM {' . CLOUD_METERING_QUEUE_TABLE . "} WHERE status = :status ORDER BY created";

  $result = db_query($query, array(':status' => CLOUD_METERING_QUEUE_JOB_INITIATED) );

  $task = array();

  foreach ($result as $o) {

    $task['qid']       = $o->qid;
    $task['task']      = $o->task;
    $task['uuid']      = $o->uuid;
    $task['timestamp'] = $o->timestamp;
    $task['status']    = $o->status;
    $task['uid']       = $o->uid;

    return $task;
  }

  return $task;
}

/**
 *  Update status of queue item
 */
function _cloud_metering_update_task_status($qid, $status, $data = NULL) {


  $query_update = db_update(CLOUD_METERING_QUEUE_TABLE);
  $fields = array();
  
  $fields['status']  = $status;
  $fields['updated'] = time();
  
  if (is_array($data)) {
    $data = (sizeof($data) > 0) ? serialize($data) : '';
    $fields['data'] = $data;
  }

  $query_update->fields($fields);
  $result = $query_update
                ->condition('qid', $qid, '=')
                ->execute();

  return ($result ? TRUE : FALSE);
}

/**
 * Get the task in progress
 */
function _cloud_metering_next_task_in_progress() {

  $query = 'SELECT * , TIMESTAMPDIFF(MINUTE, FROM_UNIXTIME(updated), CURRENT_TIMESTAMP) time_running FROM {' . CLOUD_METERING_QUEUE_TABLE . "} WHERE status = :status ORDER BY created ASC LIMIT 0, 1";
  $db_result = db_query($query, array(':status' => CLOUD_METERING_QUEUE_STATUS_IN_PROGRESS) );

  $task = array();

  foreach ($db_result as $o) {

    $task['qid']          = $o->qid;
    $task['task']         = $o->task;
    $task['uuid']         = $o->uuid;
    $task['created']      = $o->created;
    $task['updated']      = $o->updated;
    $task['status']       = $o->status;
    $task['uid']          = $o->status;
    $task['data']         = unserialize($o->data);
    $task['time_running'] = $o->time_running;

  }

  return $task;

}

/**
 *  Check if a task of specific type is running
 */
function _cloud_metering_task_running($cloud_context, $instance_id, $task) {

  $query = "SELECT COUNT(qid) AS task_count FROM {" . CLOUD_METERING_QUEUE_TABLE . "} WHERE uuid = :uuid AND task = :task AND status IN ('" . CLOUD_METERING_QUEUE_STATUS_INITIATED . "', '" . CLOUD_METERING_QUEUE_STATUS_IN_PROGRESS . "')";

  $result = db_query($query, array(
    ':uuid' => _cloud_metering_get_uuid($cloud_context, $instance_id),
    ':task' => $task,
  ) )
              ->fetchField();

  return ($result ? TRUE : FALSE);
}

/**
 *  Get next base task to execute
 *  @return
 *          The next task to execute. Next task is either the oldest initiated base task,
 *          or the oldest initiated checker task
 */
function _cloud_metering_next_task_to_execute() {

  $base_tasks    = _cloud_metering_get_base_tasks();
  $checker_tasks = _cloud_metering_get_checker_tasks();

  $query = 'SELECT * FROM {' . CLOUD_METERING_QUEUE_TABLE . "} WHERE status = '" . CLOUD_METERING_QUEUE_STATUS_INITIATED . "' ORDER BY created ASC LIMIT 0, 1";

  $db_result = db_query($query);

  $task = array();

  foreach ($db_result as $o) {

    $task['qid']          = $o->qid;
    $task['task']         = $o->task;
    $task['uuid']         = $o->uuid;
    $task['created']      = $o->created;
    $task['updated']      = $o->updated;
    $task['status']       = $o->status;
    $task['uid']          = $o->status;
    $task['data']         = unserialize($o->data);

  }

  return $task;

}

/**
 *  Get a task from queue database table
 */
function _cloud_metering_get_task($qid) {
  $query = 'SELECT *, TIMESTAMPDIFF(MINUTE, FROM_UNIXTIME(updated), CURRENT_TIMESTAMP) time_running FROM {' . CLOUD_METERING_QUEUE_TABLE . '} WHERE qid = :qid ';
  $db_result = db_query($query, array(':qid' => $qid) );

  $task = array();
  
  foreach ($db_result as $o) {
    $task['qid']          = $o->qid;
    $task['task']         = $o->task;
    $task['uuid']         = $o->uuid;
    $task['created']      = $o->created;
    $task['updated']      = $o->updated;
    $task['status']       = $o->status;
    $task['uid']          = $o->status;
    $task['data']         = unserialize($o->data);
    $task['time_running'] = $o->time_running;
  }
  return $task;
}

/**
 *  Get the tasks having status "in progress" and have run for atleast a minute
 */
function _cloud_metering_get_in_progress_tasks($type = 'base') {

  $task_types = ($type == 'base' ? _cloud_metering_get_base_tasks() : _cloud_metering_get_checker_tasks());
  $tasks = array();
  $query = db_select(CLOUD_METERING_QUEUE_TABLE, 'c')
    ->fields('c')
    ->condition('c.status', CLOUD_METERING_QUEUE_STATUS_IN_PROGRESS)
    ->condition('c.task', $task_types, 'IN')
    ->where('TIMESTAMPDIFF(MINUTE, FROM_UNIXTIME(c.updated), CURRENT_TIMESTAMP) >= 1')
    ->orderBy('c.created', 'ASC');
  $result = $query->execute();

  return $result->fetchAllAssoc('qid', PDO::FETCH_ASSOC);
}

/**
 *  Remove the complete tasks from table
 */
function _cloud_metering_delete_completed_tasks() {

  $num_deleted = db_delete(CLOUD_METERING_QUEUE_TABLE)
    ->condition('status', CLOUD_METERING_QUEUE_STATUS_COMPLETED)
    ->execute();

}
