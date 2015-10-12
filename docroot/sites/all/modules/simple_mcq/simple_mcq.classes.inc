<?php


/**
 * The main classes for the simple_mcq question type.
 *
 * These inherit or implement code found in quiz_question.classes.inc.
 *
 * Based on:
 * Other question types in the quiz framework.
 *
 *
 *
 * @file
 * Question type, enabling the creation of multiple choice and multiple answer questions.
 */

/**
 * Extension of QuizQuestion.
 */
class SimpleMCQQuestion extends QuizQuestion {

  /**
   * Forgive some possible logical flaws in the user input.
   */
  private function forgive() {
    if ($this->node->choice_multi == 1) {
      for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
        $short = &$this->node->alternatives[$i];
        $node = &$this->node;
        // If the scoring data doesn't make sense, use the data from the "correct" checkbox to set the score data
        if ($node->correct_score == 0 || !is_numeric($node->correct_score)) {
          if ($short['correct'] == 1) {
            $node->correct_score = 1;
          }
        }
      }
    }
    else {
      // For questions with one, and only one, correct answer, there will be no points awarded for alternatives
      // not chosen.
      for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
        $short = &$this->node->alternatives;
        // We get the entire node to see what will be score and feedback
        $node = &$this->node;
        if (isset($short['correct']) && $short['correct'] == 1 && !_quiz_is_int($node->correct_score, 1)) {
          $node->correct_score = 1;
        }
      }
    }
  }

  /**
   * Warn the user about possible user errors
   */
  private function warn() {
    // Count the number of correct answers

    $num_corrects = 0;
    for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
      $alt = &$this->node->alternatives[$i];
      $node = &$this->node;
      if ($alt['correct'] > 0) {
        $num_corrects++;
      }
    }
    if ($num_corrects == 1 && $this->node->choice_multi == 1 || $num_corrects > 1 && $this->node->choice_multi == 0) {
      $link_options = array();
      if (isset($_GET['destination'])) {
        $link_options['query'] = array('destination' => $_GET['destination']);
      }
      $go_back = l(t('go back'), 'node/' . $this->node->nid . '/edit', $link_options);
      if ($num_corrects == 1) {
        drupal_set_message(
          t('Your question allows multiple answers. Only one of the alternatives have been marked as correct. If this wasn\'t intended please !go_back and correct it.',
            array('!go_back' => $go_back)),
          'warning');
      }
      else {
        drupal_set_message(
          t('Your question doesn\'t allow multiple answers. More than one of the alternatives have been marked as correct. If this wasn\'t intended please !go_back and correct it.',
            array('!go_back' => $go_back)),
          'warning');
      }
    }
  }

  /**
   * Run check_markup() on the field of the specified choice alternative
   * @param $alternativeIndex
   *  The index of the alternative in the alternatives array.
   * @param $field
   *  The name of the field we want to check markup on
   * @param $check_user_access
   *  Whether or not to check for user access to the filter we're trying to apply
   * @return HTML markup
   */
  private function checkMarkup($alternativeIndex, $field, $check_user_access = FALSE) {
    $alternative = $this->node->alternatives[$alternativeIndex];
    return check_markup($alternative[$field]['value'], $alternative[$field]['format']);
  }

  /**
   * Implementation of save
   *
   * Stores the question in the database.
   *
   * @param is_new if - if the node is a new node...
   * (non-PHPdoc)
   * @see sites/all/modules/quiz-HEAD/question_types/quiz_question/QuizQuestion#save()
   */
  public function saveNodeProperties($is_new = FALSE) {
    $is_new = $is_new || $this->node->revision == 1;

    // Before we save we forgive some possible user errors
    //$this->forgive();

    // We also add warnings on other possible user errors
    //$this->warn();

    if ($is_new) {
      $id = db_insert('quiz_simple_mcq_properties')
        ->fields(array(
          'nid' => $this->node->nid,
          'vid' => $this->node->vid,
          'choice_random' => $this->node->choice_random,
          'feedback' => $this->node->feedback['value'],
          'feedback_format' => $this->node->feedback['format'],
          'score_correct' => $this->node->score_correct,
          'score_incorrect' => $this->node->score_incorrect
        ))
        ->execute();

      // TODO: utilize the benefit of multiple insert of DBTNG
      for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
        if (drupal_strlen($this->node->alternatives[$i]['answer']['value']) > 0) {
          $this->insertAlternative($i);
        }
      }
    }
    else {
      db_update('quiz_simple_mcq_properties')
        ->fields(array(
          'choice_random' => $this->node->choice_random,
          'feedback' => $this->node->feedback['value'],
          'feedback_format' => $this->node->feedback['format'],
          'score_correct' => $this->node->score_correct,
          'score_incorrect' => $this->node->score_incorrect
        ))
        ->condition('nid', $this->node->nid)
        ->condition('vid', $this->node->vid)
        ->execute();

      // We fetch ids for the existing answers belonging to this question
      // We need to figure out if an existing alternative has been changed or deleted.
      $res = db_query('SELECT id FROM {quiz_simple_mcq_answers}
              WHERE question_nid = :nid AND question_vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid));

      // We start by assuming that all existing alternatives needs to be deleted
      $ids_to_delete = array();
      while ($res_o = $res->fetch()) {
        $ids_to_delete[] = $res_o->id;
      }

      for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
        $short = $this->node->alternatives[$i];
        if (drupal_strlen($this->node->alternatives[$i]['answer']['value']) > 0) {
          // If new alternative
          if (!is_numeric($short['id'])) {
            $this->insertAlternative($i);
          }
          // If existing alternative
          else {
            $this->updateAlternative($i);
            // Make sure this alternative isn't deleted
            $key = array_search($short['id'], $ids_to_delete);
            $ids_to_delete[$key] = FALSE;
          }
        }
      }
      foreach ($ids_to_delete as $id_to_delete) {
        if ($id_to_delete) {
          db_delete('quiz_simple_mcq_answers')
            ->condition('id', $id_to_delete)
            ->execute();
        }
      }
    }
    //$this->saveUserSettings();
  }

  /**
   * Helper function. Saves new alternatives
   *
   * @param $i
   *  The alternative index
   */
  private function insertAlternative($i) {
    $alternatives = $this->node->alternatives[$i];
    $node = $this->node;
    db_insert('quiz_simple_mcq_answers')
      ->fields(array(
        'answer' => $alternatives['answer']['value'],
        'answer_format' => $alternatives['answer']['format'],
        'correct' => $alternatives['correct'],
        'question_nid' => $this->node->nid,
        'question_vid' => $this->node->vid
      ))
      ->execute();
  }

  /**
   * Helper function. Updates existing alternatives
   *
   * @param $i
   *  The alternative index
   */
  private function updateAlternative($i) {
    $alternatives = $this->node->alternatives[$i];
    db_update('quiz_simple_mcq_answers')
      ->fields(array(
        'answer' => $alternatives['answer']['value'],
        'answer_format' => $alternatives['answer']['format'],
        'correct' => $alternatives['correct'],
      ))
      ->condition('id', $alternatives['id'])
      ->condition('question_nid', $this->node->nid)
      ->condition('question_vid', $this->node->vid)
      ->execute();
  }

  /**
   * Implementation of validate
   *
   * QuizQuestion#validate()
   */
  public function validateNode(array &$form) {
    $found_one_correct = FALSE;
    for ($i = 0; (isset($this->node->alternatives[$i]) && is_array($this->node->alternatives[$i])); $i++) {
      $short = $this->node->alternatives[$i];
      if (drupal_strlen($this->checkMarkup($i, 'answer')) < 1) {
        continue;
      }
      if ($short['correct'] == 1) {
        if ($found_one_correct) {
          // We don't display an error message here since we allow alternatives to be partially correct
        }
        else {
          $found_one_correct = TRUE;
        }
      }
    }
    if (!$found_one_correct) {
      form_set_error('alternatives', t('You have not marked any alternatives as correct.'));
    }
  }

  /**
   * Implementation of delete
   *
   * @see QuizQuestion#delete()
   */
  public function delete($only_this_version = FALSE) {
    $delete_properties = db_delete('quiz_simple_mcq_properties')->condition('nid', $this->node->nid);
    $delete_answers = db_delete('quiz_simple_mcq_answers')->condition('question_nid', $this->node->nid);
    $delete_results = db_delete('quiz_simple_mcq_user_answers')->condition('question_nid', $this->node->nid);

    if ($only_this_version) {
      $delete_properties->condition('vid', $this->node->vid);
      $delete_answers->condition('question_vid', $this->node->vid);
      $delete_results->condition('question_vid', $this->node->vid);
    }

    // Delete from table quiz_simple_mcq_user_answer_multi
    if ($only_this_version) {
      $user_answer_id = db_query('SELECT id FROM {quiz_simple_mcq_user_answers} WHERE question_nid = :nid AND question_vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid))->fetchCol();
    }
    else {
      $user_answer_id = db_query('SELECT id FROM {quiz_simple_mcq_user_answers} WHERE question_nid = :nid', array(':nid' => $this->node->nid))->fetchCol();
    }

    if (count($user_answer_id)) {
      db_delete('quiz_simple_mcq_user_answer_multi')
        ->condition('user_answer_id', $user_answer_id, 'IN')
        ->execute();
    }
    $delete_properties->execute();
    $delete_answers->execute();
    $delete_results->execute();
    parent::delete($only_this_version);
  }

  /**
   * Implementation of getNodeProperties
   *
   * @see QuizQuestion#getNodeProperties()
   */
  public function getNodeProperties() {
    if (isset($this->nodeProperties) && !empty($this->nodeProperties)) {
      return $this->nodeProperties;
    }
    $props = parent::getNodeProperties();

    $res_a = db_query('SELECT choice_random, feedback, feedback_format, score_correct, score_incorrect FROM {quiz_simple_mcq_properties} WHERE nid = :nid AND vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid))->fetchAssoc();

    if (is_array($res_a)) {
      $props = array_merge($props, $res_a);
    }

    // Load the answers
    $res = db_query('SELECT id, answer, answer_format, correct FROM {quiz_simple_mcq_answers} WHERE question_nid = :question_nid AND question_vid = :question_vid ORDER BY id', array(':question_nid' => $this->node->nid, ':question_vid' => $this->node->vid));
    $props['alternatives'] = array(); // init array so it can be iterated even if empty
    while ($res_arr = $res->fetchAssoc()) {
      $props['alternatives'][] = $res_arr;
    }
    $this->nodeProperties = $props;
    return $props;
  }

  /**
   * Implementation of getNodeView
   *
   * @see QuizQuestion#getNodeView()
   */
  public function getNodeView() {
    $content = parent::getNodeView();
    if ($this->node->choice_random) {
      $this->shuffle($this->node->alternatives);
    }
    $content['answers'] = array(
       '#markup' => theme('simple_mcq_answer_node_view', array('alternatives' => $this->node->alternatives, 'show_correct' => $this->viewCanRevealCorrect())),
       '#weight' => 2,
    );

    return $content;
  }

  /**
   * Generates the question form.
   *
   * This is called whenever a question is rendered, either
   * to an administrator or to a quiz taker.
   */
  public function getAnsweringForm(array $form_state = NULL, $rid) {
    $form = parent::getAnsweringForm($form_state, $rid);
    $form['tries[answer]'] = array(
      '#options' => array(),
      '#theme' => 'simple_mcq_alternative',
    );
    if (isset($rid)) {
      // This question has already been answered. We load the answer.
      $response = new SimpleMCQResponse($rid, $this->node);
    }
    for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
      $short = $this->node->alternatives[$i];
      $answer_markup = check_markup($short['answer'], $short['answer_format']);
      if (drupal_strlen($answer_markup) > 0) {
        $form['tries[answer]']['#options'][$short['id']] = $answer_markup;
      }
    }
    if ($this->node->choice_random) {
      // We save the choice order so that the order will be the same in the answer report
      $form['tries[choice_order]'] = array(
        '#type' => 'hidden',
        '#value' => implode(',', $this->shuffle($form['tries[answer]']['#options'])),
      );
    }
    $no_of_choices = 0;
    foreach ($this->node->alternatives as $alternative) {
      if ($alternative['correct']) {
        $no_of_choices++;
      }
    }
    if ($no_of_choices > 1) {
      $form['tries[answer]']['#type'] = 'checkboxes';
      $form['tries[answer]']['#title'] = t('Choose');
      if (isset($response)) {
        if (is_array($response->getResponse())) {
          $form['tries[answer]']['#default_value'] = $response->getResponse();
        }
      }
    }
    else {
      $form['tries[answer]']['#type'] = 'radios';
      $form['tries[answer]']['#title'] = t('Choose one');
      if (isset($response)) {
        $selection = $response->getResponse();
        if (is_array($selection)) {
          $form['tries[answer]']['#default_value'] = array_pop($selection);
        }
      }
    }

    return $form;
  }

  /**
   * Custom shuffle function. It keeps the array key - value relationship intact
   *
   * @param array $array
   * @return unknown_type
   */
  private function shuffle(array &$array) {
    $newArray = array();
    $toReturn = array_keys($array);
    shuffle($toReturn);
    foreach ($toReturn as $key) {
      $newArray[$key] = $array[$key];
    }
    $array = $newArray;
    return $toReturn;
  }

  /**
   * Implementation of getCreationForm
   *
   * @see QuizQuestion#getCreationForm()
   */
  public function getCreationForm(array &$form_state = NULL) {
    $form = array();

    $form['alternatives'] = array(
      '#type' => 'fieldset',
      '#title' => t('Alternatives'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => -4,
      '#tree' => TRUE,
    );

    // Add helper tag where we will place the input selector for all the textareas.
    $form['alternatives']['input_format_all'] = array(
      '#markup' => '<DIV id="input-all-ph"></DIV>',
    );

    $form['alternatives']['#theme'][] = 'simple_mcq_creation_form';

    // choice_count might be stored in the form_state after an ajax callback
    if (isset($form_state['choice_count'])) {
      $choice_count = $form_state['choice_count'];
    }
    else {
      $choice_count = max(variable_get('simple_mcq_def_num_of_alts', 4), isset($this->node->alternatives) ? count($this->node->alternatives) : 0);
    }
    for ($i = 0; $i < $choice_count; $i++) {
      $short = isset($this->node->alternatives[$i]) ? $this->node->alternatives[$i] : NULL;
      $form['alternatives'][$i] = array(
        '#type' => 'textarea',
        '#title' => t('Alternative !i', array('!i' => ($i + 1))),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        // - The two first alternatives won't be collapsed.
        // - Populated alternatives won't be collapsed
      );
      $form['alternatives'][$i]['#theme'][] = 'simple_mcq_alternative_creation';
      $form['alternatives'][$i]['correct'] = array(
        '#type' => 'checkbox',
        '#title' => t('Correct'),
        '#default_value' => $short['correct'],
      );
      // We add id to be able to update the correct alternatives if the node is updated, without destroying
      // existing answer reports
      $form['alternatives'][$i]['id'] = array(
        '#type' => 'value',
        '#value' => $short['id'],
      );
      $form['alternatives'][$i]['answer'] = array(
        '#type' => 'text_format',
        '#base_type' => 'textarea',
        '#title' => t('Alternative !i', array('!i' => ($i + 1))),
        '#default_value' => $short['answer'],
        '#required' => $i < 2,
        '#format' => isset($short['answer_format']) ? $short['answer_format'] : NULL,
        '#rows' => 3,
      );
    }
    // ahah helper tag. New questions will be inserted before this tag
    $form['alternatives']["placeholder"] = array(
      '#markup' => '<div id="placeholder"></div>',
    );

    $form['alternatives']['simple_mcq_add_alternative'] = array(
      '#type' => 'submit',
      '#value' => t('Add more alternatives'),
      '#submit' => array('simple_mcq_more_choices_submit'), // If no javascript action.
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'simple_mcq_add_alternative_ajax_callback',
        'wrapper' => 'placeholder',
        'effect' => 'slide',
        'method' => 'before',
      ),
    );
    $form['feedback_score'] = array(
      '#type' => 'fieldset',
      '#title' => t('Feedback and Scoring'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => -3,
    );
    $form['feedback_score']['choice_random'] = array(
      '#type' => 'checkbox',
      '#title' => t('Random order'),
      '#description' => t('Show choices in random order when question is being answered.'),
      '#default_value' => isset($this->node->choice_random) ? $this->node->choice_random : FALSE,
    );
    $form['feedback_score']['choice_boolean'] = array(
      '#type' => 'checkbox',
      '#title' => t('Simple scoring'),
      '#description' => t('Give max score if everything is correct. Zero points otherwise.'),
      //'#default_value' => $this->node->choice_boolean,
    );
    $form['feedback_score']['feedback'] =array(
      '#type' => 'text_format',
      '#base_type' => 'textarea',
      '#title' => t('Feedback'),
      '#description' => t('Feedback to show after the question has been answered.'),
      '#default_value' => isset($this->node->feedback) ? $this->node->feedback : '',
      '#format' => isset($this->node->feedback_format) ? $this->node->feedback_format : NULL,
      '#rows' => 3,
    );
    $form['feedback_score']['score_correct'] =array(
      '#type' => 'textfield',
      '#title' => t('Score for correct answer'),
      '#size' => 4,
      '#maxlength' => 4,
      '#description' => t('Score to be given if answered correctly.'),
      '#default_value' => isset($this->node->score_correct) ? $this->node->score_correct : 1,
      '#required' => TRUE,
    );
    $form['feedback_score']['score_incorrect'] =array(
      '#type' => 'textfield',
      '#title' => t('Score for incorrect answer'),
      '#size' => 4,
      '#maxlength' => 4,
      '#description' => t('Score to be given if answered wrongly. To be used when negative scoring is needed.'),
      '#default_value' => isset($this->node->score_incorrect) ? $this->node->score_incorrect : 0,
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Implementation of getMaximumScore.
   *
   * @see QuizQuestion#getMaximumScore()
   */
  public function getMaximumScore() {
    return $this->node->score_correct;
  }
}

/**
 * Extension of QuizQuestionResponse
 */
class SimpleMCQResponse extends QuizQuestionResponse {
  /**
   * ID of the answers.
   */
  protected $user_answer_ids;
  protected $choice_order;

  /**
   * Constructor
   */
  public function __construct($result_id, stdClass $question_node, $tries = NULL) {
    parent::__construct($result_id, $question_node, $tries);
    $this->user_answer_ids = array();
    // tries is the tries part of the post data
    if (is_array($tries)) {
      if (isset($tries['choice_order'])) {
        $this->choice_order = $tries['choice_order'];
      }
      unset($tries['choice_order']);
      if (isset($tries['answer']) && is_array($tries['answer'])) {
        foreach ($tries['answer'] as $answer_id) {
          $this->user_answer_ids[] = $answer_id;
          $this->answer = $this->user_answer_ids; //@todo: Stop using user_answer_ids and only use answer instead...
        }
      }
      elseif (isset($tries['answer'])) {
        $this->user_answer_ids[] = $tries['answer'];
      }
    }
    // We load the answer from the database
    else {
      $res = db_query('SELECT answer_id FROM {quiz_simple_mcq_user_answers} ua
              LEFT OUTER JOIN {quiz_simple_mcq_user_answer_multi} uam ON(uam.user_answer_id = ua.id)
              WHERE ua.result_id = :result_id AND ua.question_nid = :question_nid AND ua.question_vid = :question_vid', array(':result_id' => $result_id, ':question_nid' => $this->question->nid, ':question_vid' => $this->question->vid));
      while ($res_o = $res->fetch()) {
        $this->user_answer_ids[] = $res_o->answer_id;
      }
    }
  }

  /**
   * Implementation of isValid
   *
   * @see QuizQuestionResponse#isValid()
   */
  public function isValid() {
    if (isset($this->question->choice_multi) && $this->question->choice_multi) {
      return TRUE;
    }
    if (empty($this->user_answer_ids)) {
      return t('You must provide an answer');
    }
    // Perform extra check since FAPI isn't being used:
    if (count($this->user_answer_ids) > 1) {
      //return t('You are only allowed to select one answer');
    }
    return TRUE;
  }

  /**
   * Implementation of save
   *
   * @see QuizQuestionResponse#save()
   */
  public function save() {
    $user_answer_id = db_insert('quiz_simple_mcq_user_answers')
      ->fields(array(
        'result_id' => $this->rid,
        'question_vid' => $this->question->vid,
        'question_nid' => $this->question->nid,
        'choice_order' => $this->choice_order
      ))
      ->execute();

    $query = db_insert('quiz_simple_mcq_user_answer_multi')
      ->fields(array('user_answer_id', 'answer_id'));
    for ($i = 0; $i < count($this->user_answer_ids); $i++) {
      $query->values(array($user_answer_id, $this->user_answer_ids[$i]));
    }
      $query->execute();
  }

  /**
   * Implementation of delete
   *
   * @see QuizQuestionResponse#delete()
   */
  public function delete() {
    $user_answer_id = array();
    $query = db_query('SELECT id FROM {quiz_simple_mcq_user_answers} WHERE question_nid = :nid AND question_vid = :vid AND result_id = :result_id', array(':nid' => $this->question->nid, ':vid' => $this->question->vid, ':result_id' => $this->rid));
    while ($user_answer = $query->fetch()) {
      $user_answer_id[] = $user_answer->id;
    }

    if (!empty($user_answer_id)) {
      db_delete('quiz_simple_mcq_user_answer_multi')
        ->condition('user_answer_id', $user_answer_id, 'IN')
        ->execute();
    }

    db_delete('quiz_simple_mcq_user_answers')
      ->condition('result_id', $this->rid)
      ->condition('question_nid', $this->question->nid)
      ->condition('question_vid', $this->question->vid)
      ->execute();
  }

  /**
   * Implementation of score
   *
   * @return uint
   *
   * @see QuizQuestionResponse#score()
   */
  public function score() {
    $no_of_right_answers = count($this->user_answer_ids);
    $right_answers = array();

    foreach ($this->question->alternatives as $alternative) {
      if ($alternative['correct']) {
        $right_answers[] = $alternative['id'];
      }
    }
    sort($right_answers);
    sort($this->user_answer_ids);
    if ($right_answers === $this->user_answer_ids) {
      return $this->getMaxScore();
    }
    return $this->question->score_incorrect;
  }

  /**
   * If all answers in a question is wrong
   *
   * @return boolean
   *  TRUE if all answers are wrong. False otherwise.
   */
  public function isAllWrong() {
    foreach ($this->question->alternatives as $key => $alt) {
      if ($alt['correct_score'] > 0) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Implementation of getResponse
   *
   * @return answer
   *
   * @see QuizQuestionResponse#getResponse()
   */
  public function getResponse() {
    return $this->user_answer_ids;
  }

  /**
   * Implementation of getReportFormResponse
   *
   * @see getReportFormResponse($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportFormResponse($showpoints = TRUE, $showfeedback = FALSE, $allow_scoring = FALSE) {
    $i = 0;
    $this->orderAlternatives($this->question->alternatives);
    $short['correct_score'] = $this->question->score_correct;
    // Fetch all data for the report
    $data = array();
    $no_of_choices = 0;
    while (isset($this->question->alternatives[$i])) {
      $short = $this->question->alternatives[$i];
      if ($short['correct']) {
        $no_of_choices++;
      }
      if (drupal_strlen($this->checkMarkup($short['answer'], $short['answer_format'])) > 0) {
        $alternative = array();
        // Did the user choose the alternative?
        $alternative['is_chosen'] = in_array($short['id'], $this->user_answer_ids);
        $alternative['is_correct'] = $short['correct'];
        $alternative['answer'] = $this->checkMarkup($short['answer'], $short['answer_format'], FALSE);
        $data['answer'][] = $alternative;
      }
      $i++;
    }
    $data['choice_multi'] = ($no_of_choices > 1) ? TRUE : FALSE;
    $answer = $this->question->answers[0];
    $feedback = $this->question->feedback;
    $format = $this->question->feedback_format;
    $data['feedback'] = $this->checkMarkup($feedback, $format, FALSE);
    return array('#markup' => theme('simple_mcq_response', array('data' => $data)));
  }

  public function getFeedbackFormResponse($showpoints = TRUE, $showfeedback = FALSE, $allow_scoring = FALSE) {
    $i = 0;
    $this->orderAlternatives($this->question->alternatives);

    // Find the alternative with the highest score
    if ($this->question->choice_multi == 0) {
      $max_correct_score = -999;
      while (isset($this->question->alternatives[$i]) && is_array($this->question->alternatives[$i])) {
        $short = $this->question->alternatives[$i];
        if ($short['correct_score'] > $max_correct_score) {
          $max_correct_score = $short['correct_score'];
        }
        $i++;
      }
      $i = 0;
    }
    // Fetch all data for the report
    $data = array();
    while (isset($this->question->alternatives[$i])) {
      $short = $this->question->alternatives[$i];
      if (drupal_strlen($this->checkMarkup($short['answer'], $short['answer_format'])) > 0) {
        $alternative = array();

        // Did the user choose the alternative?
        $alternative['is_chosen'] = in_array($short['id'], $this->user_answer_ids);

        // Questions where multiple answers isn't allowed are scored differently...
        if ($this->question->choice_multi == 0) {

          if ($this->question->choice_boolean == 0) {
            if ($short['correct_score'] > 0) {
              $alternative['is_correct'] = $short['correct_score'] < $max_correct_score ? 1 : 2;
            }
            else {
              $alternative['is_correct'] = 0;
            }
          }

          else {
            $alternative['is_correct'] = $short['correct_score'] > 0 ? 2 : 0;
          }
        }

        // Questions where multiple answers are allowed
        else {
          $alternative['is_correct'] = $short['correct_score'] > 0 ? 2 : 0;
        }

        $alternative['answer'] = $this->checkMarkup($short['answer'], $short['answer_format'], FALSE);

        $not = $alternative['is_chosen'] ? '' : '_not';
        $alternative['feedback'] = $this->checkMarkup($short['feedback_if' . $not . '_chosen'], $short['feedback_if' . $not . '_chosen_format'], FALSE);
        $data[] = $alternative;
      }
      $i++;
    }
    if($this->question->type == 'simple_mcq' && !$this->question->choice_boolean && $this->question->answers['0']['score'] >= 0 && $this->question->choice_multi) {
      foreach ($data as &$alternative) {
        if ($alternative['is_chosen'] == 1 && $alternative['is_correct'] > 0) {
          $feedback[] = $alternative['feedback'];
        }
      }
      return $feedback;
    }
    foreach ($data as &$alternative) {
      if ($alternative['is_chosen'] == 1) {
        //The $feedback has been made a array. This is a custom implemented to show the multiple feedbacks.
        $feedback[] = $alternative['feedback'];
      }
    }
    return @$feedback;
  }

  /**
   * Order the alternatives according to the choice order stored in the database
   *
   * @param array $alternatives
   *  The alternatives to be ordered
   */
  private function orderAlternatives(array &$alternatives) {
    if (!$this->question->choice_random) {
      return;
    }
    $result = db_query('SELECT choice_order FROM {quiz_simple_mcq_user_answers}
            WHERE result_id = :result_id AND question_nid = :question_nid AND question_vid = :question_vid', array(':result_id' => $this->rid, ':question_nid' => $this->question->nid, ':question_vid' => $this->question->vid))->fetchField();
    if (!$result) {
      return;
    }
    $order = explode(',', $result);
    $newAlternatives = array();
    foreach ($order as $value) {
      foreach ($alternatives as $alternative) {
        if ($alternative['id'] == $value) {
          $newAlternatives[] = $alternative;
          break;
        }
      }
    }
    $alternatives = $newAlternatives;
  }
  /**
   * Run check_markup() on the field of the specified choice alternative
   *
   * @param $alternative
   *  String to be checked
   * @param $format
   *  The input format to be used
   * @param $check_user_access
   *  Whether or not we are to check the users access to the chosen format
   * @return HTML markup
   */
  private function checkMarkup($alternative, $format, $check_user_access = FALSE) {
    // If the string is empty we don't run it through input filters(They might add empty tags).
    if (drupal_strlen($alternative) == 0) {
      return '';
    }
    return check_markup($alternative, $format, $langcode = '' /* TODO Set this variable. */, $check_user_access);
  }
}
