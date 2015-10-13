<?php

namespace Drupal\quizz_migrate\Destination;

use MigrateDestinationTable;
use stdClass;

class QuestionDetailsDestination extends MigrateDestinationTable {

  public function import(stdClass $entity, stdClass $row) {
    parent::import($entity, $row);
    return array($entity->qid);
  }

}
