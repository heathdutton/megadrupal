<?php

/**
 * Executes a batch operation for entity_review_mass_update().
 *
 * @param array $reviews
 *   An array of entity_review IDs.
 * @param array $updates
 *   Associative array of updates.
 * @param array $context
 *   An array of contextual key/values.
 */
function _entity_review_mass_update_batch_process($reviews, $updates, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($reviews);
    $context['sandbox']['reviews'] = $reviews;
  }

  // Process reviews by groups of 5.
  $count = min(5, count($context['sandbox']['reviews']));
  for ($i = 1; $i <= $count; $i++) {
    // For each id.
    $id = array_shift($context['sandbox']['reviews']);

    // Load the review.
    $reviews = entity_load('review', array($id));
    $review = reset($reviews);

    // Reset the values, and save it.
    _entity_review_update($review, $updates);

    // Update our progress information.
    $context['sandbox']['progress']++;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Menu callback: Reports the status of batch operation for entity_review_mass_update().
 *
 * @param bool $success
 *   A boolean indicating whether the batch mass update operation successfully
 *   concluded.
 * @param array $results
 *   The number of reviews updated via the batch mode process.
 * @param array $operations
 *   An array of function calls (not used in this function).
 */
function _entity_review_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    $message = format_plural(count($results), '1 item successfully processed:', '@count items successfully processed.');
    drupal_set_message($message);
  }
}
