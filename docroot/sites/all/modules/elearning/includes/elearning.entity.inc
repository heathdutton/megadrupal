<?php
/**
 * @file
 * Classes and methods and definitions for the Elearning entities.
 */

/**
 * Defines the ElearningEntityController.
 */
class ElearningEntityController extends EntityAPIController{

  /**
   * Overwrites EntityAPIController::__construct.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Overwrites EntityAPIController::create.
   */
  public function create(array $values = array()) {
    $account = $GLOBALS['user'];
    $values += array(
      'is_new' => TRUE,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $account->uid,
      'settings' => '',
    );
    return parent::create($values);
  }

  /**
   * Overwrites EntityAPIController::delete.
   */
  public function delete($ids, $transaction = NULL) {

    $entities = $ids ? $this->load($ids) : FALSE;

    foreach ($entities as $entity) {
      $entity_type = $entity->entityType();
      $db_field = str_replace('elearning_', '', $entity_type) . '_id';

      $query = new EntityFieldQuery();
      $results = $query->entityCondition('entity_type', $entity_type . '_result')->propertyCondition($db_field, $id)->execute();

      if (isset($results[$entity_type . '_result'])) {
        $result_ids = array_keys($results[$entity_type . '_result']);
        entity_delete($entity_type, $result_ids);
      }
    }
    parent::delete($ids, $transaction);
  }

  /**
   * Overwrites EntityAPIController::buildContent.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    $entity_type = $entity->entityType();
    $entity_path_element = str_replace('elearning_', '', $entity_type);
    $entity_info = entity_get_info($entity_type);
    $entity_id_key = $entity_info['entity keys']['id'];
    $build['#contextual_links'][$entity_type] = array('admin/elearning/' . $entity_path_element, array($entity->{$entity_id_key}));
    return $build;
  }
}

/**
 * Defines the ElearningCourseController.
 *
 * Saves and deletes related Course Access Entities.
 */
class ElearningCourseController extends ElearningEntityController {

  /**
   * Overwrites ElearningEntityController::__construct.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Overwrites ElearningEntityController::delete.
   */
  public function delete($ids, $transaction = NULL) {
    parent::delete($ids, $transaction);

    // Delete course_access entities too.
    $result_query = new EntityFieldQuery();
    $result_entities = $result_query
      ->entityCondition('entity_type', 'elearning_course_access')
      ->propertyCondition('course_id', $ids, 'IN')
      ->execute();
    if (!empty($result_entities)) {
      foreach (array_keys($result_entities['elearning_course_access']) as $course_access_id) {
        entity_delete('elearning_course_access', $course_access_id);
      }
    }
  }

  /**
   * Overwrites ElearningEntityController::save.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $new = $entity->is_new ? TRUE : FALSE;
    $return = parent::save($entity, $transaction);
    $course = $entity;
    if ($new) {
      $accounts = entity_load('user');
      foreach (array_keys($accounts) as $uid) {
        if ($uid != 0) {
          $course_access = new StdClass();

          $wrapper = entity_metadata_wrapper("elearning_course_access", $course_access);
          $wrapper->uid = $uid;
          $wrapper->course_id = $course->course_id;
          $wrapper->status = FALSE;
          $wrapper->save();
        }
      }
    }
    return $return;
  }
}

/**
 * Defines the ElearningResultEntityController.
 *
 * When a Elearning result is first created, the parent result is automatically
 * created as well, if it didn't exist already. Unit and Course results are
 * processed when an exercise has been saved. The way the exercise itself is
 * saved is handled by the exercise type modules, since it might use ajax to do
 * so, in which case we wouldn't have the unit and course as menu objects.
 *
 * @see elearning_process_course_result().
 * @see elearning_process_unit_result().
 */
class ElearningResultEntityController extends EntityAPIController{

  /**
   * Overwrites ElearningEntityController::__construct.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Overwrites ElearningEntityController::create.
   */
  public function create(array $values = array()) {

    // Add 'is_new' property to object to know we're dealing with a newly
    // created Elearning result.
    $values += array(
      'is_new' => TRUE,
    );
    return parent::create($values);
  }

