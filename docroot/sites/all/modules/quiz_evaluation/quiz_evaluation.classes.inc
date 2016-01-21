<?php

/**
 * @file
 * The EvaluationQuestion and EvaluationResponse classes for use with the quiz
 * module.
 */

/**
 * Extension of QuizQuestion.
 */
class EvaluationQuestion extends QuizQuestion {

  public $collection_id;
  public $range_id;

  public function delete($only_this_version = false) {

    $collection_id = db_query('SELECT collection_id FROM {quiz_evaluation_quiz}
      WHERE nid = :nid AND vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid))->fetchAssoc();
    $collection_id = $collection_id['collection_id'];

    db_delete('quiz_evaluation_quiz')
      ->condition('vid', $this->node->nid)
      ->execute();

    db_delete('quiz_evaluation_question')
      ->condition('collection_id', $collection_id)
      ->execute();

    db_delete('quiz_evaluation_user_score')
      ->condition('collection_id', $collection_id)
      ->execute();

    parent::delete($only_this_version);
  }

  /**
   * Validate the question creation form.
   *
   * @param $form
   *    The current form state.
   *
   * @see /quiz/question_types/quiz_question/quiz_question.core.inc,
   * QuizQuestion::validateNode(array &$form)
   */
  public function validateNode(array &$form) {
    foreach ($this->node->quiz_evaluation_answers as $id => $answer) {
      if (isset($answer['question_id']) && strlen($answer['title']) > 256) {
        form_set_error('quiz_evaluation_answers][' . $id . '][title', t('Please shorten your answer text.'));
      }
    }
  }

  /**
   * Generate the answering form so the user can answer the questions.
   *
   * @param $form_state
   *    The current form state.
   * @param $rid
   *    The current response ID.
   *
   * @return
   *    The answering form
   */
  public function getAnsweringForm(array $form_state = NULL, $rid) {
    $form = parent::getAnsweringForm($form_state, $rid);

    // This must be called 'tries'
    $form['tries'] = array(
      '#options' => array(),
      '#theme' => 'quiz_evaluation_answering',
    );

    $range = _quiz_evaluation_range_lookup($this->node->quiz_evaluation_range);
    $form['tries']['#options']['quiz_evaluation_range'] = $range;
    $answers = $this->node->quiz_evaluation_answers;
    $form['tries']['#options']['quiz_evaluation_answers'] = $answers;

    global $user;

    // Load previous answers for this question.
    $results = db_query("SELECT question_id, value_id FROM {quiz_evaluation_user_score} WHERE rid = :rid AND uid = :uid AND collection_id = :collection_id;", array(':rid' => $rid, ':uid' => $user->uid, ':collection_id' => $this->node->quiz_evaluation_collection_id))->fetchAllAssoc('question_id');

    foreach ($answers as $answer) {
      $form['tries']['title_' . $answer['question_id']] = array(
        '#markup' => $answer['title']
      );

      foreach ($range as $item) {
        $form['tries']['answer_' . $answer['question_id'] . '_' . $item->value_id] = array(
          '#type' => 'radio',
          '#name' => 'tries[answer_' . $answer['question_id'] . ']',
          '#return_value' => $item->value_id,
        );

        if (isset($form_state['input']['answer_' . $answer['question_id']]) && $form_state['input']['answer_' . $answer['question_id']] == $item->value_id) {
          // Only run if the user has submitted the form, regardless as to whether they have a valid submission or not.

          // Delete the entries for this question before inserting it all again.
          db_delete('quiz_evaluation_user_score')
            ->condition('rid', $rid)
            ->condition('uid', $user->uid)
            ->condition('question_id', $answer['question_id'])
            ->condition('collection_id', $this->node->quiz_evaluation_collection_id)
            ->execute();
          // Insert the current answers for this question
          db_insert('quiz_evaluation_user_score')
            ->fields(array(
              'rid' => $rid,
              'uid' => $user->uid,
              'collection_id' => $this->node->quiz_evaluation_collection_id,
              'question_id' => $answer['question_id'],
              'value_id' => $item->value_id
            ))
            ->execute();
        }
        elseif (in_array($answer['question_id'], array_keys($results))
          && isset($results[$answer['question_id']])
          && $results[$answer['question_id']]->value_id == $item->value_id) {
          $form['tries']['answer_' . $answer['question_id'] . '_' . $item->value_id]['#default_value'] = $item->value_id;
        }
      }
    }

    $form['rebuild'] = FALSE;

    return $form;
  }

  /**
   * Create a form that allows the evaluation questions to be created.
   *
   * @param $form_state
   *     The current form state.
   *
   * @return
   *    The evaluation question creation form.
   *
   * @see /quiz/question_types/quiz_question/quiz_question.core.inc,
   * QuizQuestion::getCreationForm(array &$form_state = NULL)
   */
  public function getCreationForm(array &$form_state = NULL) {
    $form = array();

    // We add #action to the form because of the use of ajax
    $options = array();
    $get = $_GET;
    unset($get['q']);
    if (!empty($get)) {
      $options['query'] = $get;
    }

    $ranges = _quiz_evaluation_get_ranges();
    $options = array();
    foreach ($ranges as $range) {
      $options[$range->range_id] = $range->min . ' > ' . $range->max;
    }

    $form['quiz_evaluation_range'] = array(
      '#type' => 'select',
      '#title' => 'Select a range for each answer',
      '#options' => $options,
      '#default_value' => (isset($this->node->quiz_evaluation_range)) ? $this->node->quiz_evaluation_range : ''
    );

    $form['quiz_evaluation_answers'] = array(
      '#type' => 'fieldset',
      '#title' => t('Answers'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $i = 0;

    // choice_count might be stored in the form_state after an ajax callback
    if (isset($form_state['answer_count'])) {
      $choice_count = $form_state['answer_count'];
    }
    else {
      $choice_count = max(2, isset($this->node->quiz_evaluation_answers) ? count($this->node->quiz_evaluation_answers) : 0);
    }

    for (; $i < $choice_count; $i++) {
      $short = isset($this->node->quiz_evaluation_answers[$i]) ? $this->node->quiz_evaluation_answers[$i] : NULL;
      $form['quiz_evaluation_answers'][$i] = array(
        '#type' => 'fieldset',
        '#title' => t('Answer !i', array('!i' => ($i + 1))),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['quiz_evaluation_answers'][$i]['title'] = array(
        '#type' => 'textarea',
        '#title' => t('Answer !i Text', array('!i' => ($i + 1))),
        '#default_value' => $short['title']
      );
      $form['quiz_evaluation_answers'][$i]['question_id'] = array(
        '#type' => 'value',
        '#value' => $short['question_id']
      );
    }

    // ahah helper tag. New questions will be inserted before this tag
    $form['quiz_evaluation_answers']["placeholder"] = array(
      '#markup' => '<div id="placeholder"></div>',
    );

    // We can't send the get values to the ahah callback the normal way, so we do it like this.
    $form['get'] = array(
      '#type' => 'value',
      '#value' => $get,
    );

    $form['quiz_evaluation_answers']['evaluation_add_answers'] = array(
      '#type' => 'submit',
      '#value' => t('Add more answers'),
      '#submit' => array('quiz_evaluation_more_choices_submit'), // If no javascript action.
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'quiz_evaluation_add_alternative_ajax_callback',
        'wrapper' => 'placeholder',
        'effect' => 'slide',
        'method' => 'before',
      ),
    );

    return $form;
  }

  /**
   * Implementation of saveNodeProperties
   *
   * @see /quiz/question_types/quiz_question/quiz_question.core.inc
   */
  public function saveNodeProperties($is_new = FALSE) {
    /**
     * Tables to insert data into:
     *
     *  quiz_evaluation_quiz
     *   - collection_id
     *   - nid
     *   - vid
     *   - title
     *   - range_id
     *
     *  quiz_evaluation_questions
     *   - question_id
     *   - collection_id
     *   - title
     *
     *  quiz_evaluation_range
     *  - value_id
     *  - range_id
     *  - value
     *  - score
     */
    if ($is_new || $this->node->revision == 1) {
      $collection_id = db_insert('quiz_evaluation_quiz')
        ->fields(array(
          'nid' => $this->node->nid,
          'vid' => $this->node->vid,
          'range_id' => $this->node->quiz_evaluation_range
        ))
        ->execute();

      $insert = db_insert('quiz_evaluation_question')
        ->fields(array('collection_id', 'title'));

      foreach ($this->node->quiz_evaluation_answers as $question) {
        if (is_array($question) && isset($question['title']) && $question['title'] != '') {
          $values = array(
            'collection_id' => $collection_id,
            'title' => $question['title'],
          );
          $insert->values($values);
        }
      }
      $insert->execute();
    }
    else {

      db_update('quiz_evaluation_quiz')
        ->fields(array(
          'range_id' => $this->node->quiz_evaluation_range
        ))
        ->condition('nid', $this->node->nid)
        ->condition('vid', $this->node->vid)
        ->execute();

      $collection_id = db_query('SELECT collection_id FROM {quiz_evaluation_quiz}
        WHERE nid = :nid AND vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid))->fetchAssoc();
      $collection_id = $collection_id['collection_id'];

      foreach ($this->node->quiz_evaluation_answers as $question) {
        if (is_array($question) && isset($question['title']) && $question['title'] != '') {
          if (isset($question['question_id'])) {
            db_update('quiz_evaluation_question')
              ->fields(array(
                'title' => $question['title']
              ))
              ->condition('question_id', $question['question_id'])
              ->condition('collection_id', $collection_id)
              ->execute();
          }
          else {
            $insert = db_insert('quiz_evaluation_question')
              ->fields(array('collection_id', 'title'))
              ->values(array(
                'collection_id' => $collection_id,
                'title' => $question['title'],
              ))
              ->execute();
          }
        }
      }
    }
  }

  /**
   * Implementation of getNodeProperties
   *
   * @see /quiz/question_types/quiz_question/quiz_question.core.inc
   */
  public function getNodeProperties() {
    if (isset($this->nodeProperties) && !empty($this->nodeProperties)) {
      return $this->nodeProperties;
    }

    $props = parent::getNodeProperties();

    $evaluation = db_query('SELECT collection_id, range_id FROM {quiz_evaluation_quiz}
            WHERE nid = :nid AND vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid))->fetchAssoc();

    $props['quiz_evaluation_collection_id'] = $evaluation['collection_id'];

    $props['quiz_evaluation_range'] = $evaluation['range_id'];

    $answers_results = db_query('SELECT question_id, title FROM {quiz_evaluation_question}
            WHERE collection_id = :collection_id', array(':collection_id' => $evaluation['collection_id']));

    $answers = array();
    while ($answer = $answers_results->fetchAssoc()) {
      $answers[] = array(
        'question_id' => $answer['question_id'],
        'title' => $answer['title']
      );
    }

    $props['quiz_evaluation_answers'] = $answers;

    $this->nodeProperties = $props;
    return $props;
  }

  /**
   * Implementation of getMaximumScore.
   *
   * @see /quiz/question_types/quiz_question/quiz_question.core.inc
   */
  public function getMaximumScore() {
    // need to return the upper limit of the range of questions
    $total_score = 0;
    $range = _quiz_evaluation_range_lookup($this->node->quiz_evaluation_range);
    foreach ($range as $value) {
      $total_score = max($value->score, $total_score);
    }
    return $total_score * (count($this->node->quiz_evaluation_answers) - 1);
  }

}

