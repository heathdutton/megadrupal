<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

class DdlinesDetails extends BaseDetails {

  protected $source_table_name = 'quiz_ddlines_user_answer_multi';
  protected $source_columns = array('hotspot_id', 'label_id');
  protected $dest_table_name = 'quiz_ddlines_answer';
  protected $column_mapping = array(
      'hotspot_id' => 'hotspot_id',
      'label_id'   => 'label_id',
  );

  public function setupMigrateSource() {
    $query = db_select($this->source_table_name, 'details');

    $query->innerJoin('quiz_ddlines_user_answers', 'extra_base', 'details.user_answer_id = extra_base.id');
    $query->innerJoin('quiz_node_results_answers', 'answer', 'extra_base.result_id = answer.result_id AND extra_base.question_vid = answer.question_vid');
    $query->innerJoin('quiz_node_results', 'result', 'answer.result_id = result.result_id');
    $query->innerJoin('node_revision', 'r', 'extra_base.question_vid = r.vid');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->condition('n.type', $this->bundle);

    $query->fields('answer', $this->source_base_columns);
    $query->fields('details', $this->source_columns);
    $query->addExpression(0, 'answer_id');
    $query->addExpression(0, 'user_answer');
    $query->addField('n', 'type', 'question_type');

    return $this->doSetupMigrationSource($query);
  }

}
