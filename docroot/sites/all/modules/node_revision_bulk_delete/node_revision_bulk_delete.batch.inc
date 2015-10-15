<?php
/**
 * @file
 * Contaiing batch operation functions for node revisions bulk delete
 */

/**
 * Process for one batch step, deleting a revision.
 */
function node_revision_bulk_delete__batch_delete_revision($revision, &$context) {
  $context['results'][] = 'Revision deleted: ' . $revision;
  $context['message'] = t('Processing revision "@id"', array('@id' => $revision));
  node_revision_delete($revision);
}

/**
 * Finishing bulk operations.
 */
function node_revision_bulk_delete__batch_op_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), 'One prior revision deleted.', '@count prior revisions deleted.');
    drupal_set_message($message);
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments: @args', array(
      '@operation' => $error_operation[0],
      '@args' => print_r($error_operation[0], TRUE),
    )));
  }
  if (isset($results['redirect_view'])) {
    drupal_goto('node/' . $results['redirect_view']);
  }
}

/**
 * Redirect after finished.
 *
 * In case there are no more revisions, we should be redirected to the node view
 * as the node revision page does not exist.
 */
function node_revision_bulk_delete__batch_redirect_node_view($nid, &$context) {
  $context['results']['redirect_view'] = $nid;
}
