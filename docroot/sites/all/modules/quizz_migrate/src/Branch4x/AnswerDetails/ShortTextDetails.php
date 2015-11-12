<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

class ShortTextDetails extends BaseDetails {

  protected $source_table_name = 'quiz_short_answer_user_answers';
  protected $source_columns = array('score', 'answer', 'is_evaluated', 'answer_feedback', 'answer_feedback_format');
  protected $dest_table_name = 'quizz_short_answer';
  protected $column_mapping = array(
      'score'                  => 'score',
      'answer'                 => 'answer',
      'is_evaluated'           => 'is_evaluated',
      'answer_feedback'        => 'answer_feedback',
      'answer_feedback_format' => 'answer_feedback_format'
  );

  public function setupMigrateFieldMapping() {
    parent::setupMigrateFieldMapping();

    $this->migration->removeFieldMapping('answer_feedback_format');
    $this->migration->addFieldMapping('answer_feedback_format', 'answer_feedback_format')->defaultValue($filter = filter_default_format());
  }

}
