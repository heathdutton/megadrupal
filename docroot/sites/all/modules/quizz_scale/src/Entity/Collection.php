<?php

namespace Drupal\quizz_scale\Entity;

use Entity;

class Collection extends Entity {

  /** @var int */
  public $id;

  /** @var string */
  public $name;

  /** @var string */
  public $label;

  /** @var bool */
  public $for_all;

  /** @var int */
  public $uid;

  /**
   * ID -> Label
   *
   * @var array
   */
  public $alternatives = array();

  public function insertAlternatives($answers) {
    foreach ($answers as $answer) {
      $this->insertAlternative($answer);
    }
  }

  /**
   * Insert new answer.
   * @param string $answer
   */
  public function insertAlternative($answer) {
    if (!$this->id) {
      $this->save();
    }

    return db_insert('quiz_scale_collection_item')
        ->fields(array(
            'collection_id' => $this->id,
            'answer'        => $answer
        ))
        ->execute()
    ;
  }

}
