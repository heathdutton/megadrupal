<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails\Scale;

use MigrateDestinationTable;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;

class ScaleCollectionMigration extends Migration {

  protected $source_table = 'quiz_scale_answer_collection';
  protected $dest_table = 'quiz_scale_collection';

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();

    $pk_dest = MigrateDestinationTable::getKeySchema($this->dest_table);
    $pk_source = array(
        'id' => array('alias' => 'collection') + $pk_dest['id'],
    );

    $this->map = new MigrateSQLMap($this->machineName, $pk_source, $pk_dest);
    $this->setupFieldMapping();
  }

  protected function setupMigrateDestination() {
    return new MigrateDestinationTable($this->dest_table);
  }

  protected function setupMigrateSource() {
    $query = db_select($this->source_table, 'collection');
    $query->fields('collection', array('id', 'for_all'));
    $query->orderBy('collection.id');
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addFieldMapping('for_all', 'for_all');
    $this->addFieldMapping('question_type')->defaultValue('scale');
    $this->addFieldMapping('name', 'id');
  }

}
