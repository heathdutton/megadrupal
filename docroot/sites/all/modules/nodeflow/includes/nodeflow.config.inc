<?php
/**
 * @file
 * Configuration functions for the nodeflow module.
 */

/**
 * Display confirm form for deleting a nodeflow queue.
 * 
 * @param int $queue_id
 */
function nodeflow_delete_queue_confirm($form_state, $queue_id) {
  $form['nodeflow_queue_id'] = array(
    '#type' => 'value',
    '#value' => $queue_id
  );

  return confirm_form($form, t('Are you sure you want to delete this queue?'), 'admin/structure/nodeflow');
}

/**
 * Process queue deletion confirmation form.
 * 
 * @param type $form
 * @param type $form_state
 */
function nodeflow_delete_queue_confirm_submit($form, &$form_state) {
  
  if ($form_state['values']['confirm']) {
    $queue_id = abs((int) $form_state['values']['nodeflow_queue_id']['build_info']['args'][0]);
    if ($queue_id > 0) {
      db_transaction();
      
      db_delete('nodeflow')
        ->condition('nfid', $queue_id)
        ->execute();
      
      db_delete('nodeflow_queues')
        ->condition('nfid', $queue_id)
        ->execute();
      
      drupal_set_message(t('The nodeflow queue was deleted.'));
    } else {
      drupal_set_message(t('Unable to delete nodeflow queue as form value was not an integer.'), 'error');
    }
  }
  
  drupal_goto('admin/structure/nodeflow');
}

/**
 * Add new row into nodeflow_queues table form.
 * 
 * @return drupal form array
 */
function nodeflow_manage_queues() {
  $form['nodeflow_name'] = array(
    '#title' => t('Queue name'),
    '#type' => 'textfield',
    '#description' => t('The human-readable label of this nodeflow queue.')
  );
  $form['nodeflow_weight'] = array(
    '#title' => t('Queue weight'),
    '#type' => 'textfield',
    '#default_value' => 0,
    '#description' => t('The queues are displayed sorted by this weight order.')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  
  return $form;
}

/**
 * Validate nodeflow creation form.
 */
function nodeflow_manage_queues_validate($form, &$state) {
  //
  if (empty($state['values']['nodeflow_name'])) {
    form_set_error('nodeflow_name', t('Please enter in a name.'));
  }
  if (!is_numeric($state['values']['nodeflow_weight'])) {
    form_set_error('nodeflow_weight', t('Please set a valid weight number.'));
  }
}

/**
 * Handle nodeflow creation form submission.
 */
function nodeflow_manage_queues_submit($form, &$state) {
  $name = $state['values']['nodeflow_name'];
  $weight = $state['values']['nodeflow_weight'];
  $queue = new \Nodeflow\Db\NodeflowQueue(t($name), $weight);
  $queue_id = $queue->save();
  if ($queue_id) {
    drupal_goto('admin/structure/nodeflow');
  } else {
    drupal_set_message(t('There was an error creating the nodeflow queue.'), 'error');
  }
}

/**
 * Configuration page for nodeflow queues. Add and delete queues here.
 * 
 * @return array
 */
function nodeflow_configuration_page() {
  /* @TODO move into relevant object */
  $queues = db_select('nodeflow_queues', 'n')
            ->fields('n')
            ->orderby('weight', 'ASC')
            ->execute()
            ->fetchAll();

  if (!empty($queues)) {
    $rows = array();
    foreach ($queues as $q) {
      $row = array();
      $row[] = $q->name;
      $row[] = $q->weight;
      $row[] =  l(t('View'), 'admin/nodeflow/' . $q->nfid);
      if (user_access('administer nodeflow')) {
        $row[] =  l(t('Delete'), 'admin/nodeflow/delete/' . $q->nfid);
      }
      $rows[] = $row;
    }

    $table = array(
      'header' => array(
        array('data' => 'Queue name'), 
        array('data' => 'Weight'), 
      	array('data' => 'Operations', 'colspan' => 2)
      ), 
      'rows' => $rows
    );
  }
  else {
    $table = array();
  }

  $toplink = '<ul class="action-links"><li>' . 
  	     l(t('Add a nodeflow queue'), 'admin/structure/nodeflow/add') . 
	     '</li></ul>';

  return array('#markup' => $toplink . theme('table', $table));
}

/**
 * Callback function to populate the nodeflow table when the table is empty.
 */
function nodeflow_populate($queue_id = NULL) {
  $nodes = db_query_range("SELECT nid FROM {node} WHERE status=1 AND type='article' ORDER BY nid DESC", 0, 5);
  if ($nodes) {
    $pos = 0;
    
    if (NULL == $queue_id) {
      // Create new nodeflow_queues record
      $queue = new \Nodeflow\Db\NodeflowQueue(t('Nodeflow 1'));
      $queue_id = $queue->save();
    }
    
    foreach ($nodes as $row) {
      if (!is_null($row->nid)) {
        // Create new nodeflow order record.  
        $record = new \Nodeflow\Db\Nodeflow($queue_id, $row->nid, $pos++);
        $record->save();
      }
    }
    
    drupal_set_message(t('The nodeflow table has been successfully populated with @pos nodes', array('@pos' => $pos)));
  }
  else {
    drupal_set_message(t('No nodes found with published status. Please ensure you have atleast 1 node published to populate.'), 'error');
  }
  
  drupal_goto('admin/nodeflow/' . $queue_id);
}
