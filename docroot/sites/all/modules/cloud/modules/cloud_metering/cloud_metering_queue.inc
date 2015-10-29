<?php

/**
 * @file
 * Contains the Queue handling methods used by the Cloud Metering module
 * Works with Cloud Metering module
 *
 * Copyright (c) 2011 DOCOMO Innovations, Inc.
 */

/**
 *  Implementation of hook_cron_queue_info()
 */
function cloud_metering_cron_queue_info() {
  $queues[CLOUD_METERING_QUEUE] = array(
    'worker callback' => '_cloud_metering_process_queue',
    'time' => 600,
  );
  return $queues;
}

/**
 *  Queue worker callback
 */
function _cloud_metering_process_queue() {
  $next_task = _cloud_metering_next_task_to_execute();
  if (count($next_task)) {
    if ($next_task['task'] == CLOUD_METERING_BACKUP) {
      _cloud_metering_backup($next_task);
    }
    elseif ($next_task['task'] == CLOUD_METERING_TERMINATE) {
      _cloud_metering_terminate($next_task);
    }
    elseif ($next_task['task'] == CLOUD_METERING_BACKUP_CHECK) {
      _cloud_metering_check_backup($next_task);
    }
    elseif ($next_task['task'] == CLOUD_METERING_TERMINATE_CHECK) {
      _cloud_metering_check_terminate($next_task);
    }
  }
}

/**
 *  Add item to Drupal queue
 */
function _cloud_metering_add_queue_item($item) {
  $queue = DrupalQueue::get(CLOUD_METERING_QUEUE);
  $queue->createItem($item);
}

/**
 *  Get the uuid
 */
function _cloud_metering_get_uuid($cloud_context, $instance_id) {
  return $cloud_context . '-' . $instance_id;
}

/**
 *  Check task progress and add new tasks if needed
 */
function _cloud_metering_check_task($task) {
  $qid  = $task['qid'];
  $data = $task['data'];
  $cloud_context = $data['cloud_context'];
  $instance_id = $data['instance_id'];

  switch ($task['task']) {
    case CLOUD_METERING_BACKUP:
      $volumes = $data['volumes'];
      $snapshots_complete = array();
      foreach ($volumes as $volume_id => $volume) {
        if (isset($volume['snapshot_id'])) {
          $snapshot_id = $volume['snapshot_id'];
          // check if volumes are backedup
          $snapshot_complete = cloud_perform_action('', 'check_snapshot_completion', $cloud_context, array('cloud_context' => $cloud_context, 'snapshot_id' => $snapshot_id));
          if ($snapshot_complete) {
            $snapshots_complete[] = $snapshot_id;
          }
        }
      }
      if ((count($volumes) == count($snapshots_complete))) {
        // Backup is complete. We mark the backup task as complete and initiate terminate
        _cloud_metering_update_task_status($qid, CLOUD_METERING_QUEUE_STATUS_COMPLETED, array());
        // Start detach task, send in the volume information already in $data
        _cloud_metering_add_task($cloud_context, $instance_id, CLOUD_METERING_DETACH, $data);
      }
      break;
    case CLOUD_METERING_TERMINATE:
      // Check if the instance is terminated
      $terminated = cloud_perform_action('', 'check_instance_terminated', $cloud_context, array('cloud_context' => $cloud_context, 'instance_id' => $instance_id));
      if ($terminated) {
        // Termination complete
        _cloud_metering_update_task_status($qid, CLOUD_METERING_QUEUE_JOB_COMPLETED, array());
      }
      break;
  }
}

/**
 *  Backup instance volumes
 */
function _cloud_metering_backup($task) {

  $qid = $task['qid'];
  $data = $task['data'];
  $volumes = $data['volumes'];
  $cloud_context = $data['cloud_context'];
  $instance_id = $data['instance_id'];

  // Create snapshots for each volume
  foreach ($volumes as $volume_id => $volume) {

    $params = array(
      'volume_id' => $volume_id,
      'snapshot_name' => 'Auto-Snapshot ' . date('YmdHis'),
      'snapshot_description' => t('Snapshot created before terminating instance as instance crossed the allocated budget'),
    );

    $snapshot_id = cloud_perform_action('', 'backup', $cloud_context, $params);

    if ($snapshot_id) {
      $volumes[$volume_id]['snapshot_id'] = (string)$snapshot_id;
    }
    else {
      // TODO -- Decide on steps to be taken in case snapshot creation fails
    }

  }
  $data['volumes'] = $volumes;

  // The backup task will be on a wait as we keep monitoring for the backup to complete
  _cloud_metering_update_task_status($qid, CLOUD_METERING_QUEUE_STATUS_IN_PROGRESS, $data);
}

