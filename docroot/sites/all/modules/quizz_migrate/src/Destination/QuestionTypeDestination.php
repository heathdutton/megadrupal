<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz_migrate\BaseEntityDestination;
use Drupal\quizz_question\Entity\QuestionType;

class QuestionTypeDestination extends BaseEntityDestination {

  protected $entity_type = 'quiz_question_type';
  protected static $pk_name = 'type';

  static public function getKeySchema() {
    return array(
        'type' => array(
            'type'        => 'varchar',
            'length'      => 32,
            'not null'    => TRUE,
            'default'     => '',
            'description' => 'Machine name of question type entity.',
        ),
    );
  }

  /**
   * @param QuestionType $entity
   */
  protected function doImport($entity, $row) {
    $return = parent::doImport($entity, $row);
    quizz_migrate_enable_question_type($entity->type);
    return $return;
  }

}
