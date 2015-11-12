<?php

namespace Drupal\quizz_text;

use Drupal\quizz_question\Entity\QuestionType;
use Drupal\quizz_question\QuestionHandler;
use Drupal\quizz_text\ShortAnswerResponse;

/**
 * The main classes for the short answer question type.
 *
 * If you are developing your own question type, the easiest place to start is with
 * short_answer and long_answer are good for understanding question types that
 * involve textual answers.
 */

/**
 * Extension of QuizQuestion.
 *
 * This could have extended long answer, except that that would have entailed
 * adding long answer as a dependency.
 */
class ShortAnswerQuestion extends QuestionHandler {

  // Constants for answer matching options
  const ANSWER_MATCH = 0;
  const ANSWER_INSENSITIVE_MATCH = 1;
  const ANSWER_REGEX = 2;
  const ANSWER_MANUAL = 3;

  /**
   * {@inheritdoc}
   */
  public function onSave($is_new = FALSE) {
    if ($is_new || $this->question->revision == 1) {
      db_insert('quizz_short_question')
        ->fields(array(
            'qid'                       => $this->question->qid,
            'vid'                       => $this->question->vid,
            'correct_answer'            => $this->question->correct_answer,
            'correct_answer_evaluation' => $this->question->correct_answer_evaluation,
        ))
        ->execute();
    }
    else {
      db_update('quizz_short_question')
        ->fields(array(
            'correct_answer'            => $this->question->correct_answer,
            'correct_answer_evaluation' => $this->question->correct_answer_evaluation,
        ))
        ->condition('qid', $this->question->qid)
        ->condition('vid', $this->question->vid)
        ->execute();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function validate(array &$form) {
    if ($this->question->correct_answer_evaluation != self::ANSWER_MANUAL && empty($this->question->correct_answer)) {
      form_set_error('correct_answer', t('An answer must be specified for any evaluation type other than manual scoring.'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function delete($only_this_version = FALSE) {
    parent::delete($only_this_version);
    $delete_ans = db_delete('quizz_short_answer');
    $delete_ans->condition('question_qid', $this->question->qid);

    $delete_node = db_delete('quizz_short_question');
    $delete_node->condition('qid', $this->question->qid);

    if ($only_this_version) {
      $delete_ans->condition('question_vid', $this->question->vid);
      $delete_node->condition('vid', $this->question->vid);
    }

    $delete_ans->execute();
    $delete_node->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function load() {
    if (isset($this->properties)) {
      return $this->properties;
    }
    $props = parent::load();
    $res_a = db_query('SELECT correct_answer, correct_answer_evaluation FROM {quizz_short_question}
      WHERE qid = :qid AND vid = :vid', array(':qid' => $this->question->qid, ':vid' => $this->question->vid))->fetchAssoc();
    $this->properties = (is_array($res_a)) ? array_merge($props, $res_a) : $props;
    return $this->properties;
  }

  /**
   * {@inheritdoc}
   */
  public function view() {
    $content = parent::view();
    if ($this->viewCanRevealCorrect()) {
      $content['answers'] = array(
          '#markup' => '<div class="quiz-solution">' . check_plain($this->question->correct_answer) . '</div>',
          '#weight' => 2,
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
   * {@inheritdoc}
   */
  public function getAnsweringForm(array $form_state = NULL, $result_id) {
    $form = parent::getAnsweringForm($form_state, $result_id);

    $form = array(
        '#type'          => 'textfield',
        '#title'         => t('Answer'),
        '#description'   => t('Enter your answer here'),
        '#default_value' => '',
        '#size'          => 60,
        '#maxlength'     => 256,
        '#required'      => FALSE,
        '#attributes'    => array('autocomplete' => 'off'),
    );

    if (isset($result_id)) {
      $response = new ShortAnswerResponse($result_id, $this->question);
      $form['#default_value'] = $response->getResponse();
    }

    return $form;
  }

  /**
   * Question response validator.
   */
  public function validateAnsweringForm(array &$form, array &$form_state = NULL) {
    if ('' === $form_state['values']['question'][$this->question->qid]['answer']) {
      form_set_error('', t('You must provide an answer.'));
    }
  }

  /**
   * Implementation of getCreationForm
   *
   * @see QuizQuestion#getCreationForm($form_state)
   */
  public function getCreationForm(array &$form_state = NULL) {
    $form['answer'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Answer'),
        '#description' => t('Provide the answer and the method by which the answer will be evaluated.'),
        '#collapsible' => TRUE,
        '#collapsed'   => FALSE,
        '#weight'      => -4,
    );

    $options = array(
        self::ANSWER_MATCH             => t('Automatic and case sensitive'),
        self::ANSWER_INSENSITIVE_MATCH => t('Automatic. Not case sensitive'),
    );
    $access_regex = user_access('use regex for short answer');
    if ($access_regex) {
      $options[self::ANSWER_REGEX] = t('Match against a regular expression (answer must match the supplied regular expression)');
    }
    $options[self::ANSWER_MANUAL] = t('Manual');

    $form['answer']['correct_answer_evaluation'] = array(
        '#type'          => 'radios',
        '#title'         => t('Pick an evaluation method'),
        '#description'   => t('Choose how the answer shall be evaluated.'),
        '#options'       => $options,
        '#default_value' => isset($this->question->correct_answer_evaluation) ? $this->question->correct_answer_evaluation : self::ANSWER_INSENSITIVE_MATCH,
        '#required'      => FALSE,
    );

    if ($access_regex) {
      $form['answer']['regex_box'] = array(
          '#type'        => 'fieldset',
          '#title'       => t('About regular expressions'),
          '#collapsible' => TRUE,
          '#collapsed'   => TRUE,
      );

      $form['answer']['regex_box']['regex_help'] = array(
          '#markup' => '<p>' .
          t('Regular expressions are an advanced syntax for pattern matching. They allow you to create a concise set of rules that must be met before a value can be considered a match.') .
          '</p><p>' .
          t('For more on regular expression syntax, visit !url.', array('!url' => l('the PHP regular expressions documentation', 'http://www.php.net/manual/en/book.pcre.php'))) .
          '</p>',
      );
    }

    $form['answer']['correct_answer'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Correct answer'),
        '#description'   => t('Specify the answer. If this question is manually scored, no answer needs to be supplied.'),
        '#default_value' => isset($this->question->correct_answer) ? $this->question->correct_answer : '',
        '#size'          => 60,
        '#maxlength'     => 256,
        '#required'      => FALSE,
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function getMaximumScore() {
    return $this->question->getQuestionType()->getConfig('short_answer_default_max_score', 5);
  }

  /**
   * Evaluate the correctness of an answer based on the correct answer and evaluation method.
   */
  public function evaluateAnswer($input) {
    $score = 0;

    // Ignore white spaces for correct answer
    $this->question->correct_answer = trim($this->question->correct_answer);

    switch ($this->question->correct_answer_evaluation) {
      case self::ANSWER_MATCH:
        if ($input == $this->question->correct_answer) {
          $score = $this->question->max_score;
        }
        break;
      case self::ANSWER_INSENSITIVE_MATCH:
        if (drupal_strtolower($input) == drupal_strtolower($this->question->correct_answer)) {
          $score = $this->question->max_score;
        }
        break;
      case self::ANSWER_REGEX:
        if (preg_match($this->question->correct_answer, $input) > 0) {
          $score = $this->question->max_score;
        }
        break;
    }
    return $score;
  }

  /**
   * {@inheritdoc}
   */
  public function questionTypeConfigForm(QuestionType $question_type) {
    $form['short_answer_default_max_score'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Default max score'),
        '#description'   => t('Choose the default maximum score for a short answer question.'),
        '#default_value' => $question_type->getConfig('short_answer_default_max_score', 5),
    );
    $form['#validate'][] = 'quizz_text_short_answer_config_validate';
    return $form;
  }

}
