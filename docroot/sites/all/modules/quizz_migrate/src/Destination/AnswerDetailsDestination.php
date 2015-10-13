<?php

namespace Drupal\quizz_migrate\Destination;

use MigrateDestinationTable;
use stdClass;

class AnswerDetailsDestination extends MigrateDestinationTable {

  public function import(stdClass $entity, stdClass $row) {
    parent::import($entity, $row);
    return array($entity->answer_id);
  }

}
