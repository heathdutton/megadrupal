<?php

namespace Drupal\quizz_matching;

use Drupal\quizz\Entity\Answer;
use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\ResponseHandler;

/**
 * Extension of QuizQuestionResponse
 */
class MatchingResponse extends ResponseHandler {

  public function __construct($result_id, Question $question, $input = NULL) {
    parent::__construct($result_id, $question, $input);

    if (NULL === $input) {
      if (($answer = $this->loadAnswerEntity()) && ($input = $answer->getInput())) {
        $this->answer = $input;
      }
    }

    $this->is_correct = $this->isCorrect();
  }

  public function onLoad(Answer $answer) {
    $select = db_select('quizz_matching_answer', 'input');
    $select->innerJoin('quiz_matching_question', 'question_property', 'input.match_id = question_property.match_id');
    $input = $select
      ->fields('input', array('match_id', 'answer', 'score'))
      ->condition('answer_id', $answer->id)
      ->execute()
      ->fetchAllKeyed()
    ;

    if ($input) {
      $answer->setInput($input);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    if (!isset($this->answer) || !is_array($this->answer)) {
      return;
    }

    $insert = db_insert('quizz_matching_answer')->fields(array('answer_id', 'match_id', 'answer', 'score'));
    $answer_id = $this->loadAnswerEntity()->id;
    foreach ($this->answer as $key => $value) {
      $insert->values(array(
          'answer_id' => $answer_id,
          'match_id'  => $key,
          'answer'    => (int) $value,
          'score'     => ($key == $value) ? 1 : 0,
      ));
    }
    $insert->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function delete() {
    db_query(
      "DELETE ap FROM {quizz_matching_answer} ap"
      . " INNER JOIN {quiz_answer_entity} answer ON ap.answer_id = answer.id"
      . " WHERE answer.question_vid = :vid", array(
        ':vid' => $this->question->vid,
      )
    );
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    $wrong_answer = 0;
    $correct_answer = 0;
    $user_answers = isset($this->answer['answer']) ? $this->answer['answer'] : $this->answer;
    $MatchingQuestion = new MatchingQuestion($this->question);
    $correct_answers = $MatchingQuestion->getCorrectAnswer();
    foreach ((array) $user_answers as $key => $value) {
      if ($value != 0 && $correct_answers[$key]['answer'] == $correct_answers[$value]['answer']) {
        $correct_answer++;
      }
      elseif ($value == 0 || $value == 'def') {

      }
      else {
        $wrong_answer++;
      }
    }

    $score = $correct_answer;
    if ($this->question->choice_penalty) {
      $score -= $wrong_answer;
    }

    return $score < 0 ? 0 : $score;
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedbackValues() {
    $data = array();
    $answers = $this->question->answers[0]['answer'];
    $solution = $this->question_handler->getSubquestions();
    foreach ($this->question->match as $match) {
      $data[] = array(
          'choice'            => $match['question'],
          'attempt'           => !empty($answers[$match['match_id']]) ? $solution[1][$answers[$match['match_id']]] : '',
          'correct'           => $answers[$match['match_id']] == $match['match_id'] ? theme('quizz_answer_result', array('type' => 'correct')) : theme('quizz_answer_result', array('type' => 'incorrect')),
          'score'             => $answers[$match['match_id']] == $match['match_id'] ? 1 : 0,
          'answer_feedback'   => $match['feedback'],
          'question_feedback' => 'Question feedback',
          'solution'          => $solution[1][$match['match_id']],
          'quiz_feedback'     => t('@quiz feedback', array('@quiz' => QUIZZ_NAME)),
      );
    }
    return $data;
  }

}
