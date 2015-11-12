<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails\Pool;

use MigrateDestinationTable;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;
use RuntimeException;

class QuestionReferenceMigration extends Migration {

  protected $source_table = 'field_data_field_question_reference';
  protected $dest_table = 'field_data_field_question_reference';

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();

    $pk_dest = \MigrateDestinationTable::getKeySchema($this->dest_table);
    $pk_source = array(
        'revision_id' => array('alias' => 'field_data') + $pk_dest['entity_id'],
        'delta'       => array('alias' => 'field_data') + $pk_dest['delta'],
        'language'    => array('alias' => 'field_data') + $pk_dest['language'],
    );

    $this->map = new MigrateSQLMap($this->machineName, $pk_source, $pk_dest);
    $this->setupFieldMapping();
  }

  protected function setupMigrateDestination() {
    return new MigrateDestinationTable($this->dest_table);
  }

  protected function setupMigrateSource() {
    $query = db_select($this->source_table, 'field_data');
    $query->innerJoin('node', 'pool_node', 'field_data.entity_id = pool_node.nid');
    $query->innerJoin('node', 'question_node', 'field_data.field_question_reference_target_id = question_node.nid');
    $query->addField('question_node', 'type', 'question_type');
    $query
      ->fields('field_data', array('entity_id', 'revision_id', 'language', 'delta', 'field_question_reference_target_id'))
      ->condition('field_data.entity_type', 'node')
      ->condition('field_data.bundle', 'pool')
      ->condition('field_data.deleted', 0)
      ->orderBy('pool_node.nid')
    ;
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('entity_id', 'revision_id', 'language', 'delta', 'field_question_reference_target_id'));
    $this->addFieldMapping('entity_type')->defaultValue('quiz_question_entity');
    $this->addFieldMapping('bundle')->defaultValue('pool');
    $this->addFieldMapping('deleted')->defaultValue(0);
  }

  public function prepare($field_item, $row) {
    $map = array(
        'entity_id'                          => 'SELECT destid1 FROM {migrate_map_quiz_question__pool} WHERE sourceid1 = :id',
        'revision_id'                        => 'SELECT destid1 FROM {migrate_map_quiz_question_revision__pool} WHERE sourceid1 = :id',
        'field_question_reference_target_id' => 'SELECT destid1 FROM {migrate_map_quiz_question__' . $row->question_type . '} WHERE sourceid1 = :id',
    );

    foreach ($map as $k => $sql) {
      if (!$field_item->{$k} = db_query($sql, array(':id' => $field_item->{$k}))->fetchColumn()) {
        throw new RuntimeException($k . ' not found. Source: ' . var_export($row));
      }
    }
  }

}
