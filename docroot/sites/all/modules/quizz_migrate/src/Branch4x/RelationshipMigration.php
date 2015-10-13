<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\RelationshipDestination;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;
use RuntimeException;

class RelationshipMigration extends Migration {

  protected $sourcePK = array(
      'parent_vid' => array('type' => 'int', 'not null' => TRUE, 'alias' => 'relationship'),
      'child_vid'  => array('type' => 'int', 'not null' => TRUE, 'alias' => 'relationship'),
  );

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();
    $this->map = new MigrateSQLMap($this->machineName, $this->sourcePK, RelationshipDestination::getKeySchema());
    $this->setupFieldMapping();
  }

  protected function setupMigrateSource() {
    $query = db_select('quiz_node_relationship', 'relationship')
      ->fields('relationship', array('parent_nid', 'parent_vid', 'child_nid', 'child_vid'))
      ->fields('relationship', array('question_status', 'weight', 'max_score', 'auto_update_max_score'));

    $query->innerJoin('node', 'n', 'relationship.child_nid = n.nid');
    $query->innerJoin('node_revision', 'quiz_r', 'relationship.parent_vid = quiz_r.vid');
    $query->innerJoin('node_revision', 'question_r', 'relationship.child_vid = question_r.vid');
    $query->condition('n.type', array_keys(quizz_question_get_handler_info()));
    $query->addField('n', 'type', 'question_type');
    $query->orderBy('n.nid');

    return new MigrateSourceSQL($query);
  }

  protected function setupMigrateDestination() {
    return new RelationshipDestination();
  }

  protected function setupFieldMapping() {
    $this->addFieldMapping('quiz_qid', 'parent_nid');
    $this->addFieldMapping('quiz_vid', 'parent_vid');
    $this->addFieldMapping('qr_pid')->defaultValue(NULL);
    $this->addFieldMapping('question_qid', 'child_nid');
    $this->addFieldMapping('question_vid', 'child_vid');
    $this->addFieldMapping('question_status', 'question_status');
    $this->addFieldMapping('weight', 'weight');
    $this->addFieldMapping('max_score', 'max_score');
    $this->addFieldMapping('auto_update_max_score', 'auto_update_max_score');
    $this->addUnmigratedSources(array('question_type'));
  }

  public function prepare($quiz, $row) {
    $map = array(
        'quiz_qid'     => 'SELECT destid1 FROM {migrate_map_quiz} WHERE sourceid1 = :id',
        'quiz_vid'     => 'SELECT destid1 FROM {migrate_map_quiz_revision} WHERE sourceid1 = :id',
        'question_qid' => 'SELECT destid1 FROM {migrate_map_quiz_question__' . $row->question_type . '} WHERE sourceid1 = :id',
        'question_vid' => 'SELECT destid1 FROM {migrate_map_quiz_question_revision__' . $row->question_type . '} WHERE sourceid1 = :id',
    );

    foreach ($map as $k => $sql) {
      if (!$quiz->{$k} = db_query($sql, array(':id' => $quiz->{$k}))->fetchColumn()) {
        throw new RuntimeException($k . ' not found. Source: ' . var_export($row));
      }
    }
  }

}
