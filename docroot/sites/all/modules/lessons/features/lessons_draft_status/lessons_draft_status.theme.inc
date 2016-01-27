<?php
/**
 * @file
 * Theme lesson nodes to provide visual queues indicating draft status.
 */

/**
 * Implements hook_theme().
 */
function lessons_draft_status_theme($existing, $type, $theme, $path) {
  $items = array(
    'lessons_draft_status_colors' => array(
      'render element' => 'elements',
    ),
    'field__field_lesson_draft_status' => array(
      'render element' => 'elements',
      'path' => drupal_get_path('module', 'lessons_draft_status') ."/theme",
      'template' => 'field--field-lesson-draft-status',
    ),
  );

  return $items;
}

/**
 * Implements hook_preprocess_field().
 */
function lessons_draft_status_preprocess_field(&$variables) {
  $element = $variables['element'];
  
  if ($element['#field_name'] == 'field_lesson_draft_status'
  && $element['#entity_type'] == 'node'
  && in_array('#object', array_keys($element))
  && $element['#object']->type == 'lesson') {

    // Get info about last peer review.
    $node = $element['#object'];
    if (!empty($node->field_lesson_last_peer_review['und'][0]['value'])) {
      $last_peer_review_timestamp = $node->field_lesson_last_peer_review['und'][0]['value'];
      $last_peer_review = t('Last peer review') . ': ' . date('j M Y', $last_peer_review_timestamp);  
    }
    else {
      $last_peer_review_timestamp = 0;
      $last_peer_review = t('AWAITING PEER REVIEW');
    }

    // Get CSS class.
    $lesson_draft_status = check_plain(_lessons_draft_status_machine_name($element['#items'][0]['value']));

    // Check for not-availables. Set human readable values.
    if ($variables['element']['#field_name'] == 'field_lesson_draft_status' 
    && $variables['items'][0]['#markup'] == 'not-available') {
      $variables['items'][0]['#markup'] = t('Draft status not available');
    }

    // Set variables to be surfaced in template file.
    $variables['last_peer_review_timestamp'] = $last_peer_review_timestamp;
    $variables['last_peer_review'] = $last_peer_review;
    $variables['lesson_draft_status'] = $last_peer_review;
  }
}

/**
 * Implements hook_preprocess_node().
 *
 * Expose lesson status variables in node--lesson.tpl.php: 
 *  - $lesson_draft_status
 *  - $classes (via classes_array)
 */
function lessons_draft_status_preprocess_node(&$vars) {
  $node = $vars['node'];
  if ($node->type == 'lesson') {
    // Get lesson draft status from field_lesson_draft_status.
    $field_lesson_draft_status = $node->field_lesson_draft_status;
    if (in_array('und', array_keys($field_lesson_draft_status))) {
      $lesson_draft_status = $field_lesson_draft_status['und'][0]['value'];
    }
    else if (in_array('0', array_keys($field_lesson_draft_status))) {
      $lesson_draft_status = $field_lesson_draft_status[0]['value'];
    }
    else {
      $lesson_draft_status = 'not-available';  
    }

    // Convert machine name to something we can put in CSS. Defaults are:
    // draft, ready-for-review, needs-revision, up-to-date.
    $lesson_draft_status = _lessons_draft_status_machine_name($lesson_draft_status);
    $lesson_draft_status = 'lesson-draft-status-' . $lesson_draft_status;

    // Set variables to be surfaced in template file.
    $vars['lesson_draft_status'] = $lesson_draft_status;
    $vars['classes_array'][] = $lesson_draft_status;
  }
}

/**
 * Callback for theme_lessons_draft_status_colors().
 * 
 * @param $variables
 *  Drupal render array.
 * 
 * @return
 *  String. HTML.
 */
function theme_lessons_draft_status_colors($variables) {
  $output = '';

  $statuses = $variables['elements']['#lessons_draft_statuses'];
  foreach ($statuses as $key => $value) {
    $output .= '<div class="lesson-draft-status-legend lesson-draft-status-' . $key . '">' . check_plain($value) . '</div>';
  }
  $output .= '<div class="lesson-draft-status-legend lesson-draft-status-not-available">' . t('Status not available') . '</div>';
  $output = '<div class="lesson">' . $output . '</div>';

  return $output;  
}
