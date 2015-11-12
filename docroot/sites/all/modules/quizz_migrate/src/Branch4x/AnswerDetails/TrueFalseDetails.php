<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

class TrueFalseDetails extends BaseDetails {

  protected $source_table_name = 'quiz_truefalse_user_answers';
  protected $source_columns = array('score', 'answer');
  protected $dest_table_name = 'quiz_truefalse_answer';
  protected $column_mapping = array(
      'score'  => 'score',
      'answer' => 'answer',
  );

}
