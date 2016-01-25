<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

class MatchingDetails extends BaseDetails {

  protected $source_table_name = 'quiz_matching_user_answers';
  protected $source_columns = array('match_id', 'score', 'answer');
  protected $pk_source = array(
      'result_id' => array('type' => 'int', 'not null' => TRUE, 'alias' => 'details'),
      'match_id'  => array('type' => 'int', 'not null' => TRUE, 'alias' => 'details'),
  );
  protected $dest_table_name = 'quizz_matching_answer';
  protected $column_mapping = array(
      'match_id' => 'match_id',
      'score'    => 'score',
      'answer'   => 'answer',
  );

  public function setupMigrateSource() {
    $query = db_select($this->source_table_name, 'details');

    $query->innerJoin('quiz_matching_node', 'question_property', 'details.match_id = question_property.match_id');
    $query->innerJoin('quiz_node_results_answers', 'answer', 'details.result_id = answer.result_id AND question_property.vid = answer.question_vid');
    $query->innerJoin('quiz_node_results', 'result', 'answer.result_id = result.result_id');
    $query->innerJoin('node_revision', 'r', 'question_property.vid = r.vid');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->condition('n.type', $this->bundle);

    $query->fields('answer', $this->source_base_columns);
    $query->fields('details', $this->source_columns);
    $query->addExpression(0, 'answer_id');
    $query->addField('n', 'type', 'question_type');

    return $this->doSetupMigrationSource($query);
  }

  public function prepare($entity, $row) {
    parent::prepare($entity, $row);

    $sql = 'SELECT destid1 FROM {migrate_map_quiz_question_extra_matching} WHERE sourceid1 = :id';
    if (!$entity->match_id = db_query($sql, array(':id' => $row->match_id))->fetchColumn()) {
      throw new RuntimeException('Can not find match_id. Source: ' . var_export($row));
    }
  }

}
