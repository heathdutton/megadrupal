<?php

/**
 * @file
 * Batch API Code.
 */

/**
 * Processes the Batch.
 *
 * @param string $field_name
 *   Name of the field to reassign.
 * @param string $bundle
 *   Reassign for this node type.
 */
function _menu_to_taxonomy_assign_reassign_batch($field_name, $bundle) {
  $nodes = node_load_multiple(array(), array('type' => $bundle));
  $batch = array(
    'operations' => array(
      array(
        '_menu_to_taxonomy_assign_reassign_process',
        array(
          $nodes,
          $field_name,
        ),
      ),
    ),
    'finished' => '_menu_to_taxonomy_assign_reassign_success',
    'title' => t('Reassigning term references'),
    'init_message' => t('The term references are about to be reassigned.'),
    'progress_message' => t('Import progress: Completed @current of @total stages.'),
    'redirect' => 'admin/structure/taxonomy',
    'error_message' => t('The term reference reassignment process encountered an error.'),
  );
  batch_set($batch);
  batch_process();
}

/**
 * Reassigns 10 items.
 *
 * @param array $nodes
 *   The nodes to reassign.
 * @param string $field_name
 *   Name of the field to reassign.
 * @param array $context
 *   The Batch context.
 */
function _menu_to_taxonomy_assign_reassign_process(array $nodes, $field_name, array &$context) {
  $start = NULL;
  $end = NULL;
  _menu_to_taxonomy_batch_init_context($context, $start, $end, 10);

  // Loop through $terms to process each term.
  for ($i = $start; $i < count($nodes) && $i < $end; $i++) {
    // Get the n-th node in the array (ie element number n).
    $nth_node = array_slice($nodes, $i, 1);
    $node = reset($nth_node);
    _menu_to_taxonomy_assign_save($node, $field_name);
  }

  _menu_to_taxonomy_batch_update_context($context, $end, count($nodes), 'Reassigning term references');
}

/**
 * Sets a message stating the menu has been updated.
 */
function _menu_to_taxonomy_assign_reassign_success() {
  drupal_set_message(t('The term references have been reassigned.'));
}

/**
 * Initialises the batch context.
 *
 * @param array $context
 *   Batch context array.
 * @param int $start
 *   The item to start on in this pass.
 * @param int $end
 *   The end item of this pass.
 * @param int $items
 *   The number of items to process in this pass.
 */
function _menu_to_taxonomy_assign_batch_init_context(array &$context, &$start, &$end, $items) {
  // Initialize sandbox the first time through.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }

  $start = $context['sandbox']['progress'];
  $end = $start + $items;
}

/**
 * Update the batch context.
 *
 * @param array $context
 *   The Batch context array.
 * @param int $end
 *   The end point of the most recent pass.
 * @param int $total
 *   The total number of items to process in this batch.
 * @param string $msg
 *   Message for the progress bar.
 */
function _menu_to_taxonomy_assign_batch_update_context(array &$context, $end, $total, $msg) {
  // Update context array.
  if ($end > $total) {
    $context['finished'] = 1;
    return;
  }
  $context['message'] = t("%msg: %end of %total", array(
    '%msg' => $msg,
    '%end' => $end,
    '%total' => $total,
  ));
  $context['sandbox']['progress'] = $end;
  $context['finished'] = $end / $total;
}
