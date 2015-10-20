<?php

class QuizLog {
  var $quizid;
  var $uid;
  var $quiz_type;

  /**
   * Constructor
   */
  function __construct($quizid=NULL) {
    $this->quizid = $quizid;

    global $user;
    $this->uid = $user->uid;
  }

  /**
   * Start quiz log
   */
  function start($tid, $quiz_type_name) {
    $quiz_type = $this->get_quiz_type($quiz_type_name);

    $quizid = db_insert('flipcard_quiz')
    ->fields(array(
      'uid' => $this->uid,
      'tid' => $tid,
      'quiz_type' => $quiz_type,
      'time_started' => time(),
      'time_ended' => time(),
      'correct' => 0,
      'questions' => 0))
      ->execute();

    return $quizid;
  }

  /**
   * Get the quiz machine name from the quiz type.
   */
  function get_quiz_type($quiz_type_name) {
    $sql = "SELECT type FROM {flipcard_quiz_type}
      WHERE name = '" . $quiz_type_name . "'";
    $result = db_query($sql);

    if ($result->rowCount() > 0) {
      $type = $result->fetchField();
    }
    else {
      $type = 'auto';
      watchdog('get_quiz_type', 'Unable to find quiz type; set to "auto". SQL was :sql',
      array(':sql' => $sql), WATCHDOG_ERROR);
    }

    return $type;
  }

  /**
   * Add question
   */
  function log_question($number, $nid, $correct, $start_level, $end_level) {
    //    $log_time = gmdate("Y-m-d H:i:s");
    $log_time = time(); // Unix timestamp

    // Convert boolean $correct into integer
    if ($correct) {
      $correct = 1;
    }
    else {
      $correct = 0;
    }

    db_insert('flipcard_question')
      ->fields(array(
        'quizid' => $this->quizid,
        'question' => $number,
        'nid' => $nid,
        'time_asked' => $log_time,
        'correct' => $correct,
        'start_level' => $start_level,
        'end_level' => $end_level,
        ))
      ->execute();

    db_update('flipcard_quiz')
      ->fields(array(
      'time_ended' => $log_time,
      ))
      ->expression('correct', 'correct + ' . $correct)
      ->expression('questions', 'questions + 1')
      ->condition('quizid', $this->quizid)
      ->execute();
  }

  /**
   * Change most recent question end level to 10.
   */
  function set_current_question_to_level_10() {
    $sql = "SELECT question
      FROM {flipcard_question}
      WHERE quizid = " . $this->quizid . "
        ORDER BY question DESC
        LIMIT 1";
    $result = db_query($sql);
    $last_question = $result->fetchField();

    db_update('flipcard_question')
    ->fields(array('end_level' => 10))
    ->condition('quizid', $this->quizid)
    ->condition('question', $last_question)
    ->execute();
  }

  /**
   * Get the results of the current quiz
   */
  function getResults() {
    $sql = "SELECT *
      FROM {flipcard_question}
      WHERE quizid = " . $this->quizid;
    $result = db_query($sql);

    $rows = array();
    while ($row = $result->fetchAssoc()) {
      $rows[] = $row;
    }

    return $rows;
  }

  /**
   * Find out how many questions were answered on this quiz
   */
  function questionsAnswered() {
    $answered = 0;

    $sql = "SELECT count(*) AS answered
      FROM {flipcard_question}
      WHERE quizid = " . $this->quizid;
    $result = db_query($sql);

    if ($result) {
      $answered = $result->fetchField();
    }

    return $answered;
  }
}

