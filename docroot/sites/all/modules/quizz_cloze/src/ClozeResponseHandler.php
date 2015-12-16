<?php

namespace Drupal\quizz_cloze;

use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\ResponseHandler;

/**
 * Extension of QuizQuestionResponse
 */
class ClozeResponseHandler extends ResponseHandler {

  /** {@inheritdoc} */
  protected $base_table = 'quiz_cloze_answer';

  /** @var Helper */
  private $helper;

  public function __construct($result_id, Question $question, $input = NULL) {
    parent::__construct($result_id, $question, $input);
    $this->helper = new Helper();

    if (NULL === $input) {
      if ($stored_input = $this->getStoredUserInput()) {
        $this->answer = unserialize($stored_input->answer);
        $this->score = $stored_input->score;
      }
    }
    else {
      $this->answer = $input;
    }
  }

  private function getStoredUserInput() {
    return db_query(
        "SELECT answer_id, answer, score"
        . " FROM {quiz_cloze_answer}"
        . " WHERE answer_id = :answer_id", array(
          ':answer_id' => $this->loadAnswerEntity()->id
      ))->fetch();
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    db_merge('quiz_cloze_answer')
      ->key(array('answer_id' => $this->loadAnswerEntity()->id))
      ->fields(array(
          'answer' => serialize($this->answer),
          'score'  => $this->getScore(FALSE),
      ))
      ->execute()
    ;
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    return $this->question->getHandler()->evaluateAnswer($this->answer);
  }

  /**
   * {@inheritdoc}
   */
  public function getReportForm(array $form = array()) {
    $form += parent::getReportForm($form);

    $s_question = strip_tags($this->question->quiz_question_body[LANGUAGE_NONE][0]['value']);
    $question_form = array();
    $question_form['#attached']['css'][] = drupal_get_path('module', 'quizz_cloze') . '/misc/cloze.css';
    $question_form['open_wrapper']['#markup'] = '<div class="cloze-question">';

    foreach ($this->helper->getQuestionChunks($s_question) as $position => $chunk) {
      if (strpos($chunk, '[') === FALSE) {
        $question_form['parts'][$position] = array(
            '#markup' => str_replace("\n", "<br/>", $chunk),
            '#prefix' => '<div class="form-item">',
            '#suffix' => '</div>',
        );
        continue;
      }

      $choices = explode(',', str_replace(array('[', ']'), '', $chunk));
      if (count($choices) > 1) {
        $question_form['parts'][$position] = array(
            '#type'     => 'select',
            '#options'  => $this->helper->shuffleChoices(drupal_map_assoc($choices)),
            '#required' => FALSE,
        );
        continue;
      }

      $question_form['parts'][$position] = array(
          '#type'       => 'textfield',
          '#size'       => 32,
          '#required'   => FALSE,
          '#attributes' => array('autocomplete' => 'off'),
          '#value'      => $this->answer['parts'][$position]
      );
    }

    $question_form['close_wrapper']['#markup'] = '</div>';
    $form['question']['#markup'] = drupal_render($question_form);

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedbackValues() {
    if (!isset($this->question->quiz_question_body[LANGUAGE_NONE][0]['value'])) {
      drupal_set_message(json_encode($this->question));
    }

    $s_question = strip_tags($this->question->quiz_question_body[LANGUAGE_NONE][0]['value']);
    $inputs = isset($this->answer['parts']) ? $this->answer['parts'] : array();
    $corrects = $this->helper->getCorrectAnswerChunks($s_question);

    return array(
        array(
            'choice'            => theme('item_list', array('items' => $inputs)),
            'attempt'           => $this->answer,
            'correct'           => quizz_icon($this->isCorrect() ? 'correct' : 'incorrect'),
            'score'             => $this->getScore(),
            'answer_feedback'   => '',
            'question_feedback' => '',
            'solution'          => theme('item_list', array('items' => $corrects)),
        )
    );
  }

  private function getCorrectAnswer($question) {
    $chunks = $this->helper->getQuestionChunks($question);
    $answer = $this->helper->getCorrectAnswerChunks($question);
    $correct_answer = array();
    foreach ($chunks as $key => $chunk) {
      if (isset($answer[$key])) {
        $correct_answer[] = '<span class="answer correct correct-answer">' . $answer[$key] . '</span>';
      }
      else {
        $correct_answer[] = $chunk;
      }
    }
    return str_replace("\n", "<br/>", implode(' ', $correct_answer));
  }

  public function getReportFormScore() {
    return array('#markup' => $this->getScore());
  }

}
