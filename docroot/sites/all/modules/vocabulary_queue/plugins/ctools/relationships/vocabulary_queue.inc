<?php

/**
 * @file relationships/vocabulary_queue.inc
 * Plugin to provide an relationship handler for vocabulary queues.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Vocabulary queue'),
  'keyword' => 'vocabulary_queue',
  'description' => t('Adds a vocabulary queue from term.'),
  'required context' => new ctools_context_required(t('Term'), 'entity:taxonomy_term'),
  'context' => 'ctools_vocabulary_queue_context',
);

/**
 * Return a new context based on an existing context.
 */
function ctools_vocabulary_queue_context($context, $conf) {
  if (!empty($context->data) && ($queues = vocabulary_queue_get_vocabulary_queues())) {
    $references = array();
    foreach ($queues as $queue) {
      $references[$queue->qid] = array($context->data->tid);
    }
    if ($references && ($subqueues = nodequeue_load_subqueues_by_reference($references))) {
      return ctools_context_create('nodequeue:subqueue', reset($subqueues));
    }
  }
  return ctools_context_create_empty('nodequeue:subqueue');
}
