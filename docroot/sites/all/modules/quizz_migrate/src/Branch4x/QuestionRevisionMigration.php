<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Branch4x\QuestionMigration;
use Drupal\quizz_migrate\Destination\QuestionRevisionDestination;
use MigrateSourceSQL;

class QuestionRevisionMigration extends QuestionMigration {

  protected $description = 'Migrate quiz question node to entity.';
  protected $entityType = 'quiz_question_entity';
  protected $bodyFieldName = 'quiz_question_body';
  protected $bundle = NULL;
  protected $sourcePK = array(
      'vid' => array(
          'type'        => 'int',
          'not null'    => TRUE,
          'description' => 'Node',
          'alias'       => 'r',
      )
  );

  public function __construct($arguments = array()) {
    $this->bundle = $arguments['bundle'];
    $this->dependencies[] = "quiz_question__{$this->bundle}";
    $this->machineName = "quiz_question_revision__{$this->bundle}";
    parent::__construct($arguments);
  }

  protected function setupMigrateDestination() {
    return new QuestionRevisionDestination($this->bundle);
  }

  protected function setupMigrateSource() {
    $query = db_select('node_revision', 'r');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->join('quiz_question_properties', 'p', 'r.vid = p.vid');
    $query->addField('r', 'uid', 'revision_uid');
    $query
      ->fields('r', array('nid', 'vid', 'log', 'timestamp'))
      ->fields('n', array('uid', 'language', 'title', 'created', 'status', 'changed'))
      ->fields('p', array('max_score'))
      ->orderBy('n.nid', 'ASC')
      ->condition('n.type', $this->bundle)
    ;
    $this->setUpMigateSourceFields($query);
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('log'));
    $this->addFieldMapping('revision_changed', 'timestamp');
    $this->addFieldMapping('revision_uid', 'revision_uid');
    $this->addFieldMapping('feedback')->defaultValue('');
    $this->addFieldMapping('feedback_format')->defaultValue($filter = filter_default_format());
    $this->addUnmigratedSources(array('nid'));
    parent::setupFieldMapping();
  }

}
