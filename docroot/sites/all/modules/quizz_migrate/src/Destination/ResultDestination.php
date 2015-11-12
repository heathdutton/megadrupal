<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz\Entity\Result;
use Drupal\quizz_migrate\BaseEntityDestination;
use RuntimeException;

class ResultDestination extends BaseEntityDestination {

  protected $entity_type = 'quiz_result';
  protected $base_table = 'quiz_results';
  protected static $pk_name = 'result_id';
  protected $disable_import_complete = TRUE;

  /**
   * @param Result $result
   */
  public function prepare($result, $row) {
    $map = array(
        'quiz_qid' => 'SELECT destid1 FROM {migrate_map_quiz} WHERE sourceid1 = :id',
        'quiz_vid' => 'SELECT destid1 FROM {migrate_map_quiz_revision} WHERE sourceid1 = :id',
    );

    foreach ($map as $k => $sql) {
      if (!$result->{$k} = db_query($sql, array(':id' => $result->{$k}))->fetchColumn()) {
        throw new RuntimeException($k . ' not found. Source: ' . var_export($row));
      }
    }
  }

}
