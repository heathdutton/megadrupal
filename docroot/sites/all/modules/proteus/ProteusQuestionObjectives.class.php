<?php

/**
 * @file
 * ProteusQuestionObjectives keeps a list of the objectives for one question.
 */
class ProteusQuestionObjectives {

  /**
   * The node id of the question node.
   *
   * @var int
   */
  public $nid;
  
  /**
   * The objectives of this question.
   *
   * @var array
   *   keys are objective-ids
   *   - id: objective id
   *   - enterlevel: The minimum level a user needs for this objective to get
   *     this question.
   *   - exitlevel: The level the student will have for this objective after
   *     answering this question correct in one try.
   *   - tp: The training potential of this question for this objective.
   */
  public $objectives;
  
  /**
   * The current levels the user has for the different objectives.
   *
   * @var ProteusUserObjectives
   */
  public $userObjectives;
  
  /**
   * The target objectives the user is trying to reach.
   *
   * @var array
   *   keys are objective-ids, each entry contains an associative array with the
   *   information of one objective with the fields:
   *   - id: The id of the objective.
   *   - targetlevel: The target level the user tries to reach for this
   *     objective.
   */
  public $targetObjectives;
  
  /**
   * The overall training potential for all objectives for this question.
   *
   * @var int
   */
  public $tp;
  
  /**
   * how much points can maximally be gained with this question for any student.
   *
   * @var int
   */
  public $tot_diff = 0;
  
  /**
   * how much point can *this* user maximally gain from this question.
   *
   * @var int
   */
  public $max_gain = 0;

  /**
   * Constructs a new ProteusQuestionObjectives.
   *
   * @param int $nid
   *   The node id of the question to handle the objectives for.
   * @param ProteusUserObjectives $userObjectives
   *   The current levels the user has for the different objectives.
   * @param array $targetObjectives
   *   The target objectives for the user.
   *   keys are objective-ids, each entry contains an associative array with the
   *   information of one objective with the fields:
   *   - id: The id of the objective.
   *   - targetlevel: The target level the user tries to reach for this
   *     objective.
   */
  public function __construct($nid, ProteusUserObjectives $userObjectives, array $targetObjectives) {
    $this->nid = $nid;
    $this->userObjectives = $userObjectives;
    $this->targetObjectives = $targetObjectives;
    $this->objectives = array();
  }

  /**
   * Gather the objectives associated with the question and process them.
   */
  public function fillObjectives() {
    // SELECT termid,enterlevel,exitlevel FROM {proteus_question_objective} WHERE nid=%d
    $query = db_select('proteus_question_objective', 'pqo')->fields('pqo', array('termid', 'enterlevel', 'exitlevel'));
    $query->condition('nid', $this->nid);

    foreach ($query->execute() as $row) {
      $this->addObjective($row->termid, $row->enterlevel, $row->exitlevel);
    }
  }

  /**
   * Add an objective to the data set.
   *
   * @param int $id
   *   The objective id of the objective added.
   * @param int $enterlevel
   *   The enter-level that the question has for this objective.
   * @param int $exitlevel
   *   The exil-level that the question has for this objective.
   */
  public function addObjective($id, $enterlevel, $exitlevel) {
    $objective = array(
      'id' => $id,
      'enterlevel' => $enterlevel,
      'exitlevel' => $exitlevel,
    );
    $targetObjectiveLevel = 0;

    // Calculate the training potential of this question for this objective.
    $userObjective = $this->userObjectives->getObjective($id);

    if (isset($this->targetObjectives[$id])) {
      $targetObjectiveLevel = $this->targetObjectives[$id]['targetlevel'];
    }
    
    if ($userObjective && (0 < $targetObjectiveLevel)) {
      $objective['tp'] = ($objective['exitlevel'] - $userObjective['curlevel']) / $targetObjectiveLevel;
    }
    elseif (0 < $targetObjectiveLevel) {
      $objective['tp'] = ($objective['exitlevel'] - 0) / $targetObjectiveLevel;
    }
    else {
      $objective['tp'] = 0;
    }

    $this->tp += $objective['tp'];
    $this->objectives[$id] = & $objective;
  }

}
