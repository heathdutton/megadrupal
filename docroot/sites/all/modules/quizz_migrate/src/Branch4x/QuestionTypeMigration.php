<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\QuestionTypeDestination;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;

class QuestionTypeMigration extends Migration {

  protected $sourcePK = array(
      'type' => array(
          'type'        => 'varchar',
          'length'      => 32,
          'not null'    => TRUE,
          'default'     => '',
          'description' => 'Machine name of question node type.',
      ),
  );

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();
    $this->map = new MigrateSQLMap($this->machineName, $this->sourcePK, QuestionTypeDestination::getKeySchema());
    $this->setupFieldMapping();
  }

  protected function setupMigrateSource() {
    $query = db_select('node_type', 'nt');
    $query->condition('nt.module', 'quiz_question');
    $query->condition('nt.type', array_keys(quizz_question_get_handler_info()));
    $query->fields('nt', array('type', 'name', 'description', 'help'));
    return new MigrateSourceSQL($query);
  }

  protected function setupMigrateDestination() {
    return new QuestionTypeDestination();
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('type', 'description', 'help'));
    $this->addFieldMapping('label', 'name');
    $this->addFieldMapping('handler', 'type');
    $this->addFieldMapping('module')->defaultValue('quizz_question');
    $this->addFieldMapping('disabled')->defaultValue(0);
    $this->addFieldMapping('status')->defaultValue(0);
    $this->addFieldMapping('weight')->defaultValue(0);
    $this->addFieldMapping('data')->defaultValue(array());
    $this->addUnmigratedDestinations(array('id'));
  }

}
