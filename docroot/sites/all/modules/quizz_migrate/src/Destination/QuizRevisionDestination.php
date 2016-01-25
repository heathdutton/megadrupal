<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz_migrate\Destination\QuizDestination;

class QuizRevisionDestination extends QuizDestination {

  protected $base_table = 'quiz_entity_revision';
  protected static $pk_name = 'vid';

  public function fields($migration = NULL) {
    $fields = array();

    $schema = drupal_get_schema('quiz_entity');
    foreach ($schema['fields'] as $name => $info) {
      $fields[$name] = isset($info['description']) ? $info['description'] : $name;
    }

    return $fields + parent::fields($migration);
  }

  protected function doBulkRollBack($ids) {
    foreach ($ids as $revision_id) {
      entity_revision_delete($this->entity_type, $revision_id);
    }
  }

}
