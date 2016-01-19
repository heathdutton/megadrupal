<?php

namespace Drupal\quizz_scale;

use Drupal\quizz_question\Entity\QuestionType;
use Drupal\quizz_question\QuestionHandler;
use Drupal\quizz_scale\Form\ConfigForm\FormDefinition;
use Drupal\quizz_scale\Form\ScaleQuestionForm;

/**
 * @TODO: We mix the names answer_collection and alternatives. Use either
 * alternative or answer consistently
 */
class ScaleQuestion extends QuestionHandler {

  protected $base_answer_table = 'quizz_scale_answer';
  protected $base_table = 'quiz_scale_question';

  public function onNewQuestionTypeCreated(QuestionType $question_type) {
    $return = parent::onNewQuestionTypeCreated($question_type);

    quizz_scale_collection_controller()->generateDefaultCollections($question_type);

    return $return;
  }

  /**
   * {@inheritdoc}
   */
  public function onSave($is_new = FALSE) {
    if ($this->question->revision == 1) {
      $is_new = TRUE;
    }

    $alternatives = array();
    foreach ($this->question as $property => $value) {
      if (0 === strpos($property, 'alternative')) {
        $alternatives[$property] = trim($value);
      }
    }

    $preset = !empty($this->question->save_new_collection) ? $this->question->save_new_collection : 0;
    $label = empty($this->question->save_new_collection) ? NULL : $this->question->new_collection_label;

    quizz_scale_collection_controller()
      ->getWriting()
      ->write($this->question, $is_new, $alternatives, $preset, NULL, $label)
    ;
  }

  /**
   * {@inheritdoc}
   */
  public function delete($single_revision = FALSE) {
    $cid = $this->question->{0}->collection_id;
    quizz_scale_collection_controller()->deleteCollectionIfNotUsed($cid, 0);
    parent::delete($single_revision);
  }

  /**
   * {@inheritdoc}
   */
  public function load() {
    if (empty($this->properties)) {
      $this->properties = parent::load();

      $select = db_select('quiz_scale_question', 'p');
      $select->join('quiz_scale_collection_item', 'collection_items', 'p.collection_id = collection_items.collection_id');
      $properties = $select
        ->fields('collection_items', array('id', 'answer', 'collection_id'))
        ->condition('p.vid', $this->question->vid)
        ->orderBy('collection_items.id')
        ->execute()
        ->fetchAll();
      foreach ($properties as $property) {
        $this->properties[] = $property;
      }
    }
    return $this->properties;
  }

  /**
   * {@inheritdoc}
   */
  public function view() {
    $content = parent::view();
    $alternatives = array();
    for ($i = 0; $i < $this->question->getQuestionType()->getConfig('scale_max_num_of_alts', 10); $i++) {
      if (isset($this->question->{$i}->answer) && drupal_strlen($this->question->{$i}->answer) > 0) {
        $alternatives[] = check_plain($this->question->{$i}->answer);
      }
    }
    $content['answer'] = array(
        '#markup' => theme('quizz_scale_answer_question_view', array('alternatives' => $alternatives)),
        '#weight' => 2,
    );
    return $content;
  }

  /**
   * {@inheritdoc}
   */
  public function getAnsweringForm(array $form_state = NULL, $result_id) {
    $options = array();
    for ($i = 0; $i < $this->question->getQuestionType()->getConfig('scale_max_num_of_alts', 10); $i++) {
      if (isset($this->question->{$i}) && drupal_strlen($this->question->{$i}->answer) > 0) {
        $options[$this->question->{$i}->id] = check_plain($this->question->{$i}->answer);
      }
    }

    $form = array('#type' => 'radios', '#title' => t('Choose one'), '#options' => $options);
    if (isset($result_id)) {
      $response = new ScaleResponse($result_id, $this->question);
      $form['#default_value'] = $response->getResponse();
    }

    return $form;
  }

  /**
   * Question response validator.
   */
  public function validateAnsweringForm(array &$form, array &$form_state = NULL) {
    if (!$form_state['values']['question'][$this->question->qid]['answer']) {
      form_set_error('', t('You must provide an answer.'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getCreationForm(array &$form_state = NULL) {
    $obj = new ScaleQuestionForm($this->question);
    return $obj->get($form_state);
  }

  /**
   * {@inheritdoc}
   *
   * In some use-cases we want to reward users for answering a survey question.
   * This is why 1 is returned and not zero.
   */
  public function getMaximumScore() {
    return 1;
  }

  /**
   * {@inheritdoc}
   */
  function questionTypeConfigForm(QuestionType $question_type) {
    require_once drupal_get_path('module', 'quizz_scale') . '/quizz_scale.pages.inc';
    $obj = new FormDefinition($question_type);
    return $obj->get();
  }

}
