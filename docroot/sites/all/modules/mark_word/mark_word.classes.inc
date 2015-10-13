<?php

/**
 * The main classes for the quiz words question type.
 * These inherit or implement code found in quiz_question.classes.inc.
 *
 * @file
 */

/**
 * Extension of QuizQuestion.
 *
 * This could have extended long answer, except that that would have entailed
 * adding long answer as a dependency.
 */
class MarkWordQuestion extends QuizQuestion {

  /**
   * Implementation of saveNodeProperties
   *
   * @see QuizQuestion#saveNodeProperties($is_new)
   */
  public function saveNodeProperties($is_new = FALSE) {
    // no special property for cloze question node
  }

  /**
   * Implementation of validateNode
   *
   * @see QuizQuestion#validateNode($form)
   */
  public function validateNode(array &$form) {
    if (empty($this->node->body[LANGUAGE_NONE]['0']['value'])) {
      form_set_error('body', t('Please check the question format.'));
    }
  }

  /**
   * Implementation of delete()
   *
   * @see QuizQuestion#delete($only_this_version)
   */
  public function delete($only_this_version = FALSE) {
    parent::delete($only_this_version);
    $delete_ans = db_delete('mark_word_user_answers');
    $delete_ans->condition('question_nid', $this->node->nid);
    if ($only_this_version) {
      $delete_ans->condition('question_vid', $this->node->vid);
    }
    $delete_ans->execute();
  }

  /**
   * Implementation of getNodeProperties()
   *
   * @see QuizQuestion#getNodeProperties()
   */
  public function getNodeProperties() {
    if (isset($this->nodeProperties)) {
      return $this->nodeProperties;
    }
    $props = parent::getNodeProperties();
    $this->nodeProperties = $props;
    return $props;
  }

  /**
   * Implementation of getNodeView()
   *
   * @see QuizQuestion#getNodeView()
   */
  public function getNodeView() {
    $content = parent::getNodeView();
    $content['#attached']['css'] = array(
      drupal_get_path('module', 'mark_word') . '/theme/mark_word.css'
    );
    if ($this->viewCanRevealCorrect()) {
      $solution = '';
      $content['answers'] = array(
        '#markup' => '<div class="quiz-solution mark_word-question">' . $solution . '</div>',
        '#weight' => 5,
      );
    }
    else {
      $content['answers'] = array(
        '#markup' => '<div class="quiz-answer-hidden">Answer hidden</div>',
        '#weight' => 2,
      );
    }
    return $content;
  }

