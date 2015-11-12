<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails\Matching;

use MigrateDestinationTable;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;
use RuntimeException;

class MatchingQuestionExtraMigration extends Migration {

  protected $source_table = 'quiz_matching_node';
  protected $dest_table = 'quiz_matching_question';

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();

    $pk_dest = \MigrateDestinationTable::getKeySchema($this->dest_table);
    $pk_source = array(
        'match_id' => array('alias' => 'question_extra') + $pk_dest['match_id'],
    );

    $this->map = new MigrateSQLMap($this->machineName, $pk_source, $pk_dest);
    $this->setupFieldMapping();
  }

  protected function setupMigrateDestination() {
    return new MigrateDestinationTable($this->dest_table);
  }

  protected function setupMigrateSource() {
    $query = db_select($this->source_table, 'question_extra');
    $query->innerJoin('node_revision', 'r', 'question_extra.vid = r.vid');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->addField('n', 'type', 'question_type');
    $query
      ->fields('question_extra', array('match_id', 'nid', 'vid', 'question', 'answer', 'feedback'))
      ->orderBy('r.vid')
    ;
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('vid', 'question', 'answer', 'feedback'));
    $this->addFieldMapping('qid', 'nid');
  }

  public function prepare($question_extra, $row) {
    $map = array(
        'qid' => 'SELECT destid1 FROM {migrate_map_quiz_question__' . $row->question_type . '} WHERE sourceid1 = :id',
        'vid' => 'SELECT destid1 FROM {migrate_map_quiz_question_revision__' . $row->question_type . '} WHERE sourceid1 = :id',
    );

    foreach ($map as $k => $sql) {
      if (!$question_extra->{$k} = db_query($sql, array(':id' => $question_extra->{$k}))->fetchColumn()) {
        throw new RuntimeException($k . ' not found. Source: ' . var_export($row));
      }
    }
  }

}
