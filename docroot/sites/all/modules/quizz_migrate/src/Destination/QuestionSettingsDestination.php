<?php

namespace Drupal\quizz_migrate\Destination;

use MigrateDestination;
use stdClass;

class QuestionSettingsDestination extends MigrateDestination {

  public function __toString() {
    return '';
  }

  public static function getKeySchema() {
    return array(
        'migration' => array('type' => 'varchar', 'length' => 128, 'not null' => TRUE),
        'bundle'    => array('type' => 'varchar', 'length' => 32, 'not null' => TRUE),
        'name'      => array('type' => 'varchar', 'length' => 128, 'not null' => TRUE),
    );
  }

  public function fields() {
    return array(
        'bundle' => 'Quiz type (Bundle name)',
        'name'   => 'Variable name',
        'value'  => 'Variable value',
    );
  }

  /**
   * Do nothing
   */
  public function rollback() {

  }

  public function import(stdClass $object, stdClass $row) {
    quizz_question_type_load($object->bundle)
      ->setConfig($object->name, $object->value)
      ->save()
    ;
    return array($row->migration, $object->bundle, $object->name);
  }

}