/**
 *  Terminate instance
 */
function _cloud_metering_terminate($task) {
  $qid = $task['qid'];
  $data = $task['data'];
  $cloud_context = $data['cloud_context'];
  $instance_id = $data['instance_id'];

  // Start termination of instance
  cloud_perform_action('', 'terminate', $cloud_context, array($instance_id));
  _cloud_metering_activity_audit(t('Terminated instance %instance-id because budget was exceeded', array('%instance-id' => $instance_id)));  
  
  // Update the task status. Terminate task will be waiting for the termination to complete.
  _cloud_metering_update_task_status($qid, CLOUD_METERING_QUEUE_STATUS_IN_PROGRESS);
}

/**
 *  Check backup task. If completed then mark as completed. Otherwise start a new check task.
 *  If backups completed, go straight for terminate.  Let EC2 do the volume detaching
 */
function _cloud_metering_check_backup($task) {
  $qid = $task['qid'];
  $data = $task['data'];
  $b_qid = $data['qid'];
  $cloud_context = $data['cloud_context'];
  $instance_id = $data['instance_id'];
  $volumes = $data['volumes'];
  $snapshots_complete = array();

  foreach ($volumes as $volume_id => $volume) {
    $snapshot_id = $volume['snapshot_id'];
    $snapshot_complete = cloud_perform_action('', 'check_snapshot_completion', $cloud_context, array('snapshot_id' => $snapshot_id));
    if ($snapshot_complete) {
      $snapshots_complete[] = $snapshot_id;
    }
  }

  if (sizeof($volumes) == sizeof($snapshots_complete)) {
    // Backup of all volumes completed
    _cloud_metering_update_task_status($b_qid, CLOUD_METERING_QUEUE_STATUS_COMPLETED, array());
    // go straight to terminate. Let EC2 detach the volumes
    _cloud_metering_add_task($cloud_context, $instance_id, CLOUD_METERING_TERMINATE, $data);
  }
  else {
    $backup_task = _cloud_metering_get_task($b_qid);
    if ($backup_task['time_running'] > 10) {
      _cloud_metering_update_task_status($b_qid, CLOUD_METERING_QUEUE_STATUS_FAILED, array());
      _cloud_metering_activity_audit(t('Task with %qid failed due to expiry of allocated time.', array('%qid' => $b_qid)), '', WATCHDOG_ERROR);
    }
  }
  // The current check task is completed
  _cloud_metering_update_task_status($qid, CLOUD_METERING_QUEUE_STATUS_COMPLETED, array());
}

/**
 *  Monitor termination of instances
 */
function _cloud_metering_check_terminate($task) {
  $qid = $task['qid'];
  $data = $task['data'];
  $t_qid = $data['qid'];
  $cloud_context = $data['cloud_context'];
  $instance_id = $data['instance_id'];

  // Check if instance has got terminated
  $terminated = cloud_perform_action('', 'check_instance_terminated', $cloud_context, array('instance_id' => $instance_id));

  if ($terminated) {
    // Termination task completed
    _cloud_metering_update_task_status($t_qid, CLOUD_METERING_QUEUE_STATUS_COMPLETED, array());
  }
  else {
    $t_task = _cloud_metering_get_task($t_qid);
    if ($t_task['time_running'] > 10) {
      _cloud_metering_update_task_status($t_qid, CLOUD_METERING_QUEUE_STATUS_FAILED, array());
      _cloud_metering_activity_audit(t('Task with Id %qid failed due to expiry of allocated time.', array('%qid' => $t_qid)), 'user_activity', '', WATCHDOG_ERROR);
    }
  }
  // This task is complete
  _cloud_metering_update_task_status($qid, CLOUD_METERING_QUEUE_STATUS_COMPLETED, array());
}

/**
 *  Get the base tasks - backup, detach, terminate
 */
function _cloud_metering_get_base_tasks() {
  return array(CLOUD_METERING_BACKUP, CLOUD_METERING_TERMINATE);
}

/**
 *  Get checker tasks - backup check, detach check, terminate check
 */
function _cloud_metering_get_checker_tasks() {
  return array(CLOUD_METERING_BACKUP_CHECK, CLOUD_METERING_TERMINATE_CHECK);
}

/**
 *  Get the type of a task
 *  @param String task
 *  @return String "base" or "checker"
 */
function _cloud_metering_get_task_type($task) {
  $base_tasks = _cloud_metering_get_base_tasks();
  $checker_tasks = _cloud_metering_get_checker_tasks();
  if (in_array($task, $base_tasks)) {
    return 'base';
  }
  if (in_array($task, $checker_tasks)) {
    return 'checker';
  }
  return 'base';
}
