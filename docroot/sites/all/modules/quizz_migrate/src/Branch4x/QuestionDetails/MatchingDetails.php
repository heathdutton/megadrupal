<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

class MatchingDetails extends BaseDetails {

  protected $source_table_name = 'quiz_matching_properties';
  protected $source_columns = array('nid', 'vid', 'choice_penalty');
  protected $dest_table_name = 'quiz_matching_question_settings';
  protected $column_mapping = array(
      'nid'            => 'qid',
      'vid'            => 'vid',
      'choice_penalty' => 'choice_penalty'
  );

}
