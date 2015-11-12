<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

class ClozeDetails extends BaseDetails {

  protected $source_table_name = 'quiz_cloze_user_answers';
  protected $source_columns = array('score', 'answer');
  protected $dest_table_name = 'quiz_cloze_answer';
  protected $column_mapping = array(
      'score'  => 'score',
      'answer' => 'answer',
  );

}
