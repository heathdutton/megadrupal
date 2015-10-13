<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz_migrate\BaseEntityDestination;
use Drupal\quizz_question\Entity\QuestionController;

class QuestionDestination extends BaseEntityDestination {

  protected $entity_type = 'quiz_question_entity';
  protected $bundle;
  protected $base_table = 'quiz_question_entity';
  protected static $pk_name = 'qid';

  public function __construct($bundle, $options = array()) {
    parent::__construct(array('bundle' => $bundle) + $options);
  }

  protected function doImport($entity, $row) {
    QuestionController::$disable_invoking = TRUE;
    $return = parent::doImport($entity, $row);
    QuestionController::$disable_invoking = FALSE;
    return $return;
  }

}
