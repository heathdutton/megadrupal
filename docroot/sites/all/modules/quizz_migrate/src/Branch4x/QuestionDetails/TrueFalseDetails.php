<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

class TrueFalseDetails extends BaseDetails {

  protected $source_table_name = 'quiz_truefalse_node';
  protected $source_columns = array('nid', 'vid', 'correct_answer', 'feedback');
  protected $dest_table_name = 'quiz_truefalse_question';
  protected $column_mapping = array(
      'nid'            => 'qid',
      'vid'            => 'vid',
      'correct_answer' => 'correct_answer',
      'feedback'       => 'feedback',
  );

}
