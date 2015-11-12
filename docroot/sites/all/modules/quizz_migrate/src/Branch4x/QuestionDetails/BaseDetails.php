<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

use Drupal\quizz_migrate\Branch4x\QuestionDetailMigration;
use Drupal\quizz_migrate\Destination\QuestionDetailsDestination;
use MigrateSourceSQL;
use MigrateSQLMap;

abstract class BaseDetails implements DetailsInterface {

  protected $bundle;

  /** @var QuestionDetailMigration */
  protected $migration;

  /** @var array[] */
  protected $pk_source = array(
      'nid' => array('type' => 'int', 'not null' => TRUE, 'alias' => 'details'),
      'vid' => array('type' => 'int', 'not null' => TRUE, 'alias' => 'details')
  );

  /** @var array[] */
  protected $pk_dest = array(
      'vid' => array('type' => 'int', 'not null' => TRUE)
  );

  public function __construct($bundle, QuestionDetailMigration $migration) {
    $this->bundle = $bundle;
    $this->migration = $migration;
  }

  public function setupMigrateSource() {
    $query = db_select($this->source_table_name, 'details');
    $query->innerJoin('node_revision', 'r', 'details.vid = r.vid');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->addField('n', 'type', 'question_type');
    $query->fields('details', $this->source_columns);
    $query->condition('n.type', $this->bundle);
    return new MigrateSourceSQL($query);
  }

  public function setupMigrateDestination() {
    return new QuestionDetailsDestination($this->dest_table_name);
  }

  public function setupMigrateFieldMapping() {
    $m = $this->migration;
    foreach ($this->column_mapping as $source_column => $destination_column) {
      $m->addFieldMapping($destination_column, $source_column);
    }
    $m->addUnmigratedSources(array('question_type'));
  }

  public function setupMigrateMap() {
    return new MigrateSQLMap($this->migration->getMachineName(), $this->pk_source, $this->pk_dest);
  }

  public function prepare($entity, $row) {
    $map = array(
        'qid' => 'SELECT destid1 FROM {migrate_map_quiz_question__' . $row->question_type . '} WHERE sourceid1 = :id',
        'vid' => 'SELECT destid1 FROM {migrate_map_quiz_question_revision__' . $row->question_type . '} WHERE sourceid1 = :id',
    );

    foreach ($map as $k => $sql) {
      if (!$entity->{$k} = db_query($sql, array(':id' => $entity->{$k}))->fetchColumn()) {
        throw new RuntimeException($k . ' not found. Source: ' . var_export($row));
      }
    }
  }

  /**
   * {@inhertidoc}
   */
  public function complete($entity, $row) {
    
  }

}
