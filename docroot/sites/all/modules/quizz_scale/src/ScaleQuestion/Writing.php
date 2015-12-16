<?php

namespace Drupal\quizz_scale\ScaleQuestion;

use Drupal\quizz_scale\Entity\CollectionController;
use Drupal\quizz_question\Entity\Question;

/**
 * Helper class to write question's alternatives
 */
class Writing {

  private $controller;

  public function __construct(CollectionController $controller) {
    $this->controller = $controller;
  }

  /**
   * Stores the answer collection to the database, or identifies an existing collection.
   *
   * We try to reuse answer collections as much as possible to minimize the amount of rows in the database,
   * and thereby improving performance when surveys are beeing taken.
   *
   * @param bool $is_new - the question is beeing inserted(not updated)
   * @param string[] $in_alternatives - the alternatives array to be saved.
   * @param int $preset
   * @param int|null $for_all
   * @param string|null $label
   * @return int Answer collection id
   */
  public function write(Question $question, $is_new, array $in_alternatives, $preset, $for_all = NULL, $label = NULL, $collection_id = NULL) {
    $alternatives = array();
    for ($i = 0; $i < $question->getQuestionType()->getConfig('scale_max_num_of_alts', 10); $i++) {
      if (isset($in_alternatives['alternative' . $i]) && drupal_strlen($in_alternatives['alternative' . $i]) > 0) {
        $alternatives[] = $in_alternatives['alternative' . $i];
      }
    }

    // If an identical answer collection already exists
    if ((NULL !== $collection_id) || $collection_id = $this->findCollectionId($question->type, $alternatives)) {
      $this->doWriteExistingCollection($collection_id, $question, $is_new, $preset, $for_all, $label);
    }
    else {
      $collection_id = $this->doWriteNewCollection($alternatives, $preset, $for_all, $label);
    }

    if (!empty($question->vid)) {
      db_merge('quiz_scale_question')
        ->key(array('qid' => $question->qid, 'vid' => $question->vid))
        ->fields(array('collection_id' => $collection_id))
        ->execute()
      ;
    }
  }

  private function doWriteExistingCollection($collection_id, $question, $is_new, $preset, $for_all, $label) {
    global $user;

    // We try to delete the old answer collection
    if (!$is_new & !empty($question->{0})) {
      $collection_id_to_delete = $question->{0}->collection_id;
      if ($collection_id_to_delete != $collection_id) {
        $this->controller->deleteCollectionIfNotUsed($collection_id_to_delete, 1);
      }
    }

    if ($preset || (NULL !== $for_all) || (NULL !== $label)) {
      $collection = entity_load_single('scale_collection', $collection_id);

      if ($preset) {
        $collection->uid = $user->uid;
      }

      if (NULL !== $for_all) {
        $collection->for_all = $for_all;
      }

      if (NULL !== $label) {
        $collection->label = $label;
      }

      $collection->save();
    }
  }

  private function doWriteNewCollection($alternatives, $preset, $for_all, $label) {
    global $user;

    // Register a new answer collection
    $collection = entity_create('scale_collection', array(
        'for_all' => NULl !== $for_all ? $for_all : 1,
        'label'   => NULL !== $label ? check_plain($label) : '',
        'uid'     => $preset ? $user->uid : NULL,
        'name'    => 'collection_' . REQUEST_TIME . rand(0, 1000),
    ));
    $collection->save();
    $collection->insertAlternatives($alternatives);
    return $collection->id;
  }

  /**
   * Finds out if a collection already exists.
   *
   * @param string $question_type
   * @param string[] $in_alternatives
   * @param int $collection_id
   * @param int $last_id - The id of the last alternative we compared with.
   * @return bool
   */
  private function findCollectionId($question_type, array $in_alternatives, $collection_id = NULL, $last_id = NULL) {
    $alternatives = isset($collection_id) ? $in_alternatives : array_reverse($in_alternatives);

    // Find all answers identical to the next answer in $alternatives
    $select = db_select('quiz_scale_collection_item', 'collection_items');
    $select->innerJoin('quiz_scale_collection', 'collection', 'collection_items.collection_id = collection.id');
    $select
      ->fields('collection_items', array('id', 'collection_id'))
      ->condition('collection_items.answer', array_pop($alternatives))
      ->condition('collection.question_type', $question_type)
    ;

    // Filter on collection ID
    if (isset($collection_id)) {
      $select->condition('collection_items.collection_id', $collection_id);
    }

    // Filter on alternative id (If we are investigating a specific collection,
    // the alternatives needs to be in a correct order)
    if (NULL !== $last_id) {
      $select->condition('collection_items.id', $last_id + 1);
    }

    if (!$_alternatives = $select->execute()->fetchAll()) {
      return FALSE;
    }

    // If all alternatives has matched make sure the collection we are comparing
    // against in the database doesn't have more alternatives.
    if (!$alternatives && NULL !== $collection_id && NULL !== $last_id) {
      $sql = 'SELECT 1 FROM {quiz_scale_collection_item} WHERE collection_id = :cid AND id = :id';
      if (db_query($sql, array(':cid' => $collection_id, ':id' => $last_id + 2))->fetchColumn()) {
        return $collection_id;
      }
      return FALSE;
    }

    // Do a recursive call to this function on all answer collection candidates
    foreach ($_alternatives as $alternative) {
      $aid = $alternative->id;
      $cid = $alternative->collection_id;
      if ($collection_id = $this->findCollectionId($question_type, $alternatives, $cid, $aid)) {
        return $collection_id;
      }
    }

    return FALSE;
  }

}
