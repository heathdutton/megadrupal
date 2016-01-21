<?php

namespace Drupal\quizz_scale\Entity;

use DatabaseTransaction;
use Drupal\quizz_question\Entity\QuestionType;
use Drupal\quizz_scale\ScaleQuestion\Writing;
use EntityAPIControllerExportable;

class CollectionController extends EntityAPIControllerExportable {

  /** @var Writing */
  private $writing;

  public function getWriting() {
    if (NULL === $this->writing) {
      $this->writing = new Writing($this);
    }
    return $this->writing;
  }

  public function save($entity, \DatabaseTransaction $transaction = NULL) {
    if (!$entity->name) {
      $entity->name = 'collection_' . REQUEST_TIME . rand(1, 1000);
    }

    if (!$entity->label) {
      $entity->label = $entity->name;
    }

    return parent::save($entity, $transaction);
  }

  public function load($ids = array(), $conditions = array()) {
    $collections = parent::load($ids, $conditions);

    if (!empty($collections)) {
      $alternatives = db_select('quiz_scale_collection_item')
        ->fields('quiz_scale_collection_item')
        ->condition('collection_id', array_keys($collections))
        ->execute()
        ->fetchAll();

      foreach ($alternatives as $alternative) {
        $collections[$alternative->collection_id]->alternatives[$alternative->id] = check_plain($alternative->answer);
      }
    }

    return $collections;
  }

  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $return = parent::delete($ids, $transaction);

    // Delete alternatives
    db_delete('quiz_scale_collection_item')
      ->condition('collection_id', $ids)
      ->execute();

    return $return;
  }

  /**
   * Get all available presets for a user.
   *
   * @param string $question_type
   * @param int $uid
   * @param bool $with_defaults
   * @return Collection[]
   */
  public function getPresetCollections($question_type, $uid, $with_defaults = FALSE) {
    $select = db_select('quiz_scale_collection', 'collection');
    $select->fields('collection', array('id'));
    $select->condition('question_type', $question_type);

    if (!$with_defaults) {
      $select->condition('collection.uid', $uid);
    }
    else {
      $select->condition(
        db_or()
          ->condition('collection.uid', $uid)
          ->condition('collection.for_all', 1)
      );
    }

    if ($collection_ids = $select->execute()->fetchCol()) {
      return $this->load($collection_ids);
    }

    return array();
  }

  /**
   * Make sure an answer collection isn't a preset for a given user.
   *
   * @param int $collection_id
   */
  public function unpresetCollection($collection_id) {
    $collection = quizz_scale_collection_entity_load($collection_id);
    $collection->for_all = 0;
    $collection->save();
  }

  /**
   * Make an answer collection (un)available for all question creators.
   *
   * @param int $collection_id
   * @param bool $for_all
   */
  public function setForAll($collection_id, $for_all) {
    $collection = quizz_scale_collection_entity_load($collection_id);
    if ($for_all != $collection->for_all) {
      $collection->for_all = $for_all;
      $collection->save();
    }
  }

  /**
   * Add a preset for the current user.
   *
   * @param $collection_id - answer collection id of the collection this user wants to have as a preset
   */
  public function changeOwner($collection_id, $uid) {
    $collection = quizz_scale_collection_entity_load($collection_id);
    if ($uid != $collection->uid) {
      $collection->uid = $uid;
      $collection->save();
    }
  }

  /**
   * Deletes an answer collection if it isn't beeing used.
   *
   * @param $collection_id
   * @param $accept
   *  If collection is used more than this many times we keep it.
   * @return
   *  true if deleted, false if not deleted.
   */
  public function deleteCollectionIfNotUsed($collection_id, $accept = 0) {
    // Check if the collection is someones preset. If it is we can't delete it.
    $sql_1 = 'SELECT 1 FROM {quiz_scale_collection} WHERE id = :id AND uid <> 0';
    if (db_query($sql_1, array(':id' => $collection_id))->fetchField()) {
      return FALSE;
    }

    // Check if the collection is a global preset. If it is we can't delete it.
    $sql_2 = 'SELECT 1 FROM {quiz_scale_collection} WHERE id = :id AND for_all = 1';
    if (db_query($sql_2, array(':id' => $collection_id))->fetchField()) {
      return FALSE;
    }

    // Check if the collection is used in an existing question. If it is we can't delete it.
    $sql_3 = 'SELECT COUNT(*) FROM {quiz_scale_question} WHERE collection_id = :acid';
    $count = db_query($sql_3, array(':acid' => $collection_id))->fetchField();

    // We delete the answer collection if it isnt beeing used by enough questions
    if ($count <= $accept) {
      entity_delete('scale_collection', $collection_id);
      return TRUE;
    }

    return FALSE;
  }

  public function generateDefaultCollections(QuestionType $question_type) {
    $alternatives = array(
        array('Always', 'Very often', 'Some times', 'Rarely', 'Very rarely', 'Never'),
        array('Excellent', 'Very good', 'Good', 'Ok', 'Poor', 'Very poor'),
        array('Totally agree', 'Agree', 'Not sure', 'Disagree', 'Totally disagree'),
        array('Very important', 'Important', 'Moderately important', 'Less important', 'Least important'),
    );

    /* @var $collection Collection */
    foreach ($alternatives as $_alternatives) {
      $collection = entity_create('scale_collection', array(
          'question_type' => $question_type->type,
          'for_all'       => TRUE,
          'uid'           => 1,
          'label'         => $_alternatives[0] . ' - ' . $_alternatives[count($_alternatives) - 1],
      ));
      $collection->save();
      $collection->insertAlternatives($_alternatives);
    }
  }

}
