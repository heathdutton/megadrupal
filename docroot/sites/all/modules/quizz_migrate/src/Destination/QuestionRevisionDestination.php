<?php

namespace Drupal\quizz_migrate\Destination;

use Drupal\quizz_migrate\Destination\QuestionDestination;
use Drupal\quizz_question\Entity\Question;
use stdClass;

class QuestionRevisionDestination extends QuestionDestination {

  protected $base_table = 'quiz_question_revision';
  protected static $pk_name = 'vid';

  public function fields($migration = NULL) {
    $fields = array();

    $schema = drupal_get_schema('quiz_question_entity');
    foreach ($schema['fields'] as $name => $info) {
      $fields[$name] = isset($info['description']) ? $info['description'] : $name;
    }

    return $fields + parent::fields($migration);
  }

  /**
   * @param Question $question
   * @param stdClass $row
   */
  public function prepare($question, stdClass $row) {
    $table = "migrate_map_quiz_question__{$question->type}";
    $sql = 'SELECT destid1 FROM {' . $table . '} WHERE sourceid1 = :nid';
    $question->qid = db_query($sql, array(':nid' => $row->nid))->fetchColumn();
    $question->is_new = FALSE;
    $question->is_new_revision = TRUE;
    parent::prepare($question, $row);
  }

}
