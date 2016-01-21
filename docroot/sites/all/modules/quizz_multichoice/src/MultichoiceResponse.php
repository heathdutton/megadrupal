<?php

namespace Drupal\quizz_multichoice;

use Drupal\quizz\Entity\Answer;
use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\ResponseHandler;

class MultichoiceResponse extends ResponseHandler {

  protected $choice_order = array();
  protected $answer = array();

  public function __construct($result_id, Question $question, $input = NULL) {
    parent::__construct($result_id, $question, $input['user_answer']);

    if (NULL === $input) {
      if (($answer = $this->loadAnswerEntity()) && ($input = $answer->getInput())) {
        $this->setAnswerInput((array) $input);
      }
    }
    else {
      $this->setAnswerInput($input);
    }
  }

  public function setAnswerInput($input) {
    if (isset($input['user_answer'])) {
      $this->answer = is_array($input['user_answer']) ? $input['user_answer'] : array($input['user_answer']);
      $this->choice_order = isset($input['choice_order']) ? $input['choice_order'] : array();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function onLoad(Answer $answer) {
    $sql = 'SELECT choice_order, user_answer FROM {quizz_multichoice_answer} WHERE answer_id = :answer_id';
    if ($input = db_query($sql, array(':answer_id' => $answer->id))->fetchObject()) {
      is_string($input->choice_order) && ($input->choice_order = unserialize($input->choice_order));
      is_string($input->user_answer) && ($input->user_answer = unserialize($input->user_answer));
      $answer->setInput($input);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    db_merge('quizz_multichoice_answer')
      ->key(array('answer_id' => $this->loadAnswerEntity()->id))
      ->fields(array(
          'choice_order' => serialize($this->choice_order),
          'user_answer'  => serialize($this->answer),
      ))
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function delete() {
    db_delete('quizz_multichoice_answer')
      ->condition('answer_id', $this->loadAnswerEntity()->id)
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    $all_wrong = TRUE;

    foreach ($this->question->alternatives as $alt) {
      if (($alt['score_if_chosen'] > 0) || ($alt['score_if_not_chosen'] > 0)) {
        $all_wrong = FALSE;
        break;
      }
    }

    if ($this->question->choice_boolean || $all_wrong) {
      foreach ($this->question->alternatives as $alt) {
        if (in_array($alt['id'], $this->answer)) {
          if ($alt['score_if_chosen'] <= $alt['score_if_not_chosen']) {
            return 0;
          }
        }
        elseif ($alt['score_if_chosen'] > $alt['score_if_not_chosen']) {
          return 0;
        }
      }
      return $this->getQuestionMaxScore();
    }

    $score = 0;
    foreach ($this->question->alternatives as $alt) {
      $score += in_array($alt['id'], $this->answer) ? $alt['score_if_chosen'] : $alt['score_if_not_chosen'];
    }
    return $score;
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedbackValues() {
    $this->orderAlternatives($this->question->alternatives);
    $simple_scoring = $this->question->choice_boolean;

    $data = array();
    foreach ($this->question->alternatives as $alternative) {
      $chosen = in_array($alternative['id'], $this->answer);
      $not = $chosen ? '' : 'not_';

      $data[] = array(
          'choice'            => check_markup($alternative['answer']['value'], $alternative['answer']['format']),
          'attempt'           => $chosen ? quizz_icon('selected') : '',
          'correct'           => $chosen ? $alternative['score_if_chosen'] > 0 ? quizz_icon('correct') : quizz_icon('incorrect') : '',
          'score'             => $alternative["score_if_{$not}chosen"],
          'answer_feedback'   => check_markup($alternative["feedback_if_{$not}chosen"]['value'], $alternative["feedback_if_{$not}chosen"]['format'], FALSE),
          'question_feedback' => 'Question feedback',
          'solution'          => $alternative['score_if_chosen'] > 0 ? quizz_icon('should') : ($simple_scoring ? quizz_icon('should-not') : ''),
          'quiz_feedback'     => t('@quiz feedback', array('@quiz' => QUIZZ_NAME)),
      );
    }

    return $data;
  }

  /**
   * Order the alternatives according to the choice order stored in db.
   */
  protected function orderAlternatives(array &$alternatives) {
    if (!$this->question->choice_random || !$input = $this->loadAnswerEntity()->getInput()) {
      return;
    }

    foreach ($alternatives as $alternative) {
      if (in_array($alternative['id'], $input->choice_order)) {
        $alts[] = $alternative;
      }
    }

    $alternatives = !empty($alts) ? $alts : array();
  }

}
