<?php
/**
 * @file
 * Rules implementation for nodeflow.
 */

/**
 * Implements hook_rules_event_info().
 */
function nodeflow_rules_event_info() {
  return array(
    'nodeflow_save' => array(
      'module' => 'nodeflow',
      'group' => t('Nodeflow'),
      'label' => t('When saving a nodeflow list'),
      'variables' => array(
        'queue_id' => array(
          'type' => 'text'
        )
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info()
 */
function nodeflow_rules_condition_info() {
  return array(
    'nodeflow_is_queue' => array(
      'module' => 'nodeflow',
      'group' => t('Nodeflow'),
      'label' => t('Is nodeflow queue'),
      'parameter' => array(
        'queue_id' => array (
          'type' => 'text',
          'label' => t('Nodeflow queue'),
          'options list' => 'get_nodeflow_queues'
        )
      )
    )
  );
}

/**
 * Return list of named nodeflow queues for use in rules conditions.
 *
 * @return array
 */
function get_nodeflow_queues() {
  $q = db_query("select nfid,name from {nodeflow_queues} q order by q.nfid");
  $select = array();
  foreach ($q as $row) {
    $select[$row->nfid] = $row->name;
  }
  return $select;
}

/**
 * Determine whether the condition 'saved nodeflow queue, is nodeflow queue'.
 *
 * @return boolean
 */
function nodeflow_is_queue($queue_id, $all, $rules_state) {
  return $queue_id == $rules_state->variables['queue_id'];
}

/**
 * Implements hook_rules_action_info().
 */
function nodeflow_rules_action_info() {
  return array(
    'nodeflow_insert_node' => array(
      'group' => t('Nodeflow'),
      'label' => t('Add node to nodeflow'),
      'base' => 'nodeflow_insert_into_queue',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('The node being saved')
        ),
        'nodeflow_queue_id' => array(
          'type' => 'text',
          'label' => t('Nodeflow queue'),
          'description' => t('Select the nodeflow queue to insert into'),
          'options list' => 'get_nodeflow_queues'
        ),
        'nodeflow_position' => array(
          'type' => 'text',
          'label' => t('Position in the nodeflow queue'),
          'description' => t('Enter in which position to insert the node within the queue (as integer).'),
          'default value' => 0
        )
      )
    ),
    'nodeflow_rules_add_queue' => array(
      'label'     => t('Add a nodeflow queue'),
      'group'     => t('Nodeflow'),
      'parameter' => array(
        'name'   => array(
          'type'         => 'text',
          'label'        => t('Queue name'),
          'description'  => t('The name of the nodeflow queue. Select the title of the node or taxonomy term for example.'),
          'default mode' => 'selector',
        ),
        'weight' => array(
          'type'        => 'integer',
          'label'       => t('Queue weight'),
          'description' => t('The weight of the nodeflow queue. This is used for sorting the queue.'),
        ),
      ),
      'provides' => array(
        'nodequeue_id_created' => array(
          'type'  => 'integer',
          'label' => t('Created nodeflow queue'),
          'save'  => TRUE,
        ),
      ),
    ),
  );
}
