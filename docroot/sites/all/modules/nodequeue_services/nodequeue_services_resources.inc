<?php

function _nodequeue_services_resource_retrieve($qid) {
  $queue = nodequeue_load($qid);
  return $queue;
}

function _nodequeue_services_resource_create($data) {
  $queue = (object) $data;

  $qid = _nodequeue_services_resource_save($queue);

  return (object)array(
    'qid' => $qid,
    'uri' => services_resource_uri(array('nodequeue', $qid)),
  );
}

function _nodequeue_services_resource_update($id, $data) {
  $queue = nodequeue_load($id);

  $queue = (object) array_merge((array) $queue, (array) $data);

  $qid = _nodequeue_services_resource_save($queue);

  return (object)array(
    'qid' => $qid,
    'uri' => services_resource_uri(array('nodequeue', $qid)),
  );
}

function _nodequeue_services_resource_delete($qid) {
  nodequeue_delete($qid);

  return (object) array(
    'qid' => $qid,
  );
}

function _nodequeue_services_resource_index() {
  $nodequeues = db_select('nodequeue_queue', 'nq')
    ->fields("nq")
    ->orderBy('qid', 'ASC')
    ->execute()
    ->fetchAll();

  return $nodequeues;
}

function _nodequeue_services_resource_save($queue) {
  if (!isset($queue->title)) {
    return services_error('Missing nodequeue title', 406);
  }
  if (!isset($queue->name)) {
    return services_error('Missing nodequeue name', 406);
  }

  if (!isset($queue->size)) {
    $queue->size = 0;
  }
  if (!isset($queue->owner)) {
    $queue->owner = 'nodequeue';
  }

  // Enabled content types
  if (!isset($queue->types)) {
    $queue->types = node_type_get_names();
  }

  // Fix checkboxes.
  $queue->roles = array_keys(array_filter($queue->roles));
  $queue->types = array_keys(array_filter($queue->types));

  // Remove roles that have the 'manipulate all queues' permission.
  $manipulate_all_queues = array_keys(user_roles(FALSE, 'manipulate all queues'));
  $queue->roles = array_diff($queue->roles, $manipulate_all_queues);

  if (!isset($queue->qid)) {
    $queue->new = TRUE;
  }

  if (!isset($queue->subqueue_title)) {
    $queue->subqueue_title = '';
  }

  // Modify show_in_links based on whether or not links are available.
  $queue->show_in_links = !empty($queue->link) || !empty($queue->link_remove);

  if (!isset($queue->show_in_ui)) {
    $queue->show_in_ui = true;
  }
  if (!isset($queue->show_in_tab)) {
    $queue->show_in_tab = true;
  }

  nodequeue_api_queue_form_submit($queue, $form_state);

  $qid = nodequeue_save($queue); // sets $queue->qid if needed.

  nodequeue_api_queue_form_submit_finish($queue, $form_state);

  return $qid;
}