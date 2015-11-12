<?php
/**
 * @file
 * Admin pages for vocabulary queue.
 */

/**
 * Display a list of subqueues for a queue and their sizes.
 */
function vocabulary_queue_taxonomy_vocabulary_queues($machine_name) {
  $queues = vocabulary_queue_get_vocabulary_queues($machine_name);
  if (!$queues) {
    return drupal_not_found();
  }

  if (count($queues) == 1) {
    $queue = reset($queues);
    if (!nodequeue_queue_access($queue)) {
      return drupal_access_denied();
    }
    nodequeue_queue_access($queue);
    module_load_include('admin.inc', 'nodequeue', 'includes/nodequeue');
    return nodequeue_view_subqueues(reset($queues));
  }

  // List each related nodequeue.
  $items = array();
  foreach ($queues as $queue) {
    if (nodequeue_queue_access($queue)) {
      $items[$queue->qid] = l($queue->title, 'admin/structure/nodequeue/' . $queue->qid . '/view');
    }
  }
  if (empty($items)) {
    return drupal_access_denied();
  }
  return theme('item_list', array('title' => t('Vocabulary queues'), 'items' => $items));
}

/**
 * Display a list of subqueues for a queue and their sizes.
 */
function vocabulary_queue_taxonomy_term_queues($term) {
  $queues = vocabulary_queue_get_vocabulary_queues();
  $references = array();
  foreach ($queues as $queue) {
    $references[$queue->qid] = array($term->tid);
  }
  $subqueues = nodequeue_load_subqueues_by_reference($references);

  if (!$subqueues) {
    return drupal_not_found();
  }

  if (count($subqueues) == 1) {
    $subqueue = reset($subqueues);
    $queue = $queues[$subqueue->qid];
    module_load_include('admin.inc', 'nodequeue', 'includes/nodequeue');
    return nodequeue_arrange_subqueue($queue, $subqueue);
  }

  // List each related nodequeue.
  $items = array();
  foreach ($subqueues as $subqueue) {
    $items[$subqueue->qid] = l($queues[$subqueue->qid]->title . ': ' . $subqueue->title, 'admin/structure/nodequeue/' . $subqueue->qid . '/view/' . $subqueue->sqid);
  }
  return theme('item_list', array('title' => t('Vocabulary queues'), 'items' => $items));
}