  /**
   * Implementation of getAnsweringForm
   *
   * @see QuizQuestion#getAnsweringForm($form_state, $rid)
   */
  public function getAnsweringForm(array $form_state = NULL, $rid) {
    // get answer limit
    $answer_words = $this->node->field_mark_word_answer[LANGUAGE_NONE]['0']['value'];
    $answer_words_exp = explode(',', $answer_words);
    $answer_limit = count($answer_words_exp) - 1;
    $settings = array(
      'answer_limit' => $answer_limit,
    );
    // send the answer_limit to js
    drupal_add_js( array("mark_word" => $settings), 'setting');
    // pass the correct answer to js
    $settings = array(
      'admin_answers' => $answer_words_exp,
    );
    // send the answer_limit to js
    drupal_add_js( array("mark_word" => $settings), 'setting');
    $form = parent::getAnsweringForm($form_state, $rid);
    $form['#theme'] = 'mark_word_answering_form';
    $form['#attached']['js'] = array(
      drupal_get_path('module', 'mark_word') . '/js/mark_word_answer.js',
    );
    $form['#attached']['css'] = array(
      drupal_get_path('module', 'mark_word') . '/css/mark_word.css',
    );
    // get category type
    $category_id = $this->node->field_mark_word_type[LANGUAGE_NONE]['0']['tid'];
    $category = db_query('SELECT name FROM {taxonomy_term_data}
      WHERE tid = :tid_id', array(':tid_id' => $category_id))->fetchField();
    // create circle
    $circle_id = '<span>' . t('Answer:') . '</span>';
    for ($i=1; $i <= $answer_limit; $i++) {
      $circle_id .= '<span class="mark-word-highlight" id="circle-'.$i.'">O</span>';
    }
    $form['tries[circle]'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="mw-question-circle">',
      '#markup' => $circle_id,
      '#suffix' => '</div>',
    );
    $form['tries[category]'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="mw-question-category">',
      '#markup' => 'Choose the correct ' . $category . ' from the text below.',
      '#suffix' => '</div>',
    );
    // get question
    $body_ques = $this->node->body[LANGUAGE_NONE]['0']['value'];
    $form['tries[mark_question]'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="mw-question-select">',
      '#markup' => $body_ques,
      '#suffix' => '</div>',
    );
    $form['tries[mark_answer]'] = array(
      '#type' => 'textfield',
      '#title' => 'Answer',
      '#size' => 32,
      '#required' => FALSE,
    );
    if (isset($rid)) {
      $words_response = new MarkWordResponse($rid, $this->node);
      $response = $words_response->getResponse();
      if (is_array($response)) {
        foreach ($response as $key => $value) {
          $form["tries[$key]"]['#default_value'] = $value;
        }
      }
    }
    return $form;
  }

  /**
   * Implementation of getCreationForm
   *
   * @see QuizQuestion#getCreationForm($form_state)
   */
  public function getCreationForm(array &$form_state = NULL) {
    $form['#attached']['css'] = array(
      drupal_get_path('module', 'mark_word') . '/css/mark_word.css',
    );
    $form['#attached']['js'] = array(
      drupal_get_path('module', 'mark_word') . '/js/mark_word.js',
    );
    $form['instructions'] = array(
      '#markup' => '<div class="instruction">' .
      t('Enter the phrase from which you would like the user to be able to pick the needed words. For example take the phrase "<em>Jack and Jill went up the hill"</em>, and if you would like the user to identify "Noun" in the same. Select Category as <em>Noun</em> and click on "Jack" and "Jill" from "Select answer" below.') .
      '</div>',
      '#weight' => -10,
    );
    $form['question_select'] = array(
      '#type' => 'item',
      '#markup' => '<div id="mw-answer-select"></div>',
      '#description' => 'Click on the word to mark the right words. To update the text change Category value.',
      '#title' => t('Select answer'),
      '#weight' => -1,
    );
    return $form;
  }

  /**
   * Implementation of getMaximumScore
   *
   * @see QuizQuestion#getMaximumScore()
   */
  public function getMaximumScore() {
    //TODO: Add admin settings for this
    return 10;
  }

  /**
   * Evaluate the correctness of an answer based on the correct answer and evaluation method.
   */
  public function evaluateAnswer($user_answer) {
    $admin_answer = $this->node->field_mark_word_answer[LANGUAGE_NONE][0]['value'];
    $correct_answer = explode(',', $admin_answer);
    $user_answer = explode(',', $user_answer['mark_answer']);
    $total_answer = count($correct_answer) - 1;
    $correct_answer_count = 0;
    if ($total_answer == 0) {
      return $this->getMaximumScore();
    }
    foreach ($correct_answer as $key => $value) {
       if (!empty($correct_answer[$key]) && in_array($correct_answer[$key], $user_answer)) {
        $correct_answer_count++;
      }
    }
    $score = $correct_answer_count / $total_answer * $this->getMaximumScore();
    return round($score);
  }
}

/**
 * Extension of QuizQuestionResponse
 */
class MarkWordResponse extends QuizQuestionResponse {
  /**
   * ID of the answer.
   */
  protected $answer_id = 0;

