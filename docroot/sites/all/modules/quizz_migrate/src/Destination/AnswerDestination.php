<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz_migrate\BaseEntityDestination;

class AnswerDestination extends BaseEntityDestination {

  protected $entity_type = 'quiz_result_answer';
  protected $base_table = 'quiz_answer_entity';
  protected static $pk_name = 'id';
  protected $disable_import_complete = TRUE;

}
