<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails\Multichoice;

use MigrateDestinationTable;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;
use RuntimeException;

class MultichoiceQuestionExtraMigration extends Migration {

  protected $source_table = 'quiz_multichoice_answers';
  protected $dest_table = 'quizz_multichoice_alternative';

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();

    $pk_dest = \MigrateDestinationTable::getKeySchema($this->dest_table);
    $pk_source = array(
        'id' => array('alias' => 'question_extra') + $pk_dest['id'],
    );

    $this->map = new MigrateSQLMap($this->machineName, $pk_source, $pk_dest);
    $this->setupFieldMapping();
  }

  protected function setupMigrateDestination() {
    return new MigrateDestinationTable($this->dest_table);
  }

  protected function setupMigrateSource() {
    $query = db_select($this->source_table, 'question_extra');
    $query->innerJoin('node_revision', 'r', 'question_extra.question_vid = r.vid');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->addField('n', 'type', 'question_type');
    $query
      ->fields('question_extra', array('id', 'answer', 'answer_format', 'feedback_if_chosen', 'feedback_if_chosen_format', 'feedback_if_not_chosen', 'feedback_if_not_chosen_format', 'score_if_chosen', 'score_if_not_chosen', 'question_nid', 'question_vid'))
      ->orderBy('r.vid')
    ;
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('answer', 'answer_format', 'feedback_if_chosen', 'feedback_if_chosen_format', 'feedback_if_not_chosen', 'feedback_if_not_chosen_format', 'score_if_chosen', 'score_if_not_chosen', 'question_vid'));
    $this->addFieldMapping('question_qid', 'question_nid');
    $this->addFieldMapping('weight')->defaultValue(0);
    $this->addUnmigratedSources(array('question_type', 'id'));
    $this->addUnmigratedDestinations(array('id'));
  }

  public function prepare($question_extra, $row) {
    $map = array(
        'question_qid' => 'SELECT destid1 FROM {migrate_map_quiz_question__' . $row->question_type . '} WHERE sourceid1 = :id',
        'question_vid' => 'SELECT destid1 FROM {migrate_map_quiz_question_revision__' . $row->question_type . '} WHERE sourceid1 = :id',
    );

    foreach ($map as $k => $sql) {
      if (!$question_extra->{$k} = db_query($sql, array(':id' => $question_extra->{$k}))->fetchColumn()) {
        throw new RuntimeException($k . ' not found. Source: ' . var_export($row));
      }
    }
  }

}