  /**
   * Overwrites ElearningEntityController::save.
   */
  public function save($entity, $transaction = NULL) {

    $entity_result_type = $entity->entityType();

    $account = $GLOBALS['user'];

    switch ($entity_result_type) {
      case 'elearning_course_result':
        return parent::save($entity, $transaction);

      case 'elearning_unit_result':
        $result_wrapper = entity_metadata_wrapper('elearning_unit_result', $entity);
        $course = $result_wrapper->unit->course->value();
        $course_result = $result_wrapper->unit->course->result->value();

        if (isset($entity->is_new)) {

          parent::save($entity, $transaction);

          // Save empty course result.
          if (!$course_result OR $course_result->status != ELEARNING_STATUS_STARTED) {
            $values = array(
              'course_id' => $course->course_id,
              'uid' => $account->uid,
              'started' => REQUEST_TIME,
              'status' => ELEARNING_STATUS_STARTED,
              'progress' => 0,
            );
            $course_result = entity_create('elearning_course_result', $values);
            entity_save('elearning_course_result', $course_result);
          }
          $entity->course_result = $course_result->result_id;
          return parent::save($entity, $transaction);
        }
        else {

          $return = parent::save($entity, $transaction);

          // Set Course progress and score.
          if ($course) {
            elearning_process_course_result($course);
          }

          return $return;
        }
        break;

      case 'elearning_exercise_result':
        $result_wrapper = entity_metadata_wrapper('elearning_exercise_result', $entity);
        $unit_result = $result_wrapper->exercise->unit->result->value();

        $unit = $result_wrapper->exercise->unit->value();
        $course = $result_wrapper->exercise->unit->course->value();
        if (isset($entity->is_new)) {
          // Save the exercise result a first time
          // Then add the unit result ID to it if it doesn't already exist.
          parent::save($entity, $transaction);

          // Save empty unit result.
          if ((!$unit_result) OR ($unit_result->status != ELEARNING_STATUS_STARTED)) {
            $values = array(
              'unit_id' => $unit->unit_id,
              'uid' => $account->uid,
              'started' => REQUEST_TIME,
              'status' => ELEARNING_STATUS_STARTED,
              'progress' => 0,
            );
            $unit_result = entity_create('elearning_unit_result', $values);
            entity_save('elearning_unit_result', $unit_result);
          }
          $entity->unit_result = $unit_result->result_id;
          return parent::save($entity, $transaction);
        }
        else {
          $entity->unit_result = $unit_result->result_id;
          $return = parent::save($entity, $transaction);

          // Set Unit progress and score.
          elearning_process_unit_result($unit, $course);

          return $return;
        }
        break;

      case 'elearning_question_result':
        return parent::save($entity, $transaction);
    }
  }

