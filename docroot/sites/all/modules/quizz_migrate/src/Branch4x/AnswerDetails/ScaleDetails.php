<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

class ScaleDetails extends BaseDetails {

  protected $source_table_name = 'quiz_scale_user_answers';
  protected $source_columns = array('answer_id');
  protected $dest_table_name = 'quizz_scale_answer';
  protected $column_mapping = array(
      'answer_id' => 'collection_item_id',
  );

  public function setupMigrateFieldMapping() {
    parent::setupMigrateFieldMapping();
    $this->migration->addFieldMapping('answer_id')->defaultValue(0);
  }

  public function prepare($entity, $row) {
    parent::prepare($entity, $row);

    $sql = 'SELECT destid1 FROM {migrate_map_quiz_scale_collection_item} WHERE sourceid1 = :id';
    if (!$entity->collection_item_id = db_query($sql, array(':id' => $row->answer_id))->fetchColumn()) {
      throw new RuntimeException('Can not find collection_item_id. Source: ' . var_export($row));
    }
  }

}
