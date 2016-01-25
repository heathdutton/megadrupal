<?php

namespace Drupal\quizz_stats\Controller;

use Drupal\quizz\Entity\QuizEntity;

class RevisionListingController {

  private $quiz;

  public function __construct(QuizEntity $quiz) {
    $this->quiz = $quiz;
  }

  public function render() {
    $vids = db_query('SELECT qr.vid'
      . ' FROM {quiz_entity_revision} qr'
      . ' WHERE qr.qid = :qid'
      . ' ORDER BY qr.vid DESC', array(':qid' => $this->quiz->qid))->fetchCol();

    if (!$count = count($vids)) {
      return t('Something went wrong. Please try again');
    }

    // If there is only one revision we jump directly to that revision
    if ($count == 1) {
      drupal_goto("quiz/{$this->quiz->qid}/statistics/{$vids[0]}");
    }

    $content = array();
    $content['explanation'] = t('There are !num revisions of this quiz that have been taken.
    Different revisions may have different scoring, difficulity and other differences affecting its statistics.
    Because of this you have to choose the revision you want to see statistics from.', array('!num' => $count));
    $content['links'] = array();
    foreach ($vids as $vid) {
      $content['links'][] = 'quiz/' . $this->quiz->qid . '/statistics/' . $vid;
    }
    return theme('quizz_stats_revision_selector', array('content' => $content));
  }

}
