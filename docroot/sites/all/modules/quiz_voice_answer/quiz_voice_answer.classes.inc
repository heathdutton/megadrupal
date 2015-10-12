<?php

/**
 * @file
 * This module uses the question interface.
 *
 * Quiz voice answer module provides a new content type to use with quiz module
 * as a question type. It allows users to record and save voice message as a
 * response to quiz question.
 */

/**
 * Extension of QuizQuestion.
 */
class QuizVoiceAnswerQuestion extends QuizQuestion {

  /**
   * Implementation of validateNode.
   *
   * This abstract function.
   *
   * @see QuizQuestion::validateNode($form)
   */
  public function validateNode(array &$form) {

  }

  /**
   * Implementation of saveNodeProperties.
   *
   * This abstract function.
   *
   * @see QuizQuestion::saveNodeProperties($is_new)
   */
  public function saveNodeProperties($is_new = FALSE) {

  }

  /**
   * Implementation of getAnsweringForm.
   */
  public function getAnsweringForm(array $form_state, $rid) {
    $form = parent::getAnsweringForm($form_state, $rid);
    global $user;
    if (libraries_get_path('Recordmp3js-Drupal')) {
      libraries_load('Recordmp3js-Drupal');
    }
    else {
      $download_url = 'https://github.com/Ruslan03492/Recordmp3js-Drupal';
      $args = array(
        '!url' => l($download_url, $download_url),
      );
      $msg = t('Please download the Recordmp3js-Drupal library from !url.', $args);
      drupal_set_message($msg, 'warning');
      return array();
    }
    $form['actions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Recording answer'),
    );
    $settings = array(
      'nid' => $this->node->nid,
      'uid' => $user->uid,
    );
    $form['actions']['#attached']['js'][] = array(
      'data' => array('quiz_voice_answer' => $settings),
      'type' => 'setting',
    );
    $form['actions']['rec'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'input',
      '#attributes' => array(
        'id' => 'rec',
        'type' => 'button',
        'value' => t('Rec'),
      ),
    );
    $form['actions']['stop'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'input',
      '#attributes' => array(
        'id' => 'stop',
        'type' => 'button',
        'disabled' => 'disabled',
        'value' => t('Stop'),
      ),
    );
    $form['actions']['log'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'pre',
      '#attributes' => array(
        'id' => 'log',
      ),
      '#value' => '',
    );
    $form['actions']['recordingslist'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'div',
      '#attributes' => array(
        'id' => 'recordingslist',
      ),
      '#value' => '',
    );
    $form['tries'] = array(
      '#type' => 'hidden',
      '#default_value' => '',
      '#attributes' => array('id' => 'tries'),
    );
    if (isset($rid)) {
      $record_response = new QuizVoiceAnswerResponse($rid, $this->node);
      $response = $record_response->getResponse();
      if (is_numeric($response)) {
        $file = file_load($response);
        $out = file_view($file);
        $form['actions']['recordingslist']['#value'] = drupal_render($out);
        $form['tries']['#default_value'] = $response;
      }
    }
    return $form;
  }

  /**
   * Implementation of getBodyFieldTitle.
   *
   * @see QuizQuestion::getBodyFieldTitle()
   */
  public function getBodyFieldTitle() {
    return t('Question');
  }

  /**
   * Implementation of getCreationForm.
   *
   * @see QuizQuestion::getCreationForm($form_state)
   */
  public function getCreationForm(array &$form_state = NULL) {
    return array();
  }

  /**
   * Implementation of getMaximumScore.
   *
   * @see QuizQuestion::getMaximumScore()
   */
  public function getMaximumScore() {
    return 10;
  }

}

/**
 * Extension of QuizQuestionResponse.
 */
class QuizVoiceAnswerResponse extends QuizQuestionResponse {
  /**
   * ID of the answer.
   */
  protected $answerId = 0;

  /**
   * Constructor.
   */
  public function __construct($result_id, stdClass $question_node, $answer = NULL) {
    parent::__construct($result_id, $question_node, $answer);
    if (!isset($answer) || is_numeric($answer)) {
      $r = db_select('quiz_voice_answer_user_answers', 'qva')
        ->fields('qva', array(
          'answer_id',
          'answer',
          'score',
          'is_evaluated',
          'question_vid',
          'answer_feedback',
        ))
        ->condition('qva.question_nid', $question_node->nid)
        ->condition('qva.question_vid', $question_node->vid)
        ->condition('qva.result_id', $result_id)
        ->execute()
        ->fetch();
      if (!empty($r)) {
        $this->answer = $r->answer;
        $this->score = $r->score;
        $this->evaluated = $r->is_evaluated;
        $this->answerId = $r->answer_id;
        $this->answer_feedback = $r->answer_feedback;
      }
    }
    else {
      if ($file = $this->upload($answer, $this->question->nid)) {
        $this->answer = $file->fid;
        $this->evaluated = FALSE;
      }
    }
  }

