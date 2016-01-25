<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

class MultichoiceDetails extends BaseDetails {

  protected $source_table_name = 'quiz_multichoice_properties';
  protected $source_columns = array('nid', 'vid', 'choice_multi', 'choice_random', 'choice_boolean');
  protected $dest_table_name = 'quizz_multichoice_question';
  protected $column_mapping = array(
      'nid'            => 'qid',
      'vid'            => 'vid',
      'choice_multi'   => 'choice_multi',
      'choice_random'  => 'choice_random',
      'choice_boolean' => 'choice_boolean',
  );

}
