<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz\Entity\QuizEntity;
use Drupal\quizz_migrate\Destination\QuizRevisionDestination;
use MigrateSourceSQL;

class QuizRevisionMigration extends QuizMigration {

  protected $description = 'Migrate quiz node to entity.';
  protected $machineName = 'quiz_revision';
  protected $sourcePK = array(
      'vid' => array(
          'type'        => 'int',
          'not null'    => TRUE,
          'description' => 'Node',
          'alias'       => 'r',
      )
  );

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->destination = new QuizRevisionDestination();
  }

  protected function setupMigrateSource() {
    $query = db_select('node_revision', 'r');
    $query->innerJoin('node', 'n', 'r.nid = n.nid');
    $query->innerJoin('quiz_node_properties', 'p', 'r.vid = p.vid');
    $query->addField('r', 'uid', 'revision_uid');
    $query->addField('n', 'vid', 'active_vid');
    $query
      ->fields('r', array('nid', 'vid', 'log', 'timestamp'))
      ->fields('n', array('uid', 'language', 'title', 'created', 'status', 'changed'))
      ->fields('p', array('aid', 'number_of_random_questions', 'max_score_for_random', 'pass_rate', 'summary_pass', 'summary_pass_format', 'summary_default', 'summary_default_format', 'randomization', 'backwards_navigation', 'keep_results', 'repeat_until_correct', 'feedback_time', 'display_feedback', 'quiz_open', 'quiz_close', 'takes', 'show_attempt_stats', 'time_limit', 'quiz_always', 'tid', 'has_userpoints', 'userpoints_tid', 'time_left', 'max_score', 'allow_skipping', 'allow_resume', 'allow_jumping', 'show_passed', 'mark_doubtful'))
      ->orderBy('r.vid', 'ASC')
      ->addExpression('1', 'review_options')
    ;
    $this->setUpMigateSourceFields($query);
    return new MigrateSourceSQL($query);
  }

  protected function setupFieldMapping() {
    parent::setupFieldMapping();
    $this->addSimpleMappings(array('revision_uid', 'aid', 'log', 'tid'));
    $this->addSimpleMappings(array('pass_rate', 'summary_pass', 'summary_pass_format', 'summary_default', 'summary_default_format'));
    $this->addSimpleMappings(array('quiz_open', 'quiz_close', 'time_limit', 'takes', 'quiz_always'));
    $this->addSimpleMappings(array('allow_skipping', 'allow_resume', 'allow_jumping'));
    $this->addFieldMapping('number_of_random_questions', 'number_of_random_questions');
    $this->addFieldMapping('max_score_for_random', 'max_score_for_random');
    $this->addFieldMapping('randomization', 'randomization');
    $this->addFieldMapping('backwards_navigation', 'backwards_navigation');
    $this->addFieldMapping('keep_results', 'keep_results');
    $this->addFieldMapping('repeat_until_correct', 'repeat_until_correct');
    $this->addFieldMapping('show_attempt_stats', 'show_attempt_stats');
    $this->addFieldMapping('has_userpoints', 'has_userpoints');
    $this->addFieldMapping('userpoints_tid', 'userpoints_tid');
    $this->addFieldMapping('time_left', 'time_left');
    $this->addFieldMapping('max_score', 'max_score');
    $this->addFieldMapping('show_passed', 'show_passed');
    $this->addFieldMapping('mark_doubtful', 'mark_doubtful');
    $this->addFieldMapping('allow_change')->defaultValue(1);
    $this->addFieldMapping('build_on_last')->defaultValue('');
    $this->addFieldMapping('review_options', 'review_options')->defaultValue('');
    $this->addUnmigratedSources(array('status', 'feedback_time', 'display_feedback', 'timestamp'));
    $this->addUnmigratedDestinations(array('active_vid'));
  }

  public function prepareRow($row) {
    $row->review_options = $this->prepareRowReviewOptions($row);
    return parent::prepareRow($row);
  }

  /**
   * @see quiz_update_7505().
   */
  private function prepareRowReviewOptions($row) {
    $options = array();

    if ($row->feedback_time == 0) {
      $options['end']['answer_feedback'] = 'answer_feedback';
      if ($row->display_feedback) {
        $options['end']['solution'] = 'solution';
      }
    }

    if ($row->feedback_time == 1) {
      $options['question']['answer_feedback'] = 'answer_feedback';
      if ($row->display_feedback) {
        $options['question']['solution'] = 'solution';
      }
    }

    return $options;
  }

  /**
   * @param QuizEntity $quiz
   */
  public function prepare($quiz, $source_row) {
    $quiz->revision_changed = $source_row->timestamp;
    $quiz->is_new = FALSE;
    $quiz->is_new_revision = TRUE;
    $quiz->qid = db_query('SELECT destid1 FROM {migrate_map_quiz} WHERE sourceid1 = :nid', array(':nid' => $source_row->nid))->fetchColumn();

    // Fix quiz's active revision
    if ($source_row->active_vid == $source_row->vid) {
      $quiz->is_new_revision = FALSE;
      $quiz->vid = db_query('SELECT vid FROM {quiz_entity} WHERE qid = :qid', array(':qid' => $quiz->qid))->fetchColumn();
    }
  }

}
