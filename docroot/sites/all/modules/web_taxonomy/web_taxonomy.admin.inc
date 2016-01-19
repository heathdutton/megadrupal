<?php

/**
 * @file
 * Content administration and module settings UI.
 */

function web_taxonomy_update_vocabulary($wt_name) {
  $wt = web_taxonomy_get_class($wt_name);
  $wt->updateVocabulary();
}

/*
 * Term update batch process.
 */
function _web_taxonomy_update_term_batch_process($term, $wt, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['message'] = t('Updating @term', array('@term' => $term->name));
    $context['finished'] = 0;
  }

  $wt->saveTerm($term);

  // Store result for post-processing in the finished callback.
  $context['results']['vocabulary'] = $wt->getName();

  // Update our progress information.
  $context['sandbox']['progress']++;

  $context['finished'] = 1;
}

/**
 * Term update batch 'finished' callback.
 */
function _web_taxonomy_update_vocabulary_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The @vocab vocabulary has been updated.', array('@vocab' => $results['vocabulary'])));
    return;
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    $message = format_plural(count($results), '1 item successfully processed:', '@count items successfully processed:');
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
    return;
  }
}