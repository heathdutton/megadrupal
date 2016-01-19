<?php

/**
 * @file
 * ProteusUserObjectives holds the levels the user has for objectives.
 *
 * Changes to levels are not stored to the backend until storeObjectives() is
 * called.
 */
class ProteusUserObjectives {

  /**
   * The user-id of the user to handle the levels for.
   *
   * @var int
   */
  public $uid;
  
  /**
   * The objectives and the levels that the user has.
   *
   * @var array
   *   Associative array, keys are objective ids
   *   - changed: boolean - has the level for this objective changed?
   *   - new: boolean - is objective new for the user, or did he already have a
   *     level in this objective?
   *   - curlevel: int - The current level the user has for this objective.
   *   - prevcurlevel: int - The previous current level. This valus is used to calculate the score gain for the answered question
   *   - maxlevel: int - The highest level the user ever reached for this
   *     objective.
   */
  public $objectives;

  /**
   * Constructs a new ProteusUserObjectives.
   *
   * @param object $user
   *   The Drupal user object of the user to handle the objectives for.
   */
  public function __construct($user) {
    $this->uid = $user->uid;
    $this->fillObjectives();
  }

  /**
   * Load the objectives of the user from the database.
   */
  private function fillObjectives() {
    if ($this->uid > 0) {
      // SELECT termid, curlevel, maxlevel FROM {proteus_user_objective_level} WHERE uid=%d
      $query = db_select('proteus_user_objective_level', 'puol')->fields('puol', array('termid', 'curlevel', 'maxlevel'));
      $query->condition('uid', $this->uid);

      $this->objectives = array();
      foreach ($query->execute() as $row) {
        $this->objectives[$row->termid] = array();
        $this->objectives[$row->termid]['termid']   = $row->termid;
        $this->objectives[$row->termid]['prevcurlevel'] = $row->curlevel;
        $this->objectives[$row->termid]['curlevel'] = $row->curlevel;
        $this->objectives[$row->termid]['maxlevel'] = $row->maxlevel;
        $this->objectives[$row->termid]['changed'] = FALSE;
        $this->objectives[$row->termid]['new'] = FALSE;
      }
    }
    else {
      if (!isset($_SESSION['cq']['proteus']['objectives'])) {
        $_SESSION['cq']['proteus']['objectives'] = array();
      }
      $this->objectives = & $_SESSION['cq']['proteus']['objectives'];
    }
  }

  /**
   * Write to the backend any changes to the levels a user has for objectes.
   */
  public function storeObjectives() {
    if ($this->uid > 0) {
      foreach ($this->objectives as $objective) {
        if ($objective['new']) {
          // Check against race condition on the insert.
          $row = $this->getSingleObjective($objective['termid']);
          if ($row) {
            $this->updateSingleObjective($objective['termid'], $objective['curlevel'], $objective['maxlevel']);
          }
          else {
            $this->insertSingleObjective($objective['termid'], $objective['curlevel'], $objective['maxlevel']);
          }
        }
        elseif ($objective['changed']) {
          $this->updateSingleObjective($objective['termid'], $objective['curlevel'], $objective['maxlevel']);
        }
      }
    }
  }

  /**
   * Inserts a new objective for this user into the database.
   *
   * @param int $termid
   *   The objective id of the objective to insert.
   * @param int $curlevel
   *   The current level the user has for this objective.
   * @param int $maxlevel
   *   The maximum level the user ever reached for this objective.
   */
  private function insertSingleObjective($termid, $curlevel, $maxlevel) {
    // INSERT INTO {proteus_user_objective_level} SET uid=%d, termid=%d, curlevel=%d, maxlevel=%d
    db_insert('proteus_user_objective_level')->fields(array(
      'uid' => $this->uid,
      'termid' => $termid,
      'curlevel' => $curlevel,
      'maxlevel' => $maxlevel,
    ))->execute();
  }

