<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz_migrate\BaseEntityDestination;

class QuizDestination extends BaseEntityDestination {

  protected $entity_type = 'quiz_entity';
  protected $base_table = 'quiz_entity';
  protected $bundle = 'quiz';
  protected static $pk_name = 'qid';

}
