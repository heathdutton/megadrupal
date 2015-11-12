<?php
/**
 * @file
 * Helper functions for processing the Elearning results.
 */

/**
 * Helper function process the course result.
 */
function elearning_process_course_result($course, $account = NULL) {

  if (!$account) {
    $account = $GLOBALS['user'];
  }

  if (!$course) {
    $course = menu_get_object('elearning_course', 1);
  }

  $course_wrapper = entity_metadata_wrapper('elearning_course', $course);
  $course_result = $course_wrapper->result->value();

  $total_units = $course_wrapper->elearning_unit_reference->count();

  $query = db_select('elearning_unit_result', 'ur');
  $query->join('elearning_course_result', 'cr', 'cr.result_id = ur.course_result');

  $finished_units = $query->fields('ur', array('result_id'))->condition('ur.status', ELEARNING_STATUS_STARTED, '!=')->condition('ur.uid', $account->uid)->condition('cr.result_id', $course_result->result_id)->condition('cr.status', ELEARNING_STATUS_STARTED)->execute()->fetchAll();

  $course_result->progress = count($finished_units) / $total_units;

  // All eligeble exercises have been finished: calculate the score and finish
  // the unit.
  if (count($finished_units) == $total_units) {
    $score = 0;

    foreach ($finished_units as $record) {
      $result = entity_load_single('elearning_unit_result', $record->result_id);
      $score += $result->score;
    }

    // Average score.
    $average = $score / count($finished_units);

    $course_result->score = $average;

    // Threshold.
    $threshold_per_course = variable_get('elearning_course_threshold_per_assessment', FALSE);
    $threshold = variable_get('elearning_course_default_threshold', ELEARNING_DEFAULT_PASSING_THRESHOLD);

    if ($threshold_per_course) {
      $settings = unserialize($course->settings);
      $threshold = isset($settings['pass_threshold']) ? $settings['pass_threshold'] : $threshold;
    }

    if ($score >= $threshold / 100) {
      $course_result->status = ELEARNING_STATUS_PASSED;
    }
    else {
      $course_result->status = ELEARNING_STATUS_FAILED;
    }
  }
  entity_save('elearning_course_result', $course_result);
}

/**
 * Helper function process the unit result.
 */
function elearning_process_unit_result($unit = NULL, $course = NULL, $account = NULL) {

  if (!$account) {
    $account = $GLOBALS['user'];
  }

  if (!$unit) {
    $unit = menu_get_object('elearning_unit', 3);
  }
  if (!$course) {
    $course = menu_get_object('elearning_course', 1);
  }

  $unit_wrapper = entity_metadata_wrapper('elearning_unit', $unit);
  $unit_result = $unit_wrapper->result->value();

  $types = array_values(variable_get('elearning_exercise_types_for_unit_evaluation', array()));

  $total_exercises = 0;

  foreach ($types as $key => $type) {
    if ($type === 0) {
      unset($types[$key]);
    }
    else {
      $field = $type . '_reference';
      $total_exercises += $unit_wrapper->{$field}->count();
    }
  }

  // Get all finished unit exercise belonging to this unit result.

  $query = db_select('elearning_exercise_result', 'er');

  $query->join('elearning_exercise', 'e', 'e.exercise_id = er.exercise_id');
  $query->join('elearning_unit_result', 'ur', 'ur.result_id = er.unit_result');

  $finished_exercises = $query->fields('er', array('result_id'))
    ->condition('e.type', $types, 'IN')
    ->condition('er.status', ELEARNING_STATUS_STARTED, '!=')
    ->condition('er.uid', $account->uid)
    ->condition('ur.result_id', $unit_result->result_id)
    ->execute()
    ->fetchAll();

  // Set Unit result progress.
  $unit_result->progress = count($finished_exercises) / $total_exercises;

  // Set unit result score and status. When all exercises of types that should
  // be included for results are finished.
  if (count($finished_exercises) == $total_exercises) {
    $score = 0;

    foreach ($finished_exercises as $record) {
      $result = entity_load_single('elearning_exercise_result', $record->result_id);
      $score += $result->score;
    }
    $average = $score / count($finished_exercises);
    $unit_result->score = $average;

    // Threshold.
    $threshold_per_unit = variable_get('elearning_unit_threshold_per_elearning_unit', FALSE);

    if ($threshold_per_unit) {
      $settings = unserialize($unit->settings);
      $threshold = isset($settings['pass_threshold']) ? $settings['pass_threshold'] : $threshold;
    }
    else {
      $threshold = variable_get('elearning_unit_default_threshold', ELEARNING_DEFAULT_PASSING_THRESHOLD);
    }

    if ($score >= $threshold / 100) {
      $unit_result->status = ELEARNING_STATUS_PASSED;
    }
    else {
      $unit_result->status = ELEARNING_STATUS_FAILED;
    }
  }

  entity_save('elearning_unit_result', $unit_result);
}

/**
 * Helper function to save the exercise result.
 */
function elearning_process_exercise_result($exercise_result, $question_results) {

  $correct_points = 0;
  $total_points = count($question_results);

  foreach ($question_results as $question_result) {
    $correct_points += $question_result->correct;
  }

  $score = $correct_points / $total_points;

  $exercise_result_wrapper = entity_metadata_wrapper('elearning_exercise_result', $exercise_result);
  $exercise_result_wrapper->finished = time();
  $exercise_result_wrapper->duration = time() - $exercise_result_wrapper->started->value();
  $exercise_result_wrapper->score = $score;

  // Use per exercise pass threshold or general.
  $exercise_type = $exercise_result_wrapper->exercise->type->value();
  $threshold_per_exercise = variable_get('elearning_exercise_threshold_per_' . $exercise_type, FALSE);

  if ($threshold_per_exercise) {
    $settings = unserialize($exercise_result_wrapper->exercise->value()->settings);
    $threshold = isset($settings['pass_threshold']) ? $settings['pass_threshold'] : $threshold;
  }
  else {
    $threshold = variable_get($exercise_type . '_default_threshold', ELEARNING_DEFAULT_PASSING_THRESHOLD);
  }

  if ($score >= $threshold / 100) {
    $exercise_result_wrapper->status = ELEARNING_STATUS_PASSED;
  }
  else {
    $exercise_result_wrapper->status = ELEARNING_STATUS_FAILED;
  }

  $exercise_result_wrapper->save();
}
