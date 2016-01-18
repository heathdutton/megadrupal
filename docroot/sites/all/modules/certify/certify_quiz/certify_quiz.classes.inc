<?php

/**
 * @file 
 * certify_quiz.classes.inc php file
 * Classes for the Drupal module Certify Quiz.
 */

class CertifyQuiz extends CertifyCondition {

  public $score; // Score object from Quiz module


  /**
   * Return form elements for node form
   */
  public static function certify_form($node) {
    $optionsq = quiz_get_all_titles(); // !TODO organic groups filtering

    $form['selected_quizzes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Quizzes to be part of this certificate'),
      '#options' => $optionsq,
      '#default_value' => Certify::getConditionsNidsFromNode($node, 'quiz'),
    );
    return $form;
  }

  /**
   * Process node form submit
   */
  public static function certify_insert($node) {
    foreach ($node->selected_quizzes as $quizkey => $quiz) {
      if ($quizkey == $quiz) {
        $quiznode = node_load($quiz);
        
        $id = db_insert('certify_conditions')
        ->fields(array(
          'cid' => $node->nid,
          'vid' => $node->vid,
          'condtype' => 'quiz',
          'condnode' => $quiz
        ))->execute();
      }
    }
  }

  /**
   * Render condition for view
   */
  public function certify_view($node) {
    global $user;
    $this->fillProgress($user->uid);
    return theme('certify_quiz_view', array('node' => $node, 'condition' => $this));
  }

  /**
   * Header string for output
   */
  public static function getViewHeader() {
    return t("Quizzes");
  }

  /**
   * Calculate progress data for this condition
   */
  public function fillProgress($uid = 0) {
    // Fetch quiz result
    $quizscores = quiz_get_score_data(array((int)$this->condnode), $uid);
    if (!empty($quizscores)) {
      $this->score = array_shift($quizscores);
      
      // Attempt to circumvent http://drupal.org/node/1395134
      if (!$this->score->max_score) {
        $this->score->max_score = 100;
      }

      $this->gotpoints = $this->score->percent_score * $this->score->max_score / 100;
      $this->requiredpoints = $this->score->percent_pass * $this->score->max_score / 100;
      $this->gotpointscapped = $this->gotpoints > $this->requiredpoints ? $this->requiredpoints : $this->gotpoints;
      $this->totalpoints = $this->score->max_score;
      
      $this->started = $this->score->percent_score == TRUE;
      $this->completed = $this->score->percent_score >= $this->score->percent_pass;
    }
    else { 
      $this->started = $this->completed = FALSE;
      $this->gotpoints = $this->gotpointscapped = $this->totalpoints = $this->requiredpoints = 0;
    }
  }
}
