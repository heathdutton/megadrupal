<?php

namespace Drupal\quizz_pool\Form;

use Drupal\quizz_question\Entity\Question;
use Drupal\quizz\Entity\QuizEntity;

class AnswerForm {

  private $quiz;
  private $question;
  private $session;
  private $session_key;
  private $result_id;

  public function __construct(QuizEntity $quiz, Question $question, &$session) {
    $this->quiz = $quiz;
    $this->question = $question;
    $this->session = &$session;
    $this->session_key = "pool_{$this->question->qid}";
    $this->result_id = $this->session['result_id'];

    if (!isset($this->session[$this->session_key]['delta'])) {
      $this->session[$this->session_key]['delta'] = 0;
      $this->session[$this->session_key]['passed'] = FALSE;
    }
  }

}