  /**
   * Upload file or get isset file.
   *
   * @param string $data
   *   Data audio or fid file.
   *
   * @return array
   *   An array of variables file.
   */
  public function upload($data = NULL, $nid = 0) {
    if ($data) {
      global $user;
      list(, $data) = explode(';', $data);
      list(, $data) = explode(',', $data);
      $data = base64_decode($data);
      $path_upload = 'private://quiz_voice_answer/' . $user->uid;
      if (!file_exists($path_upload)) {
        drupal_mkdir($path_upload, NULL, TRUE);
      }
      if (file_exists($path_upload)) {
        $file = file_save_data(
          $data, $path_upload . '/audio_recording_' . $nid . '_'
          . $user->uid . '.mp3'
        );
        if ($file) {
          return $file;
        }
      }
    }
    return NULL;
  }

  /**
   * Implementation of save.
   *
   * @see QuizQuestionResponse::save()
   */
  public function save() {
    $this->answerId = db_insert('quiz_voice_answer_user_answers')
      ->fields(
        array(
          'answer' => $this->answer,
          'question_nid' => $this->question->nid,
          'question_vid' => $this->question->vid,
          'result_id' => $this->rid,
          'score' => $this->getScore(),
        )
      )
      ->execute();
  }

  /**
   * Implementation of delete.
   *
   * @see QuizQuestionResponse::delete()
   */
  public function delete() {
    db_delete('quiz_voice_answer_user_answers')
      ->condition('question_nid', $this->question->nid)
      ->condition('question_vid', $this->question->vid)
      ->condition('result_id', $this->rid)
      ->execute();
  }

  /**
   * Implementation of score.
   *
   * @see QuizQuestionResponse::score()
   */
  public function score() {
    return (int) db_select('quiz_voice_answer_user_answers', 'qva')
      ->fields('qva', array('score'))
      ->condition('qva.question_nid', $this->question->nid)
      ->condition('qva.question_vid', $this->question->vid)
      ->condition('qva.result_id', $this->rid)
      ->execute()
      ->fetchField();
  }

  /**
   * Implementation of isCorrect.
   *
   * @see QuizQuestionResponse::isCorrect()
   */
  public function isCorrect() {
    return TRUE;
  }

  /**
   * Implementation of isValid.
   *
   * @see QuizQuestionResponse::isValid()
   */
  public function isValid() {
    if (empty($this->answer)) {
      return t('You must answer question.');
    }
    return TRUE;
  }

  /**
   * Implementation of getResponse.
   *
   * @see QuizQuestionResponse::getResponse()
   */
  public function getResponse() {
    return $this->answer;
  }

  /**
   * Get the response part of the report form.
   *
   * @param bool $showpoints
   *   An boolean variable for show point.
   * @param bool $showfeedback
   *   An boolean variable for show feedback.
   * @param bool $allow_scoring
   *   An boolean variable for scoring.
   *
   * @return array
   *   FAPI form array holding the response part.
   */
  public function getReportFormResponse($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    $out = '';
    if ($file = file_load($this->answer)) {
      $out = file_view($file);
    }
    $form['file_audio'] = array($out);

    if (!$allow_scoring && !empty($this->answer_feedback)) {
      $form['answer_feedback'] = array(
        '#title' => t('Feedback'),
        '#type' => 'item',
        '#theme' => 'html_tag',
        '#tag' => 'span',
        '#attributes' => array(
          'class' => array('quiz_answer_feedback'),
        ),
        '#value' => $this->answer_feedback,
      );
    }
    return $form;
  }

  /**
   * Get the score part of the report form.
   *
   * @param bool $showpoints
   *   An boolean variable for show point.
   * @param bool $showfeedback
   *   An boolean variable for show feedback.
   * @param bool $allow_scoring
   *   An boolean variable for scoring.
   *
   * @return array
   *   FAPI form array holding the response part.
   */
  public function getReportFormScore($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    // The score will be shown as a questionmark if the question haven't
    // been scored already.
    $score = ($this->isEvaluated()) ? $this->getScore() : '?';
    // We show a textfield if the quiz shall be scored. Markup otherwise.
    if (quiz_access_to_score() && $allow_scoring) {
      return array(
        '#type' => 'textfield',
        '#default_value' => $score,
        '#size' => 3,
        '#maxlength' => 3,
        '#attributes' => array('class' => array('quiz-report-score')),
      );
    }
    else {
      return array('#markup' => $score);
    }
  }

  /**
   * Implementation of getReportFormAnswerFeedback.
   */
  public function getReportFormAnswerFeedback($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    if (quiz_access_to_score() && $allow_scoring) {
      return array(
        '#title' => t('Enter feedback'),
        '#type' => 'textarea',
        '#default_value' => isset($this->answer_feedback) ? $this->answer_feedback : '',
        '#attributes' => array('class' => array('quiz-report-score')),
      );
    }
    return FALSE;
  }

  /**
   * Implementation of getReportFormSubmit.
   */
  public function getReportFormSubmit($showfeedback = TRUE, $showpoints = TRUE, $allow_scoring = FALSE) {
    return $allow_scoring ? 'quiz_voice_answer_report_submit' : FALSE;
  }

  /**
   * Implementation of getReportFormValidate.
   */
  public function getReportFormValidate($showfeedback = TRUE, $showpoints = TRUE, $allow_scoring = FALSE) {
    return $allow_scoring ? 'quiz_voice_answer_report_validate' : FALSE;
  }

}
