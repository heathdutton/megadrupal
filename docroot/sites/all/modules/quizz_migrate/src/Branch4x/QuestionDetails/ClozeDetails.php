<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

class ClozeDetails extends BaseDetails {

  protected $source_table_name = 'quiz_cloze_node_properties';
  protected $source_columns = array('nid', 'vid', 'learning_mode');
  protected $dest_table_name = 'quiz_cloze_question';
  protected $column_mapping = array(
      'nid'           => 'qid',
      'vid'           => 'vid',
      'learning_mode' => 'learning_mode'
  );

}
