<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\AnswerDestination;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;
use RuntimeException;

class AnswerMigration extends Migration {

  protected $sourcePK = array(
      'result_id'    => array('type' => 'int', 'not null' => TRUE, 'alias' => 'answer'),
      'question_vid' => array('type' => 'int', 'not null' => TRUE, 'alias' => 'answer'),
  );

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();
    $this->map = new MigrateSQLMap($this->machineName, $this->sourcePK, AnswerDestination::getKeySchema());
    $this->setupFieldMapping();
  }

  protected function setupMigrateSource() {
    $query = db_select('quiz_node_results_answers', 'answer');
    $query->innerJoin('quiz_node_results', 'result', 'answer.result_id = result.result_id'); // Do not migrate broken revisions
    $query->innerJoin('node_revision', 'r', 'answer.question_vid = r.vid');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->addField('n', 'type', 'question_type');
    $query
      ->fields('answer', array('result_id', 'question_nid', 'question_vid', 'number'))
      ->fields('answer', array('is_correct', 'is_skipped', 'answer_timestamp'))
      ->fields('answer', array('tid', 'points_awarded', 'is_doubtful'))
      ->condition('n.type', array_keys(quizz_question_get_handler_info()))
      ->orderBy('r.vid')
    ;
    return new MigrateSourceSQL($query);
  }

  protected function setupMigrateDestination() {
    return new AnswerDestination();
  }

  protected function setupFieldMapping() {
    $this->addSimpleMappings(array('result_id', 'number', 'tid'));
    $this->addSimpleMappings(array('is_correct', 'is_skipped'));
    $this->addSimpleMappings(array('points_awarded', 'answer_timestamp', 'is_doubtful'));
    $this->addFieldMapping('question_qid', 'question_nid');
    $this->addFieldMapping('question_vid', 'question_vid');
    $this->addFieldMapping('type', 'question_type');
  }

  public function prepare($answer, $row) {
    $answer->type = $row->question_type;
    $map = array(
        'result_id'    => 'SELECT destid1 FROM {migrate_map_quiz_result} WHERE sourceid1 = :id',
        'question_qid' => 'SELECT destid1 FROM {migrate_map_quiz_question__' . $row->question_type . '} WHERE sourceid1 = :id',
        'question_vid' => 'SELECT destid1 FROM {migrate_map_quiz_question_revision__' . $row->question_type . '} WHERE sourceid1 = :id',
    );

    foreach ($map as $k => $sql) {
      if (!$answer->{$k} = db_query($sql, array(':id' => $answer->{$k}))->fetchColumn()) {
        throw new RuntimeException($k . ' not found. Source: ' . var_export($row));
      }
    }
  }

}
