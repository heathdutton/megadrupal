<?php

namespace Drupal\quizz_pool;

use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\QuestionHandler;
use Drupal\quizz\Entity\Result;

class PoolQuestionHandler extends QuestionHandler {

  protected $base_answer_table = 'quizz_pool_answer';

  /**
   * {@inheritdoc}
   */
  public function load() {
    $properties = parent::load();

    if (empty($this->question->field_question_reference['und'])) {
      return $properties;
    }

    // Referenced question maybe deleted. Remove them if it was
    /// @TODO: This should be resolved at entityreference module
    $ref_items = field_get_items('quiz_question_entity', $this->question, 'field_question_reference');
    $this->question->field_question_reference['und'] = array();
    $field_items = &$this->question->field_question_reference['und'];
    foreach ($ref_items as $ref_item) {
      if ($ref_question = quizz_question_load($ref_item['target_id'])) {
        $field_items[]['target_id'] = $ref_item['target_id'];
      }
    }

    return $properties;
  }

  /**
   * {@inheritdoc}
   */
  public function delete($single_revision) {
    $key = $single_revision ? 'question_vid' : 'question_qid';
    $id = $single_revision ? $this->question->vid : $this->question->qid;

    db_query('DELETE attemp'
      . ' FROM {quizz_pool_answer_attemp} attemp'
      . ' INNER JOIN {quiz_answer_entity} answer ON attemp.answer_id = answer.id'
      . " WHERE answer.{$key} = :id", array(
        ':id' => $id
      )
    );

    parent::delete($single_revision);
  }

  /**
   * {@inheritdoc}
   */
  public function view() {
    $build = parent::view();
    $wrapper = entity_metadata_wrapper('quiz_question_entity', $this->question);

    /* @var $sub_question Question */
    $markup = '';
    foreach ($wrapper->field_question_reference->getIterator() as $sub_wrapper) {
      $sub_question = $sub_wrapper->value();
      if ($content = $sub_question->getHandler()->view() && !empty($content['answer'])) {
        $markup .= "<h3>{$sub_question->title}</h3>";
        $markup .= $content['answer']['#markup'];
      }
    }
    $build['answers']['#markup'] = $markup;
    return $build;
  }

  private function getCurrentQuestion($quiz_id, $retry = FALSE) {
    $session = &$_SESSION['quiz'][$quiz_id];
    $key = "pool_{$this->question->qid}";
    if (!isset($_SESSION['quiz'][$quiz_id][$key])) {
      $_SESSION['quiz'][$quiz_id][$key] = array('passed' => FALSE, 'delta' => 0);
    }
    $wrapper = entity_metadata_wrapper('quiz_question_entity', $this->question);
    $delta = &$session[$key]['delta'];

    if ($retry) {
      $delta++;
      if (!isset($wrapper->field_question_reference[$delta])) {
        $delta = 0;
      }
      drupal_goto($_GET['q']);
    }

    return $wrapper->field_question_reference[$delta]->value();
  }

  /**
   * Generates the question form.
   *
   * This is called whenever a question is rendered, either
   * to an administrator or to a quiz taker.
   */
  public function getAnsweringForm(array $form_state = NULL, $result_id) {
    $quiz = quizz_result_load($result_id)->getQuiz();
    $retry = $quiz->repeat_until_correct && !empty($_GET['retry']);
    $question = $this->getCurrentQuestion($quiz->qid, $retry);
    $form = array();
    $form[$question->qid] = $question->getHandler()->getAnsweringForm($form_state, $result_id);

    if ($quiz->repeat_until_correct) {
      $form['navigation']['pool_retry'] = array(
          '#type'   => 'markup',
          '#markup' => t('Try an <a href="!url">other question</a>', array(
              '!url' => url($_GET['q'], array('query' => array('retry' => 1)))
          )),
          '#weight' => 50,
      );
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function getMaximumScore() {
    $score = 0;
    $wrapper = entity_metadata_wrapper('quiz_question_entity', $this->question);
    /* @var $question Question */
    foreach ($wrapper->field_question_reference->getIterator() as $wrapper_question) {
      if ($question = $wrapper_question->value()) {
        $score += $question->getHandler()->getMaximumScore();
      }
    }
    return $score;
  }

  /**
   * {@inheritdoc}
   */
  public function onRepeatUntiCorrect(Result $result, array &$element) {
    $msg = t('The answer was incorrect. Please try again.');
    $msg .= ' ' . t('You can try with <a href="!url">another question</a>.', array(
          '!url' => url(check_plain($_GET['q']), array('query' => array('retry' => 1)))
    ));
    return parent::onRepeatUntiCorrect($result, $element, $msg);
  }

}
