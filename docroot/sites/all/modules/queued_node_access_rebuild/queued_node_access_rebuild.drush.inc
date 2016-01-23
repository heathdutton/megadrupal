<?php

/**
 * Implementation of hook_drush_command().
 */
function queued_node_access_rebuild_drush_command() {

  $items['queued-node-access-rebuild'] = array(
    'description' => "Queue nodes for node access rebuild.",
    'examples' => array(
      'drush queued-node-access-rebuild',
    ),
    'options' => array(
      'type' => array(
        'description' => 'The content type to queue for rebuild.',
      )
    ),
    'callback' => 'drush_queued_node_access_rebuild',
  );

  return $items;
}

/**
 * Drush callback.
 */
function drush_queued_node_access_rebuild() {
  variable_set('queued_node_access_rebuild_processing', FALSE);
  $types = array();
  $type = drush_get_option('type', NULL);
  if ($type) {
    $types[] = $type;
  }
  $nids = queued_node_access_rebuild_get_nodes($types);
  $count = count($nids);
  $row = 0;

  drush_print(dt('Queueing @count nodes for node access rebuild (1 dot = 10,000 nodes queued).', array('@count' => $count)));

  if (!empty($nids)) {
    // If we are enqueueing all nodes and the current queue isn't empty, delete it.
    if (empty($types)) {
      $queue = DrupalQueue::get('queued_node_access_rebuild');
      if ($queue->numberOfItems() != 0) {
        $queue->deleteQueue();
      }
    }

    $chunks = array_chunk($nids, 1000, TRUE);
    foreach ($chunks as $chunk) {
      // Add nodes to the queue.
      queued_node_access_rebuild_add($chunk);
      // Print progress
      $row++;
      if ($row %10 == 0) {
        drush_print('.', 0, NULL, FALSE);
      }
    }

    variable_set('queued_node_access_rebuild_processing', TRUE);

    // If we just initiated a full rebuild, set the node_access_needs_rebuild
    // state to false.
    if (empty($types)) {
      node_access_needs_rebuild(FALSE);
    }
    drush_print(dt('Complete!'));
  }
}
