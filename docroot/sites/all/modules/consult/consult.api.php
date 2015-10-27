<?php
/**
 * @file
 * API documentation for consult module.
 */

/**
 * Alter questions of a consult before they are added to javascript settings.
 *
 * @param array $questions
 *   The questions being outputted.
 */
function hook_consult_js_questions_alter(&$questions) {
  foreach ($questions as $question) {
    if ($question['type'] == 'foo') {
      $question['label'] = t('bar');
    }
  }
}

/**
 * Alter results of a consult before they are added to javascript settings.
 *
 * @param array $results
 *   The results being outputted.
 */
function hook_consult_js_results_alter(&$results) {
  foreach ($results as $result) {
    if ($result['type'] == 'foo') {
      $result['label'] = t('bar');
    }
  }
}
