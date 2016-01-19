<?php

namespace Drupal\quizz_scale;

use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\ResponseHandler;
use Drupal\quizz\Entity\Answer;

/**
 * Extension of QuizQuestionResponse
 */
class ScaleResponse extends ResponseHandler {

  /**
   * {@inheritdoc}
   * @var string
   */
  protected $base_table = 'quizz_scale_answer';

  public function __construct($result_id, Question $question, $input = NULL) {
    parent::__construct($result_id, $question, $input);

    if (NULL === $input) {
      if (($answer = $this->loadAnswerEntity()) && ($input = $answer->getInput())) {
        $this->setAnswerInput($answer->getInput());
      }
    }
    else {
      $sql = 'SELECT answer FROM {quiz_scale_collection_item} WHERE id = :id';
      if ($user_answer = db_query($sql, array(':id' => (int) $input))->fetchField()) {
        $this->setAnswerInput($user_answer);
      }
    }
  }

  public function onLoad(Answer $answer) {
    $sql = 'SELECT collection_item_id FROM {' . $this->base_table . '} ap';
    $sql .= ' INNER JOIN {quiz_scale_collection_item} item ON ap.collection_item_id = item.id';
    $sql .= ' WHERE answer_id = :id';
    if ($input = db_query($sql, array(':id' => $answer->id))->fetchField()) {
      $answer->setInput($input);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    db_merge($this->base_table)
      ->key(array('answer_id' => $this->loadAnswerEntity()->id))
      ->fields(array('collection_item_id' => $this->answer))
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    return $this->isValid() ? 1 : 0;
  }

  /**
   * {@inheritdoc}
   */
  public function getResponse() {
    return $this->answer;
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedbackValues() {
    return array(array('choice' => $this->answer));
  }

}
