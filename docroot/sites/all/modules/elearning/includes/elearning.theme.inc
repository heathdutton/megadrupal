<?php

/**
 * @file
 * Theme functions for the Elearning module.
 */

/**
 * Implements theme_HOOK().
 *
 * Theme function for the answer field table.
 */
function theme_elearning_question_field_form_table($variables) {
  $element = $variables['element'];
  $delta = $element['#delta'];
  $columns = $element['#table_columns'];
  $row_count = count(element_children($element['key']));

  $header = $columns;

  $rows = array();
  for ($i = 0; $i < $row_count; $i++) {
    $array = array();
    foreach ($header as $column => $column_name) {
      $array[] = array('data' => $element[$column][$i]);
    }
    $rows[] = $array;
  }
  $return = theme('table', array('header' => $header, 'rows' => $rows));
  $return .= render($element['add_row']);
  $return .= render($element['remove_row']);
  return $return;
}

/**
 * Implements theme_HOOK().
 */
function theme_elearning_progress($variables) {
  $progress = $variables['progress'];
  $result = $variables['result'];
  if ($result && $result->status != ELEARNING_STATUS_UNSTARTED) {
    if ($progress >= 1):
      $message = t('Completed!');
    else:
      $message = t('@percentage complete', array('@percentage' => (round($progress * 100)) . '%'));
    endif;

    $output = '<div class="progress">';
    $output .= $message;
    $output .= '<div class="progress-bar-wrapper">
      <div class="progress-bar" style="width:' . $progress * 100 . '%">
      </div>
      </div>
      </div>';
    return $output;
  }
}

/**
 * Implements theme_HOOK().
 */
function theme_elearning_score($variables) {
  $score = $variables['score'];
  $status = $variables['status'];
  return "<div class='score $status'><div class='inner'>" . round($score * 100) . "%</div></div>";
}
