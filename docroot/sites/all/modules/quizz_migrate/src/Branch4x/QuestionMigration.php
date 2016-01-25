<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\QuestionDestination;
use MigrateSourceSQL;

class QuestionMigration extends QuizMigration {

  protected $description = 'Migrate quiz question node to entity.';
  protected $entityType = 'quiz_question_entity';
  protected $bodyFieldName = 'quiz_question_body';
  protected $bundle = NULL;

  public function __construct($arguments = array()) {
    $this->bundle = $arguments['bundle'];
    $this->machineName = "quiz_question__{$this->bundle}";
    parent::__construct($arguments);
  }

  protected function setupMigrateDestination() {
    return new QuestionDestination($this->bundle);
  }

  protected function setupMigrateSource() {
    $query = db_select('node', 'n');
    $query->join('quiz_question_properties', 'p', 'n.vid = p.vid');
    $query
      ->fields('n', array('nid', 'vid', 'uid', 'language', 'title', 'created', 'status', 'changed'))
      ->fields('p', array('max_score'))
      ->orderBy('n.nid', 'ASC')
      ->condition('n.type', $this->bundle)
    ;
    $this->setUpMigateSourceFields($query);
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('max_score'));
    parent::setupFieldMapping();
  }

}
