<?php
/**
 * @file
 * Access related functionality for the Elearning module.
 */

/**
 * Access callback for the main elearning entities.
 */
function elearning_entity_access($op, $entity, $account, $entity_type) {

  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  $access = FALSE;

  switch ($op) {
    case 'create':
    case 'delete':
    case 'update':
      $access = user_access('administer ' . str_replace('_', ' ', $entity_type), $account);
      break;

    case 'view':

      // This is necesary to make sure entityreference works properly.
      // Otherwise it will show - Restricted Access -
      $current_path = current_path();

      if (strpos($current_path, 'system/ajax') !== FALSE
        || strpos($current_path, 'admin/elearning') !== FALSE
        || strpos($current_path, 'multiple_fields_remove_button/ajax') !== FALSE
        || strpos($current_path, 'entityreference') !== FALSE
      ) {
        if (user_access('administer ' . str_replace('_', ' ', $entity_type))) {
          $access = TRUE;
        }
      }
      else {
        $func = $entity_type . '_entity_access';
        $access = call_user_func($func, $op, $entity, $account, $entity_type);
        $args[] = $access;
      }
      break;
  }
  return $access;
}

/**
 * Access callback for editing Elearning entities.
 */
function elearning_entity_edit_access($op, $entity, $account = NULL, $entity_type = NULL) {
  if (!$account) {
    $account = $GLOBALS['user'];
  }
  return elearning_entity_access($op, $entity, $account, $entity_type);
}

/**
 * Access callback for the course entity.
 */
function elearning_course_entity_access($op, $course, $account, $entity_type) {

  $access = FALSE;

  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  $account = $GLOBALS['user'];
  $access = elearning_course_get_access($course, $account);

  $invoke = module_invoke_all('elearning_course_access', $op, $course, $account, $entity_type, $access);
  if (!empty($invoke)) {
    $access = end($invoke);
  }

  return $access;
}

/**
 * Access callback for the unit entity.
 */
function elearning_unit_entity_access($op, $unit, $account, $entity_type, $course = NULL, $page = NULL) {

  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  $access = FALSE;

  if (!$course) {
    $course = menu_get_object('elearning_course', 1);
  }
  if ($course) {
    $course_wrapper = entity_metadata_wrapper('elearning_course', $course);
    $units_in_course = $course_wrapper->elearning_unit_reference->raw();

    if (in_array($unit->unit_id, $units_in_course)) {
      $access = elearning_course_entity_access($op, $course, $account, 'elearning_course');
      if ($access) {
        $invoke = module_invoke_all('elearning_unit_access', $op, $unit, $account, $entity_type, $course, $access);
        if (!empty($invoke)) {
          $access = end($invoke);
        }
      }
    }
  }
  return $access;
}

/**
 * Access callback for the exercise entity.
 */
function elearning_exercise_entity_access($op, $exercise, $account, $entity_type, $unit = NULL, $course = NULL, $page = FALSE) {
  $access = FALSE;

  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  if (!$unit) {
    $unit = menu_get_object('elearning_unit', 3);
  }
  if (!$course) {
    $course = menu_get_object('elearning_course', 1);
  }

  if ($unit && $course) {

    $unit_wrapper = entity_metadata_wrapper('elearning_unit', $unit);
    $field = $exercise->type . '_reference';
    $exercises_in_unit = $unit_wrapper->{$field}->raw();

    if (in_array($exercise->exercise_id, $exercises_in_unit)) {
      $access = elearning_unit_entity_access($op, $unit, $account, 'elearning_unit', $course);
      if ($page) {
        $exercise_wrapper = entity_metadata_wrapper('elearning_exercise', $exercise);
        $exercise_result = $exercise_wrapper->result->value();
        $allow_retry = variable_get($exercise->type . '_allow_retry', FALSE);
        if ($exercise_result && $exercise_result->status != ELEARNING_STATUS_STARTED && !$allow_retry) {
          $access = FALSE;
        }
      }
      $invoke = module_invoke_all('elearning_exercise_access', $op, $exercise, $account, $entity_type, $unit, $course, $access);
      if (!empty($invoke)) {
        $access = end($invoke);
      }
    }
  }
  return $access;
}

/**
 * Access callback for the question entity.
 */
function elearning_question_entity_access($op, $question, $account, $entity_type, $exercise = NULL, $unit = NULL, $course = NULL) {
  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  $access = FALSE;

  if (!$exercise) {
    $exercise = menu_get_object('elearning_exercise', 5);
  }
  if (!$unit) {
    $unit = menu_get_object('elearning_unit', 3);
  }
  if (!$course) {
    $course = menu_get_object('elearning_course', 1);
  }

  if ($exercise) {
    $exercise_wrapper = entity_metadata_wrapper('elearning_exercise', $exercise);
    $questions_in_exercise = $exercise_wrapper->elearning_question_reference->raw();
    if (in_array($question->question_id, $questions_in_exercise)) {
      $access = elearning_exercise_entity_access($op, $unit, $account, 'elearning_exercise', $unit, $course);
    }
  }

  return $access;
}

/**
 * Access callback for the Result entities.
 */
function elearning_entity_result_access($op, $result, $account = NULL, $entity_type = NULL) {

  $access = FALSE;

  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  if (user_access('administer elearning results', $account)) {
    $access = TRUE;
  }
  else {
    if ($result->uid == $account->uid && user_access('view own results', $account)) {

      // Allow modules to alter the access to the results.
      $access = TRUE;

      $access_invoke = module_invoke_all('elearning_result_access', $op, $result, $account, $entity_type);
      if (!empty($access_invoke)) {
        $access = end($access_invoke);
      }
    }
    else {
      $access = FALSE;
    }
  }

  return $access;
}

/**
 * Check if the user has access to a certain course.
 */
function elearning_course_get_access($course, $account) {
  $uid = $account->uid;

  $course_id = $course->course_id;
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'elearning_course_access')->propertyCondition('course_id', $course_id)->propertyCondition('uid', $uid)->execute();

  if (!empty($result)) {
    $access_id = reset($result['elearning_course_access'])->course_access_id;
    $access_entity = entity_load_single('elearning_course_access', $access_id);
    return $access_entity->status;
  }
}

/**
 * Restart Elearning entity access callback.
 */
function elearning_entity_retry_access() {
  $args = func_get_args();
  $entity_type = $args[0];
  $account = $GLOBALS['user'];

  switch ($entity_type) {
    case 'elearning_course':
      $course = $args[1];
      if (elearning_course_entity_access('view', $course, $account, $entity_type)) {
        $wrapper = entity_metadata_wrapper($entity_type, $course);
        $result = $wrapper->result->value();
        if ($result && $result->status != ELEARNING_STATUS_STARTED) {
          return variable_get($entity_type . '_allow_retry', TRUE);
        }
        else {
          return FALSE;
        }
      }
      break;

    case 'elearning_unit':
      $unit = $args[1];
      $course = $args[2];
      if ($course && elearning_unit_entity_access('view', $unit, $account, 'elearning_unit', $course)) {
        $wrapper = entity_metadata_wrapper($entity_type, $unit);
        $result = $wrapper->result->value();
        if ($result && $result->status != ELEARNING_STATUS_STARTED) {
          return variable_get($entity_type . '_allow_retry', TRUE);
        }
        else {
          return FALSE;
        }
      }
      break;
  }
}
