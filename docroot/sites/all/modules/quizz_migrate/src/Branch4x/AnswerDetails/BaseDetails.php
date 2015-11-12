<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

use Drupal\quizz_migrate\Branch4x\AnswerDetailMigration;
use Drupal\quizz_migrate\Branch4x\QuestionDetails\DetailsInterface;
use Drupal\quizz_migrate\Destination\AnswerDetailsDestination;
use MigrateSourceSQL;
use MigrateSQLMap;
use RuntimeException;
use SelectQuery;

abstract class BaseDetails implements DetailsInterface {

  /** @var AnswerDetailMigration */
  protected $migration;

  /** @var string */
  protected $bundle;

  /** @var string */
  protected $source_table_name;

  /** @var string[] */
  protected $source_base_columns = array('result_id', 'question_nid', 'question_vid');

  /** @var string[] */
  protected $source_columns = array();

  /** @var string */
  protected $dest_table_name;

  /** @var string[] */
  protected $column_mapping = array();

  /** @var array[] */
  protected $pk_source = array(
      'result_id'    => array('type' => 'int', 'not null' => TRUE, 'alias' => 'details'),
      'question_vid' => array('type' => 'int', 'not null' => TRUE, 'alias' => 'details')
  );

  /** @var array[] */
  protected $pk_dest = array(
      'answer_id' => array('type' => 'int', 'not null' => TRUE)
  );

  public function __construct($bundle, AnswerDetailMigration $migration) {
    $this->bundle = $bundle;
    $this->migration = $migration;
  }

  public function setupMigrateSource() {
    $query = db_select($this->source_table_name, 'details');

    $query->innerJoin('quiz_node_results_answers', 'answer', 'details.result_id = answer.result_id AND details.question_vid = answer.question_vid');
    $query->innerJoin('quiz_node_results', 'result', 'answer.result_id = result.result_id');
    $query->innerJoin('node_revision', 'r', 'details.question_vid = r.vid');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->condition('n.type', $this->bundle);

    $query->fields('answer', $this->source_base_columns);
    $query->fields('details', $this->source_columns);
    $query->addExpression(0, 'answer_id');
    $query->addField('n', 'type', 'question_type');

    return $this->doSetupMigrationSource($query);
  }

  /**
   * Sub classes can override this one.
   * @param SelectQuery $query
   * @return MigrateSourceSQL
   */
  protected function doSetupMigrationSource(SelectQuery $query) {
    return new MigrateSourceSQL($query);
  }

  public function setupMigrateDestination() {
    return new AnswerDetailsDestination($this->dest_table_name);
  }

  public function setupMigrateFieldMapping() {
    foreach ($this->column_mapping as $source_column => $destination_column) {
      $this->migration->addFieldMapping($destination_column, $source_column);
    }

    if (!isset($this->column_mapping['answer_id'])) {
      $this->migration->addFieldMapping('answer_id', 'answer_id');
    }

    $this->migration->addUnmigratedSources(array('question_type', 'question_nid', 'question_vid'));
  }

  public function setupMigrateMap() {
    return new MigrateSQLMap($this->migration->getMachineName(), $this->pk_source, $this->pk_dest);
  }

  public function prepare($entity, $row) {
    $result_id = db_query('SELECT destid1 FROM {migrate_map_quiz_result} WHERE sourceid1 = :id', array(':id' => $row->result_id))->fetchColumn();
    $question_vid = db_query('SELECT destid1 FROM {migrate_map_quiz_question_revision__' . $row->question_type . '} WHERE sourceid1 = :id', array(':id' => $row->question_vid))->fetchColumn();
    $entity->answer_id = db_query('SELECT id FROM {quiz_answer_entity} WHERE result_id = :result_id AND question_vid = :question_vid', array(
        ':result_id'    => $result_id,
        ':question_vid' => $question_vid,
      ))->fetchColumn();

    if (!$entity->answer_id) {
      throw new RuntimeException('Can not find answer_id. Source: ' . var_export($row));
    }
  }

  /**
   * {@inhertidoc}
   */
  public function complete($entity, $row) {

  }

}
