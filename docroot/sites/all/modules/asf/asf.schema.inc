<?php
/**
 * @file
 * The database functions and schema.
 */

/**
 * Return a list of actions according to your selection.
 *
 * @param array $options
 *   key value array of action-field and action-value
 */
function asf_select_actions($options) {
  $query = db_select('asf_schedule', 'asf')
      ->fields('asf');
  if (isset($options['aid'])) {
    $query->condition('asf.aid', $options['aid'], '=');
  }
  if (isset($options['nid'])) {
    $query->condition('asf.nid', $options['nid'], '=');
  }
  if (isset($options['nfid'])) {
    $query->condition('asf.nfid', $options['nfid'], '=');
  }
  if (isset($options['action'])) {
    $query->condition('asf.action', $options['action'], '=');
  }
  if (isset($options['status'])) {
    $query->condition('asf.status', $options['status'], '=');
  }
  if (isset($options['time'])) {
    $query->condition('asf.time', $options['time']['value'], $options['time']['operator']);
  }
  if (isset($options['created'])) {
    $query->condition('asf.created', $options['created']['value'], $options['created']['operator']);
  }
  if (isset($options['changed'])) {
    $query->condition('asf.changed', $options['changed']['value'], $options['changed']['operator']);
  }
  return $query->execute()->fetchAll();
}

/**
 * Update an action.
 *
 * @param object $action
 *   an action.
 */
function asf_update_action($action) {
  $num_updated = db_update('asf_schedule')
    ->fields(array(
      'nid' => $action->nid,
      'nfid' => $action->nfid,
      'action' => $action->action,
      'time' => $action->time,
      'status' => $action->status,
      'changed' => REQUEST_TIME,
  ))
  ->condition('aid', $action->aid, '=')
  ->execute();
  return $num_updated;
}

/**
 * Insert an action into the DB.
 *
 * @param object $action
 *   the action object.
 */
function asf_insert_action($action) {
  $aid = db_insert('asf_schedule')
    ->fields(array(
      'nid' => $action->nid,
      'nfid' => $action->nfid,
      'action' => $action->action,
      'time' => $action->time,
      'status' => $action->status,
      'changed' => REQUEST_TIME,
      'created' => REQUEST_TIME,
    ))
  ->execute();
  return $aid;
}

/**
 * Delete an action.
 *
 * @param string $action_id
 *   the id of the action in the schedule table
 */
function asf_delete_action($action_id) {
  $num_deleted = db_delete('asf_schedule')
  ->condition('aid', $action_id)
  ->execute();
  /*
  // Later on deletion should be replaced by de-activation of action.
  $num_updated = db_update('asf_schedule')
  ->fields(array(
  'status' => ASF_STATUS_DELETED,
  'changed' => REQUEST_TIME,
  ))
  ->condition('aid', $action_id, '=')
  ->execute();
  return $num_updated;*/
}

/**
 *  Implements hook_schema().
 */
function asf_schema() {
  $schema['asf_schedule'] = array(
    'description' => 'Advanced scheduled nodes data.',
    'fields' => array(
      'aid' => array(
        'description' => 'The primary identifier for a action.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'nid' => array(
        'description' => 'Tidentifier for a node.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'nfid' => array(
        'description' => 'Tidentifier for a node field.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'action' => array(
        'description' => 'The workflow action that should occur',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'time' => array(
        'description' => 'When the action should occurr.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'status' => array(
        'description' => 'The status of the action.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => 'PEN',
      ),
      'created' => array(
        'description' => 'The Unix timestamp when the schedule was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when the schedule was most recently saved.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
    ),
    'indexes' => array(
      'asf_schedule_nid'         => array('nid'),
      'asf_schedule_aid'         => array('aid'),
      'asf_schedule_action_time' => array('time'),
      'asf_schedule_action_status' => array('status'),
      'asf_schedule_action' => array('action'),
    ),
    'unique keys' => array(
      'asf_id' => array('aid'),
    ),
    'primary key' => array('aid'),
  );
  return $schema;
}
