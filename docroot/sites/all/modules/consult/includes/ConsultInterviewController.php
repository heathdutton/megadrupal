<?php
/**
 * @file
 * Interview controller class.
 */

class ConsultInterviewController extends EntityAPIControllerExportable {

  /**
   * Get all questions attached to a Interview.
   *
   * @param ConsultInterview $entity
   *   The interview entity to get questions of.
   *
   * @return array
   *   All questions keyed by name.
   */
  public function getQuestions(ConsultInterview $entity) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', CONSULT_QUESTION_ENTITY_NAME)
      ->propertyCondition('interview_name', $entity->identifier());

    $results = $query->execute();
    if (!empty($results)) {
      return entity_load(CONSULT_QUESTION_ENTITY_NAME, array_keys($results[CONSULT_QUESTION_ENTITY_NAME]));
    }
    else {
      return array();
    }
  }

  /**
   * Get all results attached to a Interview.
   *
   * @param ConsultInterview $entity
   *   The interview entity to get results of.
   *
   * @return array
   *   All results keyed by name.
   */
  public function getResults(ConsultInterview $entity) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', CONSULT_RESULT_ENTITY_NAME)
      ->propertyCondition('interview_name', $entity->identifier());

    $results = $query->execute();
    if (!empty($results)) {
      return entity_load(CONSULT_RESULT_ENTITY_NAME, array_keys($results[CONSULT_RESULT_ENTITY_NAME]));
    }
    else {
      return array();
    }
  }

  /**
   * Set the sub entity settings tied to an interview.
   *
   * @param ConsultInterview $interview
   *   The interview to set the values for.
   * @param ConsultInterviewSubEntity $sub_entity
   *   The sub entity.
   * @param int $weight
   *   The weight to set.
   * @param int $enabled
   *   The enabled state.
   */
  public function setSubEntityValue(ConsultInterview $interview, ConsultInterviewSubEntity $sub_entity, $weight, $enabled) {
    $interview->settings[$sub_entity->entityType()][$sub_entity->identifier()] = array(
      'weight' => $weight,
      'enabled' => $enabled,
    );
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * Overridden to use custom theme hook implementation.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    $view = parent::view($entities, $view_mode, $langcode, $page);

    foreach ($view[$this->entityType] as &$build) {
      // Create blank consult array for data injection.
      $build['#attached']['js'][] = array(
        'data' => array(
          'consult' => array(),
        ),
        'type' => 'setting',
      );

      // Perform cached Javascript data injection.
      drupal_add_js('consult/' . $build['#entity']->identifier() . '/js-settings', array(
        'scope' => 'footer',
        'preprocess' => FALSE,
      ));
    }

    return $view;
  }

  /**
   * Get JavaScript settings object array for a given interview.
   *
   * @param ConsultInterview $entity
   *   The Interview to retrieve the JS settings for.
   *
   * @return array
   *   The JavaScript settings array.
   */
  public function getJsSettings(ConsultInterview $entity) {
    $questions = array();
    $results = array();

    foreach ($entity->getQuestions() as $delta => $question) {
      if (!$question->isEnabled()) {
        continue;
      }

      // Keep the answers in array format to force a correct order in js.
      $answer_options = array();
      foreach ($question->getAnswers() as $key => $label) {
        $answer_options[] = array(
          'key' => $key,
          'label' => $label,
        );
      }

      $questions[$delta] = array(
        'name' => $question->identifier(),
        'label' => $question->label(),
        'markup' => render($question->view()),
        'answers' => $answer_options,
        'dependencies' => $question->getDependencyOutput(),
        'type' => $question->getGroupName(),
        'shown' => $question->getShown(),
      );
    }

    foreach ($entity->getResults() as $delta => $result) {
      if (!$result->isEnabled()) {
        continue;
      }

      $results[$delta] = array(
        'name' => $result->identifier(),
        'label' => $result->label(),
        'markup' => render($result->view()),
        'dependencies' => $result->getDependencyOutput(),
        'type' => $result->getGroupName(),
        'shown' => $result->getShown(),
      );
    }

    drupal_alter('consult_js_results', $results);
    drupal_alter('consult_js_questions', $questions);

    return array(
      $entity->identifier() => array(
        'questions' => array_values($questions),
        'results' => array_values($results),
      ),
    );
  }

  /**
   * Delete all sub entities.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if ($entities) {
      foreach ($entities as $entity) {
        $questions = $entity->getQuestions();
        $results = $entity->getResults();

        foreach ($questions as $question) {
          $question->delete();
        }

        foreach ($results as $result) {
          $result->delete();
        }
      }
      parent::delete($ids, $transaction);
    }
  }
}
