<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

class LongTextDetails extends BaseDetails {

  protected $source_table_name = 'quiz_long_answer_node_properties';
  protected $source_columns = array('nid', 'vid', 'rubric');
  protected $dest_table_name = 'quizz_long_question';
  protected $column_mapping = array(
      'nid'    => 'qid',
      'vid'    => 'vid',
      'rubric' => 'rubric'
  );

}
