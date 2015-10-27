<?php

namespace Drupal\quizz_pool;

use Drupal\quizz\Entity\Answer;
use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\ResponseHandler;

/**
 * Extension of QuizQuestionResponse
 */
class PoolResponseHandler extends ResponseHandler {

  /**
   * {@inheritdoc}
   * @var string
   */
  protected $base_table = 'quizz_pool_answer';
  protected $user_answer_ids;
  protected $choice_order;
  protected $need_evaluated;
  private $session;

  public function __construct($result_id, Question $question, $input = NULL) {
    parent::__construct($result_id, $question, $input);

    if (NULL === $input) {
      if (($answer = $this->loadAnswerEntity()) && ($input = $answer->getInput())) {
        $this->answer = $input->answer;
        $this->score = $input->score;
      }
    }
    else {
      $this->setAnswerInput($input);
    }

    // Only work with session when user is taking quiz.
    $this->setupSession();
  }

  private function setupSession() {
    $valid_1 = isset($_SESSION['quiz']) && isset($_SESSION['quiz'][$this->result->quiz_qid]);
    $valid_2 = isset($_SESSION['quiz']['temp']) && ($this->result_id != $_SESSION['quiz']['temp']['result_id']);
    if (!$valid_1 && !$valid_2) {
      return;
    }

    $quiz_id = $this->result->getQuiz()->qid;
    $key = "pool_{$this->question->qid}";

    if (!isset($_SESSION['quiz'][$quiz_id][$key])) {
      $_SESSION['quiz'][$quiz_id][$key] = array('passed' => FALSE, 'delta' => 0);
    }

    if (isset($_SESSION['quiz'][$quiz_id][$key])) {
      $this->session = &$_SESSION['quiz'][$quiz_id][$key];
    }
  }

  /**
   * {@inheritdoc}
   */
  public function onLoad(Answer $answer) {
    $sql = 'SELECT answer, score FROM {quizz_pool_answer} WHERE answer_id = :id';
    if ($input = db_query($sql, array(':id' => $answer->id))->fetchObject()) {
      $answer->setInput($input);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function setAnswerInput($input) {
    if (NULL !== $input && is_array($input)) {
      $input = reset($input);
    }
    parent::setAnswerInput($input);
  }

  /**
   * {@inheritdoc}
   */
  public function isValid() {
    if (2 == $this->answer) { // @TODO Number 2 here is too magic.
      drupal_set_message(t("You haven't completed the quiz pool"), 'warning');
      return FALSE;
    }
    return parent::isValid();
  }

  /**
   * @return Question
   */
  private function getQuestion() {
    if (!empty($this->session)) {
      $passed = $this->session['passed'];
      $delta = $this->session['delta'];
      return entity_metadata_wrapper('quiz_question_entity', $this->question)
          ->field_question_reference[$passed ? $delta - 1 : $delta]
          ->value();
    }

    $question_vid = db_select('quizz_pool_answer_attemp', 'attemp')
      ->fields('attemp', array('question_vid'))
      ->condition('answer_id', $this->loadAnswerEntity()->id)
      ->condition('is_evaluated', 1)
      ->execute()
      ->fetchColumn();

    return $question_vid ? quizz_question_load(NULL, $question_vid) : NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    $passed = &$this->session['passed'];
    $delta = &$this->session['delta'];

    $wrapper = entity_metadata_wrapper('quiz_question_entity', $this->question);
    if ($question = $wrapper->field_question_reference[$delta]->value()) {
      $answer = $this->evaluateQuestion($question);
      if ($answer->is_valid) {
        $passed = $answer->is_correct ? TRUE : $passed;
        if ($delta < $wrapper->field_question_reference->count()) {
          $delta++;
        }
      }
    }

    db_merge('quizz_pool_answer')
      ->key(array('answer_id' => $this->loadAnswerEntity()->id))
      ->fields(array(
          'score'  => (int) $this->getScore(),
          'answer' => (int) $this->answer,
      ))
      ->execute()
    ;
  }

  private function evaluateQuestion(Question $question) {
    $handler = $question->getResponseHandler($this->result_id, $this->answer);
    $answer = $handler->toBareObject();

    if (isset($this->answer)) {
      db_merge('quizz_pool_answer_attemp')
        ->key(array(
            'answer_id'    => $this->loadAnswerEntity()->id,
            'question_vid' => $question->vid,
        ))
        ->fields(array(
            'question_qid' => $question->qid,
            'answer'       => serialize($this->answer),
            'is_correct'   => $answer->is_correct,
            'is_evaluated' => (int) $handler->isEvaluated(),
            'score'        => (int) $handler->score(),
        ))
        ->execute()
      ;
    }

    // fix error with score
    if ($this->result->score < 0) {
      $this->result->score = 0;
    }

    return $answer;
  }

  /**
   * {@inheritdoc}
   */
  public function delete() {
    parent::delete();

    if (!isset($this->question->created)) {
      db_delete('quizz_pool_answer_attemp')
        ->condition('answer_id', $this->loadAnswerEntity()->id)
        ->execute();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    return $this->isCorrect() ? $this->getQuestionMaxScore() : 0;
  }

  /**
   * {@inheritdoc}
   */
  public function isCorrect() {
    $handler = $this->getQuestion()->getResponseHandler($this->result_id, $this->answer);
    $handler->setAnswerInput($this->answer);
    return $handler->isCorrect();
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedbackValues() {
    if (!$question = $this->getQuestion()) {
      return array('#markup' => t('No question passed.'));
    }
    return $question->getResponseHandler($this->result_id)->getFeedbackValues();
  }

}
