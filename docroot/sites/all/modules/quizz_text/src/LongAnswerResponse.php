<?php

namespace Drupal\quizz_text;

use Drupal\quizz\Entity\Answer;

/**
 * Extension of QuizQuestionResponse
 */
class LongAnswerResponse extends TextResponse {

  /**
   * {@inheritdoc}
   * @var string
   */
  protected $base_table = 'quizz_long_answer';

  /** @var int */
  protected $answer_id = 0;

  /** @var bool */
  protected $allow_feedback = TRUE;

  public function onLoad(Answer $answer) {
    // Question has been answered already. We fetch the answer data from db.
    $input = db_select($this->base_table, 'input')
      ->fields('input')
      ->condition('answer_id', $answer->id)
      ->execute()
      ->fetchObject();
    if ($input) {
      $answer->setInput($input);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    db_merge($this->base_table)
      ->key(array('answer_id' => $this->loadAnswerEntity()->id))
      ->fields(array('answer' => $this->answer))
      ->execute()
    ;
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    if ($input = $this->loadAnswerEntity()->getInput()) {
      return $input->score;
    }
    return 0;
  }

  /**
   * {@inheritdoc}
   */
  public function getReportFormSubmit() {
    return 'quizz_text_long_answer_report_submit';
  }

}