  /**
   * Updates a single objective in the database.
   *
   * @param int $termid
   *   The objective id of the objective to insert.
   * @param int $curlevel
   *   The current level the user has for this objective.
   * @param int $maxlevel
   *   The maximum level the user ever reached for this objective.
   */
  private function updateSingleObjective($termid, $curlevel, $maxlevel) {
    // UPDATE {proteus_user_objective_level} SET curlevel=%d, maxlevel=%d WHERE uid=%d AND termid=%d
    db_update('proteus_user_objective_level')->fields(array(
      'curlevel' => $curlevel,
      'maxlevel' => $maxlevel,
    ))->condition('uid', $this->uid)->condition('termid', $termid)->execute();
  }

  /**
   * Fetches a single objective from the database.
   *
   * @param int $termid
   *   The objective id of the objective to fetch the data for.
   *
   * @return array
   *   An associative array with the fields:
   *   - curlevel: int - The current level the user has for this objective.
   *   - maxlevel: int - The maximum level the user ever reached for this
   *     objective.
   */
  private function getSingleObjective($termid) {
    // SELECT curlevel,maxlevel FROM {proteus_user_objective_level} WHERE uid=%d AND termid=%d
    $query = db_select('proteus_user_objective_level', 'puol')->fields('puol', array('curlevel', 'maxlevel'));
    return $query->condition('uid', $this->uid)->condition('termid', $termid)->execute()->fetchField();
  }

  /**
   * Fetches the userdata for an objective returns an array with the fields:
   *
   * @param int $termid
   *   The objective id of the objective to fetch the data for.
   *
   * @return array or boolean FALSE
   *   FALSE if the objective is not found, or an associative array with the
   *   fields:
   *   - termid: int - the objective id of the objective.
   *   - curlevel: int - The current level the user has for this objective.
   *   - maxlevel: int - The maximum level the user ever reached for this
   *     objective.
   */
  public function getObjective($termid) {
    if (isset($this->objectives[$termid])) {
      return $this->objectives[$termid];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Set the user's level for a specific objective and log the change.
   *
   * @param int $termid
   *   The objective ID to set the level for.
   * @param int $curlevel
   *   The level to set.
   * @param int $quizid
   *   The quiz id that caused the change. For logging only. Default=0
   * @param int $questionid
   *   The question id that caused the change. For logging only.  Default=0
   * @param int $tries
   *   The number of tries that caused the change. For logging only.  Default=0
   */
  public function setLevelForObjective($termid, $curlevel, $quizid = 0, $questionid = 0, $tries = 0) {
    $change = 0;
    
    // When the user has answered a question correct, the new level is calculated and is given here as a float. The
    // MySQL database interface or the Drupal database abstract layer converts this float to a round-up integer value
    // when it is stored. This is not a correct way, let it up to the database interface. Be precise; round-up here
    $curlevel = round( $curlevel );
    
    if (!isset($this->objectives[$termid])) {
      $this->objectives[$termid] = array(
        'new' => TRUE,
        'changed' => TRUE,
        'termid' => $termid,
        'prevcurlevel' => 0,
        'curlevel' => $curlevel,
        'maxlevel' => $curlevel,
      );
      $change = $curlevel;
    }
    else {
      if ($this->objectives[$termid]['curlevel'] != $curlevel) {
        $change = $curlevel - $this->objectives[$termid]['curlevel'];
        $this->objectives[$termid]['changed'] = TRUE;
        $this->objectives[$termid]['prevcurlevel'] = $this->objectives[$termid]['curlevel'];
        $this->objectives[$termid]['curlevel'] = $curlevel;
        $this->objectives[$termid]['maxlevel'] = max($this->objectives[$termid]['maxlevel'], $curlevel);
      }
    }
    
    if ($change != 0) {
      $msg = "Objective level reset";
      if ( $questionid == 0 ) {
        $msg = "Objective level update";
      }

      // INSERT INTO {proteus_user_objective_log} SET time=%d, uid=%d, tid=%d, quizid=%d, questionid=%d, newlevel=%d, levelinc=%d, tries=%d
      db_insert('proteus_user_objective_log')->fields(array(
        'time' => REQUEST_TIME,
        'uid' => $this->uid,
        'tid' => $termid,
        'quizid' => $quizid,
        'questionid' => $questionid,
        'newlevel' => $curlevel,
        'levelinc' => $change,
        'tries' => $tries,
        'msg' => $msg,
      ))->execute();
    }
  }
}
