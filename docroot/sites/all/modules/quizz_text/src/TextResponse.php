<?php

namespace Drupal\quizz_text;

use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\ResponseHandler;

abstract class TextResponse extends ResponseHandler {

  public function __construct($result_id, Question $question, $input = NULL) {
    parent::__construct($result_id, $question, $input);

    if (!isset($input)) {
      if (($answer = $this->loadAnswerEntity()) && ($input = $this->loadAnswerEntity()->getInput())) {
        $this->answer = $input->answer;
        $this->score = $input->score;
        $this->evaluated = $input->is_evaluated;
        $this->answer_id = $input->answer_id;
        $this->answer_feedback = $input->answer_feedback;
        $this->answer_feedback_format = $input->answer_feedback_format;
      }
    }
    else {
      $this->answer = $input;
      $this->evaluated = FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function validateReportForm(&$element, &$form_state) {
    $max = $this->question->max_score;
    // Check to make sure that entered score is not higher than max allowed score.
    if ($element['score']['#value'] > $max) {
      $msg = t('The score needs to be a number between @min and @max', array('@min' => 0, '@max' => $max));
      form_error($element['score'], $msg);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedbackValues() {
    $data = array();

    $data[] = array(
        'choice'            => '',
        'attempt'           => $this->answer,
        'correct'           => !$this->evaluated ? t('This answer has not yet been scored.') : '',
        'score'             => $this->getScore(),
        'answer_feedback'   => check_markup($this->answer_feedback, $this->answer_feedback_format),
        'question_feedback' => '',
        'solution'          => '',
    );

    return $data;
  }

}
