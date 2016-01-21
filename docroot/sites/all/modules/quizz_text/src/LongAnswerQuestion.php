<?php

namespace Drupal\quizz_text;

use Drupal\quizz_question\Entity\QuestionType;
use Drupal\quizz_question\QuestionHandler;
use Drupal\quizz_text\LongAnswerResponse;

class LongAnswerQuestion extends QuestionHandler {

  protected $base_table = 'quizz_long_question';
  protected $base_answer_table = 'quizz_long_answer';

  /**
   * {@inheritdoc}
   */
  public function onSave($is_new = FALSE) {
    if (!isset($this->question->feedback)) {
      $this->question->feedback = '';
    }

    if ($is_new || $this->question->revision == 1) {
      db_insert('quizz_long_question')
        ->fields(array(
            'qid'    => $this->question->qid,
            'vid'    => $this->question->vid,
            'rubric' => $this->question->rubric,
        ))
        ->execute();
    }
    else {
      db_update('quizz_long_question')
        ->fields(array('rubric' => isset($this->question->rubric) ? $this->question->rubric : ''))
        ->condition('qid', $this->question->qid)
        ->condition('vid', $this->question->vid)
        ->execute();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function load() {
    if (isset($this->properties)) {
      return $this->properties;
    }
    $properties = parent::load();

    $res_a = db_query(
      'SELECT rubric
       FROM {quizz_long_question}
       WHERE qid = :qid AND vid = :vid', array(
        ':qid' => $this->question->qid,
        ':vid' => $this->question->vid))->fetchAssoc();

    if (is_array($res_a)) {
      $properties = array_merge($properties, $res_a);
    }

    return $this->properties = $properties;
  }

  /**
   * {@inheritdoc}
   */
  public function view() {
    $content = parent::view();

    if ($this->viewCanRevealCorrect()) {
      if (!empty($this->question->rubric)) {
        $content['answers'] = array(
            '#type'   => 'item',
            '#title'  => t('Rubric'),
            '#prefix' => '<div class="quiz-solution">',
            '#suffix' => '</div>',
            '#markup' => _filter_autop($this->question->rubric),
            '#weight' => 1,
        );
      }
    }
    else {
      $content['answers'] = array(
          '#markup' => '<div class="quiz-answer-hidden">Answer hidden</div>',
          '#weight' => 1,
      );
    }

    return $content;
  }

  /**
   * {@inheritdoc}
   */
  public function getAnsweringForm(array $form_state = NULL, $result_id) {
    $element = parent::getAnsweringForm($form_state, $result_id);

    $element += array(
        '#type'        => 'textarea',
        '#title'       => t('Answer'),
        '#description' => t('Enter your answer here. If you need more space, click on the grey bar at the bottom of this area and drag it down.'),
        '#rows'        => 15,
        '#cols'        => 60,
    );

    if (isset($result_id)) {
      $response = new LongAnswerResponse($result_id, $this->question);
      $element['#default_value'] = $response->getResponse();
    }

    return $element;
  }

  /**
   * Question response validator.
   */
  public function validateAnsweringForm(array &$form, array &$form_state = NULL) {
    if ($form_state['values']['question'][$this->question->qid]['answer'] == '') {
      form_set_error('', t('You must provide an answer.'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getCreationForm(array &$form_state = NULL) {
    $form['rubric'] = array(
        '#type'          => 'textarea',
        '#title'         => t('Rubric'),
        '#description'   => t('Specify the criteria for grading the response.'),
        '#default_value' => isset($this->question->rubric) ? $this->question->rubric : '',
        '#size'          => 60,
        '#maxlength'     => 2048,
        '#required'      => FALSE,
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function getMaximumScore() {
    return $this->question->getQuestionType()->getConfig('long_answer_default_max_score', 10);
  }

  /**
   * {@inheritdoc}
   */
  public function questionTypeConfigForm(QuestionType $question_type) {
    $form = array('#validate' => array('quizz_text_long_answer_config_validate'));
    $form['long_answer_default_max_score'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Default max score'),
        '#description'   => t('Choose the default maximum score for a long answer question.'),
        '#default_value' => $question_type->getConfig('long_answer_default_max_score', 10),
    );
    return $form;
  }

}