  /**
   * Overwrites ElearningEntityController::delete.
   */
  public function delete($ids, $transaction = NULL) {

    // Delete all underlying results when a result is deleted.
    $entities = $ids ? $this->load($ids) : FALSE;

    foreach ($entities as $id => $entity) {
      $entity_type_result = $entity->entityType();
      $entity_type = str_replace('_result', '', $entity_type_result);
      $entity_type = str_replace('elearning_', '', $entity_type);

      $child_type = elearning_entity_basic_keys_relative($entity_type, 'child');

      if ($child_type) {

        $result_query = new EntityFieldQuery();

        $parent_reference_field = str_replace('elearning_', '', $entity_type_result);

        $result_entities = $result_query
          ->entityCondition('entity_type', 'elearning_' . $child_type . '_result')
          ->propertyCondition($parent_reference_field, $id)
          ->execute();

        if (!empty($result_entities)) {
          foreach (array_keys($result_entities['elearning_' . $child_type . '_result']) as $child_id) {
            entity_delete('elearning_' . $child_type . '_result', $child_id);
          }
          drupal_set_message(t('Deleted !count related !child results.', array(
                '!count' => count($result_entities['elearning_' . $child_type . '_result']),
                '!child' => $child_type,
              )));
        }
      }
    }
    parent::delete($ids, $transaction);
  }
}

/**
 * Defines uri for Elearning Course entity.
 */
function elearning_course_uri($course) {
  return array(
    'path' => 'course/' . $course->course_id,
  );
}

/**
 * Defines uri for Elearning Unit entity.
 */
function elearning_unit_uri($unit) {
  $unit_wrapper = entity_metadata_wrapper('elearning_unit', $unit);
  $course = $unit_wrapper->course->value();
  if ($course) {
    $path = 'course/' . $course->course_id . '/unit/' . $unit->unit_id;
  }
  else {
    $path = 'admin/elearning/unit/' . $unit->unit_id;
  }
  return array(
    'path' => $path,
  );
}

/**
 * Defines uri for Elearning Unit entity.
 */
function elearning_exercise_uri($exercise) {
  $exercise_wrapper = entity_metadata_wrapper('elearning_exercise', $exercise);
  $unit = $exercise_wrapper->unit->value();
  if ($unit) {
    $course = $exercise_wrapper->unit->course->value();
    $path = 'course/' . $course->course_id . '/unit/' . $unit->unit_id . '/exercise/' . $exercise->exercise_id;
  }
  else {
    $path = 'admin/elearning/exercise/' . $exercise->exercise_id;
  }
  return array(
    'path' => $path,
  );
}

/**
 * Computed property callback for getting the course.
 */
function elearning_property_course_getter($data, $options, $name, $type, $info) {
  $course = menu_get_object('elearning_course', 1);
  if ($course) {
    return $course->course_id;
  }
}

/**
 * Computed property callback for getting the exercise.
 */
function elearning_property_unit_getter($data, $options, $name, $type, $info) {
  $course = menu_get_object('elearning_course', 1);
  $unit = menu_get_object('elearning_unit', 3);
  if ($course && $unit) {
    return $unit->unit_id;
  }
}

/**
 * Computed property callback for getting the exercise.
 */
function elearning_property_exercise_getter($data, $options, $name, $type, $info) {
  $course = menu_get_object('elearning_course', 1);
  $unit = menu_get_object('elearning_unit', 3);
  $exercise = menu_get_object('elearning_exercise', 5);
  if ($course && $unit && $exercise) {
    return $exercise->exercise_id;
  }
}

/**
 * Computed value of the last result belonging to an Elearning entity Type.
 */
function elearning_property_result_getter($entity, $options = array(), $name = NULL, $type = NULL, $info = NULL) {

  if ($entity) {
    $entity_type = $entity->entityType();

    $account = $GLOBALS['user'];

    switch ($entity_type) {
      case 'elearning_course':
        $query = db_select('elearning_course_result', 'cr');

        $results = $query->fields('cr', array('result_id'))
          ->condition('cr.uid', $account->uid)
          ->condition('cr.course_id', $entity->course_id)
          ->orderBy('cr.started', 'DESC')
          ->execute()
          ->fetchAll();

        if (!empty($results)) {
          return entity_load_single('elearning_course_result', $results[0]->result_id);
        }
        else {
          return FALSE;
        }
        break;

      case 'elearning_unit':

        // We cannot use either $wrapper->course nor menu_get_object because
        // this would cause a loop.

        $course_id = arg(1);

        $course = entity_load_single('elearning_course', $course_id);

        $course_result = elearning_property_result_getter($course);

        $query = db_select('elearning_unit_result', 'ur');
        $query->join('elearning_course_result', 'cr', 'cr.result_id = ur.course_result');

        $query->fields('ur', array('result_id'))
          ->condition('ur.uid', $account->uid)
          ->condition('ur.unit_id', $entity->unit_id);

        if ($course_result) {
          $query->condition('cr.result_id', $course_result->result_id);
        }

        $results = $query->orderBy('ur.started', 'DESC')->execute()->fetchAll();

        if (!empty($results)) {
          return entity_load_single('elearning_unit_result', $results[0]->result_id);
        }
        else {
          return FALSE;
        }
        break;

      case 'elearning_exercise':

        // We cannot use either $wrapper->course nor menu_get_object because
        // this would cause a loop.

        $unit_id = arg(3);
        $course_id = arg(1);

        if ($unit_id && $course_id) {
          $unit = entity_load_single('elearning_unit', $unit_id);
          $course = entity_load_single('elearning_course', $course_id);
          $unit_result = elearning_property_result_getter($unit);
          $course_result = elearning_property_result_getter($course);
        }

        if ($unit_result && $course_result) {
          $query = db_select('elearning_exercise_result', 'er');

          $query->join('elearning_unit_result', 'ur', 'ur.result_id = er.unit_result');
          $query->join('elearning_course_result', 'cr', 'cr.result_id = ur.course_result');

          $query->fields('er', array('result_id'))
            ->condition('er.uid', $account->uid)
            ->condition('er.exercise_id', $entity->exercise_id);

          if (isset($unit_result) && $unit_result) {
            $query->condition('ur.result_id', $unit_result->result_id);
          }
          if (isset($course_result) && $course_result) {
            $query->condition('cr.result_id', $course_result->result_id);
          }

          $results = $query->orderBy('er.started', 'DESC')
            ->execute()
            ->fetchAll();

          if (!empty($results)) {
            return entity_load_single('elearning_exercise_result', $results[0]->result_id);
          }
          else {
            return FALSE;
          }
        }
        else {
          return FALSE;
        }
        break;

      case 'elearning_question':
        $question_wrapper = entity_metadata_wrapper('elearning_question', $entity);

        $query = db_select('elearning_question_result', 'qr');
        $query->join('elearning_exercise_result', 'er', 'er.result_id = qr.exercise_result');

        $results = $query->fields('qr', array('result_id'))->condition('qr.uid', $account->uid)->condition('qr.question_id', $entity->question_id)->condition('er.status', ELEARNING_STATUS_STARTED)->execute()->fetchAll();
        if (!empty($results)) {
          return entity_load_single('elearning_question_result', $results[0]->result_id);
        }
        else {
          return FALSE;
        }
        break;
    }
  }
  else {
    return FALSE;
  }
}

/**
 * Property callback for different elearning entities.
 *
 * To improve readability of the code we create properties that actually take
 * their value from a table field that is called differently. For example:
 * $entity_wrapper->question->value() means to get the entity based
 * in the 'question_id' field value.
 */
function elearning_metadata_get_properties($data, $options, $name) {

  switch ($name) {
    case 'author':
      return $data->uid;

    case 'user':
      return $data->uid;

    case 'question':
      return $data->question_id;

    case 'exercise':
      return $data->exercise_id;

    case 'unit':
      return $data->unit_id;

    case 'course':
      return $data->course_id;

    case 'score_formatted':
      return t('@score correct', array('@score' => (int) ($data->score * 100) . '%'));

    case 'progress_formatted':
      return t('@progress complete', array('@progress' => (int) ($data->score * 100) . '%'));

    case 'given_answer':
    case 'feedback':
      drupal_add_css(drupal_get_path('module', 'elearning') . '/theme/elearning.question.css');
      $variables = array();
      $question = elearning_question_load($data->question_id);
      $question_wrapper = entity_metadata_wrapper('elearning_question', $question);
      $variables['question'] = $question;
      $variables['correct_answer'] = $question->answer[LANGUAGE_NONE];
      $variables['given_answer'] = unserialize($data->given_answer);
      $variables['correct'] = $data->correct;
      return theme('elearning_question_' . $name . '_' . $question->type, $variables);
  }
}

/**
 * Callback for Elearning entity label.
 *
 * @see hook_elearning_entity_label()
 */
function elearning_entity_label($entity) {
  $label = check_plain($entity->label);
  foreach (module_implements('elearning_entity_label') as $module) {
    $label = module_invoke($module, 'elearning_entity_label', $entity);
    if (!empty($label)) {
      return $label;
    }
  }
  return $label;
}

/**
 * Callback for loading courses.
 */
function elearning_course_load($course_id, $reset = FALSE) {
  $courses = elearning_course_load_multiple(array($course_id), array(), $reset);
  return reset($courses);
}

/**
 * Callback for loading multiple courses.
 */
function elearning_course_load_multiple($course_ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('elearning_course', $course_ids, $conditions, $reset);
}

/**
 * Callback for loading a single unit.
 */
function elearning_unit_load($unit_id, $reset = FALSE) {
  $units = elearning_unit_load_multiple(array($unit_id), array(), $reset);
  return reset($units);
}

/**
 * Callback for loading multiple units.
 */
function elearning_unit_load_multiple($unit_ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('elearning_unit', $unit_ids, $conditions, $reset);
}

/**
 * Callback for loading unit results.
 */
function elearning_unit_result_load($unit_result_id, $reset = FALSE) {
  $unit_results = entity_load('elearning_unit_result', array($unit_result_id));
  return reset($unit_results);
}

/**
 * Callback for loading a single exercise.
 */
function elearning_exercise_load($exercise_id, $reset = FALSE) {
  $exercises = elearning_exercise_load_multiple(array($exercise_id), array(), $reset);
  return reset($exercises);
}

/**
 * Callback for loading a multiple exercises.
 */
function elearning_exercise_load_multiple($exercise_ids = array(), $conditions = array(), $reset = FALSE) {
  $entities = entity_load('elearning_exercise', $exercise_ids, $conditions, $reset);
  return $entities;
}

/**
 * Callback for loading course results.
 */
function elearning_exercise_result_load($exercise_result_id, $reset = FALSE) {
  return entity_load_single('elearning_exercise_result', $exercise_result_id);
}

/**
 * Callback for loading a single question.
 */
function elearning_question_load($question_id, $reset = FALSE) {
  $questions = elearning_question_load_multiple(array($question_id), array(), $reset);
  return reset($questions);
}

/**
 * Callback for loading a multiple questions.
 */
function elearning_question_load_multiple($question_ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('elearning_question', $question_ids, $conditions, $reset);
}
