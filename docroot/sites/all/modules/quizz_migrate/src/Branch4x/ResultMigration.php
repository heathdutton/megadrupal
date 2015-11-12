<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\ResultDestination;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;

class ResultMigration extends Migration {

  protected $sourcePK = array(
      'result_id' => array('type' => 'int', 'not null' => TRUE, 'alias' => 'result'),
  );

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();
    $this->map = new MigrateSQLMap($this->machineName, $this->sourcePK, ResultDestination::getKeySchema());
    $this->setupFieldMapping();
  }

  protected function setupMigrateSource() {
    $query = db_select('quiz_node_results', 'result');
    $query->innerJoin('node_revision', 'r', 'result.vid = r.vid'); // Do not migrate broken revisions
    $query
      ->fields('result', array('result_id', 'nid', 'vid', 'uid', 'time_start', 'time_end', 'released', 'score', 'is_invalid', 'is_evaluated', 'time_left', 'archived'))
      ->orderBy('result.vid')
    ;
    return new MigrateSourceSQL($query);
  }

  protected function setupMigrateDestination() {
    return new ResultDestination();
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('uid', 'released', 'score'));
    $this->addSimpleMappings(array('is_invalid', 'is_evaluated'));
    $this->addSimpleMappings(array('time_start', 'time_end', 'time_left'));
    $this->addFieldMapping('quiz_qid', 'nid');
    $this->addFieldMapping('quiz_vid', 'vid');
    $this->addFieldMapping('type')->defaultValue('quiz');
    $this->addUnmigratedSources(array('archived'));
  }

}
