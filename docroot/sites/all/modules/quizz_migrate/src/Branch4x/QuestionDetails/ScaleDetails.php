<?php

namespace Drupal\quizz_migrate\Branch4x\QuestionDetails;

use RuntimeException;

class ScaleDetails extends BaseDetails {

  protected $source_table_name = 'quiz_scale_node_properties';
  protected $source_columns = array('nid', 'vid', 'answer_collection_id');
  protected $dest_table_name = 'quiz_scale_question';
  protected $column_mapping = array(
      'nid'                  => 'qid',
      'vid'                  => 'vid',
      'answer_collection_id' => 'collection_id'
  );

  public function prepare($item, $row) {
    parent::prepare($item, $row);

    $sql = 'SELECT destid1 FROM {migrate_map_quiz_scale_collection} WHERE sourceid1 = :id';
    $item->collection_id = db_query($sql, array(':id' => $row->answer_collection_id))->fetchColumn();
    if ($item->collection_id) {
      throw new RuntimeException('Can not find item.collection_id');
    }
  }

}
