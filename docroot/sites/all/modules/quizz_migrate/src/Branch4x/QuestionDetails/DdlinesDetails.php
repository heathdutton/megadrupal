<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

class DdlinesDetails extends BaseDetails {

  protected $source_table_name = 'quiz_ddlines_node';
  protected $source_columns = array('nid', 'vid', 'feedback_enabled', 'hotspot_radius', 'ddlines_elements', 'execution_mode');
  protected $dest_table_name = 'quiz_ddlines_question';
  protected $column_mapping = array(
      'nid'              => 'qid',
      'vid'              => 'vid',
      'feedback_enabled' => 'feedback_enabled',
      'hotspot_radius'   => 'hotspot_radius',
      'ddlines_elements' => 'ddlines_elements',
      'execution_mode'   => 'execution_mode',
  );

}
