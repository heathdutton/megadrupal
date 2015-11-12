<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails\Scale;

use MigrateDestinationTable;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;
use RuntimeException;

class ScaleCollectionItemMigration extends Migration {

  protected $source_table = 'quiz_scale_answer';
  protected $dest_table = 'quiz_scale_collection_item';

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();

    $pk_dest = MigrateDestinationTable::getKeySchema($this->dest_table);
    $pk_source = array(
        'id' => array('alias' => 'item') + $pk_dest['id'],
    );

    $this->map = new MigrateSQLMap($this->machineName, $pk_source, $pk_dest);
    $this->setupFieldMapping();
  }

  protected function setupMigrateDestination() {
    return new MigrateDestinationTable($this->dest_table);
  }

  protected function setupMigrateSource() {
    $query = db_select($this->source_table, 'item')
      ->fields('item', array('id', 'answer_collection_id', 'answer'))
      ->orderBy('item.id');
    $query->innerJoin('quiz_scale_answer_collection', 'collection', 'item.answer_collection_id = collection.id');
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addFieldMapping('collection_id', 'answer_collection_id');
    $this->addFieldMapping('answer', 'answer');
    $this->addUnmigratedDestinations(array('id'));
  }

  public function prepare($item, $row) {
    $sql = 'SELECT destid1 FROM {migrate_map_quiz_scale_collection} WHERE sourceid1 = :id';
    $item->collection_id = db_query($sql, array(':id' => $row->answer_collection_id))->fetchColumn();
    if (!$item->collection_id) {
      throw new RuntimeException('Can not find item.collection_id. Source: ' . var_export($row));
    }
  }

}