/**
 * Extension of QuizQuestionResponse
 */
class EvaluationResponse extends QuizQuestionResponse {

  /**
   * ID of the answer.
   */
  protected $answer_id = 0;

  /**
   * Save the current response.
   */
  public function save() {
    global $user;

    db_delete('quiz_evaluation_user_score')
      ->condition('rid', $this->rid)
      ->condition('uid', $user->uid)
      ->condition('collection_id', $this->question->quiz_evaluation_collection_id)
      ->execute();

    foreach ($this->answer as $answer_id => $value_id) {
      if (preg_match('/^(answer_\d+)$/', $answer_id) == 1) {
        $answer_id = drupal_substr($answer_id, strpos($answer_id, '_') + 1);
        // Insert the current answers for this question
        db_insert('quiz_evaluation_user_score')
          ->fields(array(
            'rid' => $this->rid,
            'uid' => $user->uid,
            'collection_id' => $this->question->quiz_evaluation_collection_id,
            'question_id' => $answer_id,
            'value_id' => $value_id
          ))
          ->execute();
      }
    }
  }

  /**
   * Delete the response.
   */
  public function delete() {
    global $user;

    db_delete('quiz_evaluation_user_score')
      ->condition('rid', $this->rid)
      ->condition('uid', $user->uid)
      ->condition('collection_id', $this->question->quiz_evaluation_collection_id)
      ->execute();
  }

