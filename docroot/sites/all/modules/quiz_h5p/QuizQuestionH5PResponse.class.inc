<?php

class QuizQuestionH5PResponse extends QuizQuestionResponse {

  public function delete() {

  }

  public function getResponse() {
    return 'In H5P';
  }

  public function save() {
    global $user;

    $percentScore = 0;
    if ($this->answer !== 1 && is_numeric($this->answer)) {
      // Answer is the "encrypted" percentScore. Let's "decrypt" it...
      $percentScore = $this->answer / 1.234 - 32.17;
    }
    if ($percentScore < 0) {
      $percentScore = 0;
    }
    elseif ($percentScore > 1) {
      $percentScore = 0;
    }

    db_merge('quiz_h5p_user_results')
      ->key(array(
        'question_nid' => $this->question->nid,
        'question_vid' => $this->question->vid,
        'result_id' => $this->result_id,
      ))
      ->fields(array(
        'question_nid' => $this->question->nid,
        'question_vid' => $this->question->vid,
        'result_id' => $this->result_id,
        'percent_score' => $percentScore,
      ))
      ->execute();
  }

  public function score() {
    global $user;

    $percentScore = db_query(
      "SELECT percent_score
      FROM {quiz_h5p_user_results}
      WHERE question_nid = :question_nid
      AND question_vid = :question_vid
      AND result_id = :result_id", array(
      'question_nid' => $this->question->nid,
      'question_vid' => $this->question->vid,
      'result_id' => $this->result_id
      ))->fetchField();

    return round($percentScore * $this->getMaxScore());
  }

  /**
   * Feedback is handled in the H5P type.
   */
  public function getFeedbackValues() {
    return array();
  }

}
