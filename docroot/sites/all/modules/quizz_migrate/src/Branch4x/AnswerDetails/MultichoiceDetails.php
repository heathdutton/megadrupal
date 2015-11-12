<?php

namespace Drupal\quizz_migrate\Branch4x\AnswerDetails;

use MigrateSourceSQL;
use SelectQuery;

class MultichoiceDetails extends BaseDetails {

  protected $source_table_name = 'quiz_multichoice_user_answers';
  protected $source_columns = array('id', 'choice_order');
  protected $dest_table_name = 'quizz_multichoice_answer';
  protected $column_mapping = array(
      'choice_order' => 'choice_order',
      'user_answer'  => 'user_answer',
  );

  public function setupMigrateFieldMapping() {
    parent::setupMigrateFieldMapping();
    $this->migration->addUnmigratedSources(array('id'));
  }

  protected function doSetupMigrationSource(SelectQuery $query) {
    $query->addExpression(0, 'user_answer');
    return new MigrateSourceSQL($query);
  }

  public function prepare($entity, $row) {
    parent::prepare($entity, $row);

    $entity->choice_order = array();
    $entity->user_answer = array();

    // Prepare choice_order
    if (!empty($row->choice_order)) {
      $entity->choice_order = explode(',', $row->choice_order);
    }

    // Prepare user_answer
    $sql = 'SELECT answer_id FROM {quiz_multichoice_user_answer_multi} WHERE user_answer_id = :id';
    $entity->user_answer = db_query($sql, array(':id' => $row->id))->fetchCol();

    // Convert to string before persisted to db.
    $entity->choice_order = serialize($entity->choice_order);
    $entity->user_answer = serialize($entity->user_answer);
  }

}