  /**
   * Calculate the score for the response.
   *
   * The score is worked out by adding the different values together.
   * So for three questions in a collection the score would be the sum
   * of the scores for each of the answers.
   *
   * @return
   *    The total score of the answer as an integer.
   */
  public function score() {
    global $user;

    $total_score = 0;
    $options = array(
      ':rid' => $this->rid,
      ':collection_id' => $this->question->quiz_evaluation_collection_id
    );
    $results = db_query("SELECT answer_id, value_id
FROM {quiz_evaluation_user_score}
WHERE rid = :rid AND collection_id = :collection_id;", $options)
      ->fetchAllAssoc('answer_id');
    $range = _quiz_evaluation_range_lookup($this->question->quiz_evaluation_range);
    foreach ($results as $result) {
      foreach ($range as $value) {
        if ($result->value_id == $value->value_id) {
          $total_score += $value->score;
        }
      }
    }
    return $total_score;
  }

  /**
   * Get the user's responses for a group of questions.
   *
   * @return
   *    An associative array containing:
   *      - question_id: The id of the question.
   *      - value_id: The id of the range value answered.
   */
  public function getResponse() {
    global $user;
    $options = array(
      ':rid' => $this->rid,
      ':collection_id' => $this->question->quiz_evaluation_collection_id
    );

    $results = db_query("SELECT question_id, value_id
      FROM {quiz_evaluation_user_score}
      WHERE rid = :rid
      AND collection_id = :collection_id;", $options)
      ->fetchAllAssoc('question_id');

    return $results;
  }

  /**
   * It is be impossible to get question wrong so we always return true.
   *
   * @return
   *    Always return TRUE.
   *
   * @see /quiz/question_types/quiz_question/quiz_question.core.inc,
   * isCorrect()
   */
  public function isCorrect() {
    return TRUE;
  }

  /**
   * Validate the user submitted form.
   *
   * We would normally return any string to invalidate the submission. In this
   * case there is no need because if a user doesn't answer a question they
   * get a score of 0.
   *
   * @return
   *    Always return TRUE.
   */
  public function isValid() {
    if (count($this->answer) != count($this->question->quiz_evaluation_answers)) {
      return t('You must provide an answer to all parts of the evaluation.');
    }
    return TRUE;
  }

  /**
   * Implementation of getReportFormResponse
   *
   * @see /quiz/question_types/quiz_question/quiz_question.core.inc,
   * getReportFormResponse($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE)
   */
  public function getReportFormResponse($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    $range = _quiz_evaluation_range_lookup($this->question->quiz_evaluation_range);

    $results = $this->getResponse();

    $header = array('Question', 'Your Answer');

    $rows = array();

    // Render the answers as a table.
    $answers = $this->question->quiz_evaluation_answers;
    foreach ($answers as $answer) {

      foreach ($range as $item) {
        if (in_array($answer['question_id'], array_keys($results))
          && isset($results[$answer['question_id']])
          && $results[$answer['question_id']]->value_id == $item->value_id) {
          $rows[] = array(t($answer['title']), t($item->value));
          break;
        }
      }
    }

    
    $table = theme('table', array('header' => $header, 'rows' => $rows));
    $script = "jQuery('#evaluation_answer_click_" . $answer['question_id'] . "').click(function() {
  jQuery('#evaluation_answer_" . $answer['question_id'] . "').slideToggle('slow', function() {
    
  });
  return false;
});
jQuery('#evaluation_answer_" . $answer['question_id'] . "').css('display', 'none')";
    drupal_add_js($script, array('type' => 'inline', 'group' => JS_LIBRARY+1));
    
    $markup = '<button id="evaluation_answer_click_' . $answer['question_id'] . '">'. t('Show Answers') . ' &raquo;</button>';
    $markup .= '<div id="evaluation_answer_' . $answer['question_id'] . '">' . $table . '</div>';
    
    return array('#markup' => $markup);
  }
}