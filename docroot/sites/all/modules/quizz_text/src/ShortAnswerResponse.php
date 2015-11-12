<?php

namespace Drupal\quizz_text;

use Drupal\quizz\Entity\Answer;
use Drupal\quizz_question\Entity\Question;

class ShortAnswerResponse extends TextResponse {

  protected $answer_id = 0;

  /**
   * {@inheritdoc}
   * @var string
   */
  protected $base_table = 'quizz_short_answer';

  /** @var bool */
  protected $allow_feedback = TRUE;

  public function __construct($result_id, Question $question, $input = NULL) {
    if ((NULL !== $input) && !is_array($input)) {
      $this->evaluated = $question->correct_answer_evaluation != ShortAnswerQuestion::ANSWER_MANUAL;
    }
    parent::__construct($result_id, $question, $input);
  }

  public function onLoad(Answer $answer) {
    $sql = 'SELECT input.* FROM {quizz_short_answer} input WHERE answer_id = :id';
    if ($input = db_query($sql, array(':id' => $answer->id))->fetchObject()) {
      $answer->setInput($input);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    db_merge($this->base_table)
      ->key(array('answer_id' => $this->loadAnswerEntity()->id))
      ->fields(array(
          'answer'            => $this->answer,
          'score'             => $this->getScore(FALSE),
          'is_evaluated'      => $this->is_evaluated = (int) ($this->question->correct_answer_evaluation != ShortAnswerQuestion::ANSWER_MANUAL)
      ))
      ->execute()
    ;
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    // Manual scoring means we go with what is in the DB.
    if (ShortAnswerQuestion::ANSWER_MANUAL == $this->question->correct_answer_evaluation) {
      if ($input = $this->loadAnswerEntity()->getInput()) {
        return $input->score;
      }
      return 0;
    }

    $handler = new ShortAnswerQuestion($this->question);
    return $handler->evaluateAnswer($this->getResponse());
  }

  /**
   * {@inheritdoc}
   */
  public function getReportFormSubmit() {
    return 'quizz_text_short_answer_report_submit';
  }

}
