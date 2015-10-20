<?php

namespace Drupal\quizz_scale\Form\ConfigForm;

use Drupal\quizz_scale\Entity\CollectionController;
use Drupal\quizz_scale\ScaleQuestion;

class FormSubmit {

  /** @var CollectionController */
  private $controller;

  public function __construct() {
    $this->controller = quizz_scale_collection_controller();
  }

  private function getQuestionHandler($question_type) {
    $question = entity_create('quiz_question_entity', array('type' => $question_type));
    return new ScaleQuestion($question);
  }

  public function submit($form, &$form_state) {
    $handler = $this->getQuestionHandler($form_state['quiz_question_type']->type);
    $changed = $deleted = 0;
    foreach ($form_state['values']['configuration']['collections'] as $key => $input) {
      $matches = array();
      preg_match('/^collection([0-9]{1,}|new)$/', $key, $matches);
      if ($matches && ($collection_id = $matches[1])) {
        $this->doSubmitAlternatives($handler, $collection_id, $input, $changed, $deleted);
      }
    }

    if ($changed) {
      drupal_set_message(t('!changed presets have been changed.', array('!changed' => $changed)));
    }

    if ($deleted) {
      drupal_set_message(t('!deleted presets have been deleted.', array('!deleted' => $deleted)));
    }
  }

  private function doSubmitAlternatives(ScaleQuestion $handler, $collection_id, $input, &$changed, &$deleted) {
    switch ($input['to-do']) {
      case 'save_safe': // Save, but don't change
      case 'save': // Save and change existing questions
        if (FALSE !== $this->doSubmitSave($handler, $input, $collection_id)) {
          $changed++;
        }
        break;

      // Delete
      case 'delete_safe':
        if (!$got_deleted = $this->controller->deleteCollectionIfNotUsed($collection_id)) {
          $this->controller->unpresetCollection($collection_id);
        }
        $deleted++;
        break;

      case 'save_new':
        $this->doSubmitNew($handler, $input);
        break;
    }
  }

  private function doSubmitSave(ScaleQuestion $handler, $input, $collection_id) {
    $for_all = isset($input['for_all']) ? $input['for_all'] : NULL;
    $label = check_plain($input['label']);

    $new_collection_id = $this->controller
      ->getWriting()
      ->write($handler->question, FALSE, $input, 1, $for_all, $label, $collection_id);

    if ($new_collection_id == $collection_id) {
      return FALSE;
    }

    // We save the changes, but don't change existing questions
    if ('save_safe' === $input['to-do']) {
      $this->doSubmitSaveSafe($collection_id);
    }

    if ('save' === $input['to-do']) {
      $this->doSubmitSaveNormal($collection_id, $new_collection_id);
    }
  }

  private function doSubmitSaveSafe($collection_id) {
    // The old version of the collection shall not be a preset anymore
    $this->controller->unpresetCollection($collection_id);

    // If the old version of the collection doesn't belong to any questions it is safe to delete it.
    $this->controller->deleteCollectionIfNotUsed($collection_id);
  }

  private function doSubmitSaveNormal($collection_id, $new_collection_id) {
    // Update all the users questions where the collection is used
    $sql = 'SELECT qid FROM {quiz_question_entity} WHERE uid = :uid';
    $question_ids = db_query($sql, array(':uid' => 1))->fetchCol();

    db_update('quiz_scale_question')
      ->fields(array('collection_id' => $new_collection_id))
      ->condition('qid', $question_ids)
      ->execute();

    $this->controller->deleteCollectionIfNotUsed($collection_id);
  }

  private function doSubmitNew(ScaleQuestion $handler, $input) {
    if (drupal_strlen($input['alternative0']) > 0) {
      $collection_id = $handler->saveAnswerCollection($handler->question, FALSE, $input, 1);
      $this->controller->setForAll($collection_id, $input['for_all']);
      drupal_set_message(t('New preset has been added'));
    }
  }

}