  /**
   * Constructor
   */
  public function __construct($result_id, stdClass $question_node, $answer = NULL) {
    parent::__construct($result_id, $question_node, $answer);
    if (!isset($answer)) {
      $r = db_query("SELECT answer_id, answer, score, question_vid, question_nid, result_id FROM {mark_word_user_answers} WHERE question_nid = :question_nid AND question_vid = :question_vid AND result_id = :result_id", array(':question_nid' => $question_node->nid, ':question_vid'=>$question_node->vid, ':result_id' => $result_id))->fetch();
      if (!empty($r)) {
  $this->answer = unserialize($r->answer);
  $this->score = $r->score;
  $this->answer_id = $r->answer_id;
      }
    }
    else {
      $this->answer = $answer;
    }
  }

  /**
   * Implementation of isValid
   *
   * @see QuizQuestionResponse#isValid()
   */
  public function isValid() {
    if (trim($this->answer['mark_answer']) == '') {
      return t('You must Choose a Answer');
    }
    return TRUE;
  }

  /**
   * Implementation of save
   *
   * @see QuizQuestionResponse#save()
   */
  public function save() {
    $this->answer_id = db_insert('mark_word_user_answers')
      ->fields(array(
        'answer' => serialize($this->answer),
        'question_nid' => $this->question->nid,
        'question_vid' => $this->question->vid,
        'result_id' => $this->rid,
        'score' => $this->getScore(FALSE),
      ))
      ->execute();
  }

  /**
   * Implementation of delete()
   *
   * @see QuizQuestionResponse#delete()
   */
  public function delete() {
    db_delete('mark_word_user_answers')
      ->condition('question_nid', $this->question->nid)
      ->condition('question_vid', $this->question->vid)
      ->condition('result_id', $this->rid)
      ->execute();
  }

  /**
   * Implementation of score()
   *
   * @see QuizQuestionResponse#score()
   */
  public function score() {
    $shortAnswer = new MarkWordQuestion($this->question);
    $score = $shortAnswer->evaluateAnswer($this->answer);
    return $score;
  }

  /**
   * Implementation of getResponse()
   *
   * @see QuizQuestionResponse#getResponse()
   */
  public function getResponse() {
    return $this->answer;
  }

  /**
   * Implementation of getReportForm()
   *
   * @see QuizQuestionResponse#getReportForm($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportForm($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    $form = parent::getReportForm($showpoints, $showfeedback, $allow_scoring);
    $question = strip_tags($form['question']['#markup']);
    $question_form['open_wrapper'] = array(
      '#markup' => '<div class="mark_word-question">',
    );
    $question_form['close_wrapper'] = array(
      '#markup' => '</div>',
    );
    $form['question']['#markup'] = drupal_render($question_form);
    return $form;
  }

  /**
   * Implementation of getReportFormResponse()
   *
   * @see QuizQuestionResponse#getReportFormResponse($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportFormResponse($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    $form = array();
    $form['#theme'] = 'mark_word_response_form';
    $form['#attached']['css'] = array(
      drupal_get_path('module', 'mark_word') . '/theme/mark_word.css'
    );
    if (($this->question) && !empty($this->question->answers)) {
      $answer = (object) current($this->question->answers);
    }
    else {
      return $form;
    }
    $this->question = node_load($this->question->nid);
    $question = $this->question->body[LANGUAGE_NONE][0]['value'];
    $admin_answer = $this->question->field_mark_word_answer[LANGUAGE_NONE][0]['value'];
    $correct_answer = _mark_word_get_correct_answer($question, $admin_answer);
    $user_answer = $this->answer['mark_answer'];
    $user_answer = _mark_word_get_user_answer($question, $user_answer, $admin_answer);
    $form['answer'] = array(
      '#markup' => theme('mark_word_user_answer', array('answer' => $user_answer, 'correct' => $correct_answer)),
    );
    return $form;
  }

  /**
   * Implementation of getReportFormScore()
   *
   * @see QuizQuestionResponse#getReportFormScore($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportFormScore($showfeedback = TRUE, $showpoints = TRUE, $allow_scoring = FALSE) {
    return array(
     '#markup' => $this->getScore(),
    );
  }
}
