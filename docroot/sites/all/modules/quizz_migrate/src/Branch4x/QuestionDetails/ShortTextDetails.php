<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

class ShortTextDetails extends BaseDetails {

  protected $source_table_name = 'quiz_short_answer_node_properties';
  protected $source_columns = array('nid', 'vid', 'maximum_score', 'text_entry_type', 'correct_answer_evaluation', 'correct_answer', 'feedback_correct', 'feedback_incorrect');
  protected $dest_table_name = 'quizz_short_question';
  protected $column_mapping = array(
      'nid'                       => 'qid',
      'vid'                       => 'vid',
      'maximum_score'             => 'maximum_score',
      'text_entry_type'           => 'text_entry_type',
      'correct_answer_evaluation' => 'correct_answer_evaluation',
      'correct_answer'            => 'correct_answer',
      'feedback_correct'          => 'feedback_correct',
      'feedback_incorrect'        => 'feedback_incorrect',
  );

}
