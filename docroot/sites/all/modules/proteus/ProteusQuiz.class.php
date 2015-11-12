<?php

/**
 * @file
 * ProteusQuiz finds questions for the current user.
 */
class ProteusQuiz implements CqListenerQuestionInterface, CqQuestionInterface {

  /**
   * The Drupal node object of the proteus node.
   *
   * @var object
   */
  public $node;
  
  /**
   * The user object of the user taking the quiz.
   *
   * @var object
   */
  public $user;
  
  /**
   * The levels the user has for the different objectives.
   *
   * @var ProteusUserObjectives
   */
  public $userObjectives;
  
  /**
   * The target objectives with their levels.
   *
   * @var array
   *   keys are objective-ids, each entry contains an associative array with the
   *   information of one objective with the fields:
   *   - id: The id of the objective.
   *   - targetlevel: The target level the user tries to reach for this
   *     objective.
   */
  public $targets;
  
  /**
   * The CqUserAnswer implementation used to store user-specific data.
   *
   * @var CqUserAnswerDefault
   */
  public $userAnswer;
  
  /**
   * The user-specific data to store.
   *
   * @var array
   *   - nid: int - node_id of the question the user is currently making.
   *   - choice: string - The choice the user made during the last question
   *     selection.
   */
  public $data;
  
  /**
   * Has the user finished this proteus module?
   *
   * @var int
   *   -1: not checked yet.
   *   0: not finished.
   *   1: finished.
   */
  public $correct = -1;
  
  /**
   * The Drupal node of the current question.
   *
   * @var object
   */
  public $questionNode;
  
  /**
   * The url-variable used to pass the user's choice for the next question-step.
   *
   * @var string
   */
  public $nextLinkName;
  
  /**
   * The question listeners that are interested in question events.
   *
   * @var CqListenerQuestionInterface
   */
  private $listeners = array();
  
  /**
   * @var String the path that was used to load this question. We need to store
   * it because we can't trust the path if the object is stored in a cached
   * form and later used from a json call.
   */
  public $usedPath = '';

  /**
   * Default constructor for ProteusQuiz
   *
   * @param object $node
   *   The node object this quiz belongs to.
   * @param object $user
   *   The user object of the user taking the quiz.
   */
  public function __construct(&$node, &$user) {
    $this->usedPath = isset($_GET['q']) ? $_GET['q'] : '';
    $this->node = & $node;
    $this->user = & $user;
    $this->userObjectives = new ProteusUserObjectives($user);
    $this->targets = $node->proteus;
    $this->userAnswer = new CqUserAnswerDefault($node->nid, $user->uid);
    $this->data = $this->userAnswer->getAnswer();
    $this->nextLinkName = 'CqQS_' . $node->nid . '_Next';
  }

  /**
   * Fetches the userdata for an objective.
   *
   * @param int $termid
   *   The objective is to fetch the user data for.
   *
   * @return Array
   *   The objective data.
   *   - termid: the ID of the objective.
   *   - curlevel: the current level the user has for this objective.
   *   - maxlevel: the highest level the user ever reached for this objective.
   */
  public function getUserLevelsFor($termid) {
    return $this->userObjectives->getObjective($termid);
  }

  /**
   * Reset the levels the user already gained for the objectives that are target
   * objectives of this proteus quiz.
   */
  public function resetUserLevels() {
    // INSERT INTO {proteus_user_objective_log} SET time=%d, uid=%d, quizid=%d, msg="%s"
    db_insert('proteus_user_objective_log')->fields(array(
      'time' => REQUEST_TIME,
      'uid' => $this->user->uid,
      'tid' => 0,                       // There can be multiple, so don't specify here
      'quizid' => $this->node->nid,
      'questionid' => 0,                // This is not about questions, but the quiz. Nothing to specify
      'newlevel' => 0,                  // Not used in this action
      'levelinc' => 0,                  // Not used in this action
      'tries' => 0,                     // Not used in this action
      'msg' => 'Levels reset',
    ))->execute();

    foreach ($this->targets as $tid => $term) {
      $this->userObjectives->setLevelForObjective($tid, 0, $this->node->nid);
    }
    
    $this->userObjectives->storeObjectives();
    $this->data['nid'] = 0;
    $this->data['choice'] = '-';
    $this->userAnswer->setAnswer($this->data);
    $this->userAnswer->store();
    drupal_set_message(t('Levels reset.'));
  }

  
  /**
   * Clear the answers for all questions that have objectives that are target
   * objectives of this proteus quiz.
   */
  public function resetUserQuestions() {
    // INSERT INTO {proteus_user_objective_log} SET time=%d, uid=%d, quizid=%d, msg="%s"
    db_insert('proteus_user_objective_log')->fields(array(
      'time' => REQUEST_TIME,
      'uid' => $this->user->uid,
      'tid' => 0,                       // There can be multiple, so don't specify here
      'quizid' => $this->node->nid,
      'questionid' => 0,                // This is not about questions, but the quiz. Nothing to specify
      'newlevel' => 0,                  // Not used in this action
      'levelinc' => 0,                  // Not used in this action
      'tries' => 0,                     // Not used in this action
      'msg' => 'Questions reset',
    ))->execute();

    foreach ($this->targets as $tid => $term) {
      // SELECT QNO.nid FROM {proteus_question_objective} AS QNO INNER JOIN {cq_user_answer} AS UA ON QNO.nid=UA.nid AND uid=%d WHERE QNO.termid=%d
      $query = db_select('proteus_question_objective', 'QNO')->fields('QNO', array('nid'));
      $query->join('cq_user_answer', 'UA', 'QNO.nid = %alias.nid AND uid = :uid', array(':uid' => $this->user->uid));
      $query->condition('QNO.termid', $tid)->execute();

      foreach ($query->execute() as $row) {
        $ua = new CqUserAnswerDefault($row->nid, $this->user->uid);
        $ua->reset();
      }
    }
    drupal_set_message(t('Questions reset.'));
  }

  
  /**
   * TODO Echo some real information
   */
  public function getAllText() {
    return t("Proteus quiz text.");
  }


  /**
   * Is the user currently working on a question? If so, return the question ID.
   *
   * @return int
   *   Node id of the question the user is currently working on, or 0 if the
   *   user is between questions.
   *
   * @see clearOldQuestion()
   * @see selectQuestion()
   */
  public function hasOldQuestion() {
    return $this->data['nid'];
  }

  /**
   * Clear the "current" question ID, this sets the user to be "in between"
   * questions.
   *
   * @see hasOldQuestion()
   * @see selectQuestion()
   */
  public function clearOldQuestion() {
    $this->data['nid'] = 0;
    $this->userAnswer->setAnswer($this->data);
    $this->userAnswer->store();
  }

  /**
   * Find a question for the user, using the given selection rule.
   *
   * @param $selectionRule
   *   The selection rule to use.
   *
   * @see hasOldQuestion()
   * @see clearOldQuestion()
   */
  public function selectQuestion($selectionRule = PROTEUS_USEOLD) {
    if ($selectionRule == PROTEUS_USEOLD && $this->data['nid']) {
      $nid = $this->data['nid'];
    }
    else {
      $negativeIds = & $this->getNegativeIds();
      $positiveIds = array();
      $tempPosIds = & $this->getPositiveIds(FALSE);
      foreach ($tempPosIds as $v) {
        if (!in_array($v->nid, $negativeIds)) {
          $positiveIds[] = $v;
        }
      }
      if (count($positiveIds) == 0) {
        //drupal_set_message( t('No unanswered questions found, adding answered ones. Debug: p=%p n=%n', array(
        //  '%p' => count($tempPosIds),
        //  '%n' => count($negativeIds),
        //)));
        
        $tempPosIds = & $this->getPositiveIds(TRUE);
        $positiveIds = array();
        
        foreach ($tempPosIds as $v) {
          if (!in_array($v->nid, $negativeIds)) {
            $positiveIds[] = $v;
          }
        }
      }
      
      //if (count($positiveIds) == 0) {
      //  drupal_set_message(t('No questions found! Debug: p=%p n=%n', array(
      //    '%p' => count($tempPosIds),
      //    '%n' => count($negativeIds),
      //  )), 'error');
      //}

      $allQuestions = array();

      $largest = new stdClass();
      $largest->tp = -99999;
      $smallest = new stdClass();
      $smallest->tp = 99999;

      foreach ($positiveIds as $v) {
        $q = new ProteusQuestionObjectives($v->nid, $this->userObjectives, $this->targets);
        $q->fillObjectives();
        $allQuestions[] = $q;
        if ($q->tp > $largest->tp) {
          $largest = $q;
          //dpm('Largest=' . $q->nid);
        }
        elseif ($q->tp == $largest->tp && rand(0, 1) == 1) { // 50% chance we take the later question
          $largest = $q;
          //dpm('Largest=' . $q->nid);
        }
        if ($q->tp < $smallest->tp) {
          $smallest = $q;
          //dpm('Smallest=' . $q->nid);
        }
        elseif ($q->tp == $smallest->tp && rand(0, 1) == 1) { // 50% chance we take the later question
          $smallest = $q;
          //dpm('Smallest=' . $q->nid);
        }
      }
      
      $nid = 0;

      switch ($selectionRule) {
        case PROTEUS_SMALL:
          $choice = 'S';
          if ( isset( $smallest->nid ) ) {
            $nid = $smallest->nid;
          }
          break;
        case PROTEUS_MEDIUM:
          $choice = 'M';
          $midpoint = ($largest->tp + $smallest->tp) / 2;
          $middle = $smallest;
          $div = abs($midpoint - $smallest->tp);
          //dpm('Midpoint = ' . $midpoint);
          foreach ($allQuestions as $q) {
            $qdiv = abs($midpoint - $q->tp);
            if ($qdiv < $div) {
              //dpm('Picking ' . $q->nid . ' because ' . $qdiv . ' < ' . $div);
              $middle = $q;
              $div = $qdiv;
            }
            elseif ($qdiv == $div && rand(0, 1) == 1) {
              //dpm('Picking ' . $q->nid . ' because ' . $qdiv . ' == ' . $div);
              $middle = $q;
              $div = $qdiv;
            }
          }
          $nid = $middle->nid;
          break;
        case PROTEUS_LARGE:
        default:
          $choice = 'L';
          if ( isset( $largest->nid ) ) {
            $nid = $largest->nid;
          }
          break;
      }
      
      if ( isset( $nid ) ) {
        $this->data['nid'] = (int) $nid;
        $this->data['choice'] = $choice;
        $this->userAnswer->setAnswer($this->data);
        $this->userAnswer->store();

        $uaNextQuestion = new CqUserAnswerDefault((int) $nid, $this->user->uid);
        if ($uaNextQuestion->onceCorrect()) {
          $uaNextQuestion->reset();
          $uaNextQuestion->store();
        }
      }
    }
    //if (user_access(PROTEUS_CREATE)) {
    //  drupal_set_message(t('ProteusQuiz: Loading node %n. (Teacher only message)', array('%n' => $nid)));
    //}

    if ( isset( $nid ) ) {
      $this->questionNode = node_load($nid);
    }

    if (isset($this->questionNode->question)) {
      if (!user_access(PROTEUS_CREATE)) {
        $this->questionNode->noReset = 1;
      }

      $this->questionNode->question->addListener($this);
    }
    else {
      if ( !$this->onceCorrect() ) {
        drupal_set_message(t('This Proteus quiz has no no (more) question. Please contact your teacher about this.'), 'warning');
      }
    }
  }

  
  /**
   * Return the node object of the currently selected question.
   *
   * @return Object
   *   The node of the current question.
   */
  public function getQuestionNode() {
    return $this->questionNode;
  }

  
  /**
   * Get the list of question id's of questions that are outside of the
   * students scope, because the question has objectives with an enterlevel
   * that is higher than the current level of the student or higher than the
   * target level of the proteus quiz.
   *
   * TODO: currently disqualifies questions that also have objectives not in the
   * current module...
   *
   * @return array of ints.
   */
  private function &getNegativeIds() {
    $retval = array();

    // SELECT DISTINCT QNO.nid FROM {proteus_question_objective} AS QNO
    //   LEFT JOIN {proteus_quiz_objective_target} AS QZO
    //     ON QNO.termid = QZO.termid AND QZO.nid = $this->node->nid
    //   LEFT JOIN {proteus_user_objective_level} AS UOL
    //     ON UOL.termid = QNO.termid AND UOL.uid = $this->user->uid
    //   WHERE QZO.nid = $this->user->nid AND
    //     ( QNO.enterlevel > UOL.curlevel OR (QNO.enterlevel > 0 AND UOL.curlevel IS NULL) OR
    //       (QNO.enterlevel > QZO.targetlevel OR QZO.targetlevel IS NULL) )
    $query = db_select('proteus_question_objective', 'QNO')->fields('QNO', array('nid'));
    $aliasQZO = $query->leftJoin('proteus_quiz_objective_target', 'QZO', 'QNO.termid = %alias.termid AND %alias.nid = :QZO_nid', array(':QZO_nid' => $this->node->nid));
    $aliasUOL = $query->leftJoin('proteus_user_objective_level', 'UOL', '%alias.termid = QNO.termid AND %alias.uid = :UOL_uid', array(':UOL_uid' => $this->user->uid));
    $query->condition("$aliasQZO.nid", $this->node->nid);
    $query->condition(db_or()->where("QNO.enterlevel > $aliasUOL.curlevel")->condition(db_and()->condition('QNO.enterlevel', 0, '>')->isNull("$aliasUOL.curlevel"))->condition(db_or()->where("QNO.enterlevel > $aliasQZO.targetlevel")->isNull("$aliasQZO.targetlevel")));
    $query->distinct();

    //drupal_set_message( (string) $query );
    //drupal_set_message( $query->arguments() );

    foreach ($query->execute() as $row) {
      $retval[] = $row->nid;
    }

    return $retval;
  }

  
  /**
   * Get the list of question id's that have objectives that match the current
   * levels of the student.
   *
   * @param boolean $include_answered
   *   Also include questions that the student has already answered?
   *
   * @return Array of ints.
   */
  private function &getPositiveIds($include_answered = FALSE) {
    $retval = array();
    
    /* here we find all the positive matches
     * we order them from least interesting to most interesting, because
     * we would like to have the most interesting questions at the end of the array
     */
    // if ($include_answered) {
    //   $answered = "";
    // }
    // else {
    //   $answered = "(UA.nid IS NULL OR UA.once_correct = 0) AND ";
    // }
    // SELECT DISTINCT QNO.nid, COUNT(UA2.uid) AS UACOUNT FROM {proteus_question_objective} AS QNO
    //   INNER JOIN {proteus_quiz_objective_target} AS QZO
    //     ON QNO.termid = QZO.termid AND QZO.nid = %d
    //   LEFT JOIN {cq_user_answer} AS UA
    //     ON QNO.nid = UA.nid AND UA.uid = %d
    //   LEFT JOIN {proteus_user_objective_level} AS UOL
    //     ON UOL.termid = QNO.termid AND UOL.uid = %d
    //   LEFT JOIN {cq_user_answer} AS UA2
    //     ON UA2.nid = QNO.nid
    //   WHERE ' . $answered . '
    //     ( (UOL.curlevel IS NULL AND QNO.enterlevel = 0) OR
    //       (UOL.curlevel < QZO.targetlevel AND UOL.curlevel < QNO.exitlevel AND UOL.curlevel >= QNO.enterlevel) )
    //   GROUP BY QNO.nid, QZO.targetlevel, UOL.curlevel
    //   ORDER BY UACOUNT DESC
    $query = db_select('proteus_question_objective', 'QNO')->fields('QNO', array('nid'));
    $aliasQZO = $query->join('proteus_quiz_objective_target', 'QZO', 'QNO.termid = %alias.termid AND %alias.nid = :QZO_nid', array(':QZO_nid' => $this->node->nid));
    $aliasUA = $query->leftJoin('cq_user_answer', 'UA', 'QNO.nid = %alias.nid AND %alias.uid = :UA_uid', array(':UA_uid' => $this->user->uid));
    $aliasUOL = $query->leftJoin('proteus_user_objective_level', 'UOL', '%alias.termid = QNO.termid AND %alias.uid = :UOL_uid', array(':UOL_uid' => $this->user->uid));
    $aliasUA2 = $query->leftJoin('cq_user_answer', 'UA2', '%alias.nid = QNO.nid');
    $query->addExpression("COUNT($aliasUA2.uid)", 'UACOUNT'); // Adding here is the correct way, with the guaranteed alias

    if (!$include_answered) {
      $query->condition(db_or()->isNull("$aliasUA.nid")->condition("$aliasUA.once_correct", 0));
    }
    $query->condition(db_or()->condition(db_and()->isNull("$aliasUOL.curlevel")->condition('QNO.enterlevel', 0))->condition(db_and()->where("$aliasUOL.curlevel < $aliasQZO.targetlevel")->where("$aliasUOL.curlevel < QNO.exitlevel")->where("$aliasUOL.curlevel >= QNO.enterlevel")));
    // echo 'ordering on how many other students already answered the question<br>';
    // Updated to select questions from all modules, as long as the objectives match
    $query->groupBy('QNO.nid')->groupBy("$aliasQZO.targetlevel")->groupBy("$aliasUOL.curlevel")->orderBy('UACOUNT', 'desc')->distinct();

    //drupal_set_message( (string) $query );
    //drupal_set_message( $query->arguments() );

    foreach ($query->execute() as $row) {
      $retval[] = $row;
    }

    return $retval;
  }

  
  /**
   * Generate an overview of all the questions that have objectives that match
   * the target objectives of this proteus quiz.
   *
   * @return string
   *   The themed table with the overview.
   */
  public function generateQuestionOverview() {
    $questions_byId = array();
    $questions_sorted = array();
    $maxExit_byTermId = array();

    foreach ($this->targets as $tid => $target) {
      // SELECT QNO.nid, QNO.termid, QNO.enterlevel, QNO.exitlevel, N.title
      //   FROM {proteus_question_objective} AS QNO
      //   INNER JOIN {node} AS N
      //     ON N.nid = QNO.nid
      //   WHERE QNO.termid=%d
      //   ORDER BY QNO.exitlevel";
      $query = db_select('proteus_question_objective', 'QNO')->fields('QNO', array('nid', 'termid', 'enterlevel', 'exitlevel'));
      $aliasN = $query->join('node', 'N', '%alias.nid = QNO.nid');
      $query->fields("$aliasN", array('title'));
      $query->condition('QNO.termid', $tid);
      $query->orderBy('QNO.exitlevel', 'asc');

      $maxExit = 0;
      
      foreach ($query->execute() as $row) {
        $q_nid = $row->nid;
        $maxExit = max($maxExit, (int) $row->exitlevel);

        if (!isset($questions_byId[$q_nid])) {
          $new_question = array();
          $new_question['nid'] = $q_nid;
          $new_question['title'] = $row->title;
          $new_question['objectives'][$tid]['enterlevel'] = (int) $row->enterlevel;
          $new_question['objectives'][$tid]['exitlevel'] = (int) $row->exitlevel;
          $new_question['objectives'][$tid]['leading_count'] = 0;

          $questions_byId[$q_nid] = $new_question;
          $questions_sorted[] = & $questions_byId[$q_nid];
        }
        else {
          $questions_byId[$q_nid]['objectives'][$tid]['enterlevel'] = (int) $row->enterlevel;
          $questions_byId[$q_nid]['objectives'][$tid]['exitlevel'] = (int) $row->exitlevel;
          $questions_byId[$q_nid]['objectives'][$tid]['leading_count'] = 0;
        }
      }
      $maxExit_byTermId[$tid] = $maxExit;
    }

    $header = array();
    $rows = array();
    $termNames = array();
    $header[] = t('Question');
    
    foreach ($this->targets as $tid => $target) {
      $taxterm = taxonomy_term_load($tid);
      $termNames[$tid] = $taxterm->name . '(' . $tid . ')';
      $header[] = array(
        'data' => $termNames[$tid],
        'header' => 1,
        'colspan' => '1',
      );
    }
    
    foreach ($questions_sorted as $question) {
      $row = array();
      $row[] = l($question['title'] . ' (' . $question['nid'] . ')', drupal_get_path_alias('node/' . $question['nid']));
      foreach ($this->targets as $tid => $target) {
        if ( array_key_exists($tid, $question['objectives']) ) {
          $level_enter = $question['objectives'][$tid]['enterlevel'];
          $level_exit = $question['objectives'][$tid]['exitlevel'];
          if ($level_enter || $level_exit) {

            $max = $target['targetlevel'];
            if ($max == 0) {
              $max = $maxExit_byTermId[$tid] - 1;
            }
            $max = max($max, 1);
            $right = min(100, 100 * $level_exit / $max);
            $left = 100 * $level_enter / $max;
            $class = 'bar';
            if ($left >= $right) {
              //drupal_set_message($tid . ' ' . $maxExit_byTermId[$tid] . ': $left >= $right : ' . $left . '>=' . $right . ' max=' . $max . ' enter=' . $level_enter . ' exit=' . $level_exit);
              $right = $left + 1;
              $class = 'barbad';
            }
            $row[] = $level_enter . ' - ' . $level_exit . '<div class="' . $class . '" style="margin-left:' . $left . '% ;width: ' . ($right - $left) . '%;"> </div>';
          }
          else {
            // No valid levels defined for this question
            $row[] = array(
              'data' => '-',
              'colspan' => '1',
            );
          }
        }
        else {
          // This question is not attached to this objective (term)
          $row[] = array(
            'data' => '-',
            'colspan' => '1',
          );
        }
      }
      
      $rows[] = $row;
    }

    $retval = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'proteus-questions')));
    $retval .= t('Total number of questions: %total', array('%total' => count($rows)));
    
    return $retval;
  }


  /**
   * Implements CqListenerQuestionInterface::FirstSolutionFound().
   */
  public function FirstSolutionFound($tries) {
    //drupal_set_message('Question answered correct in ' . $tries . ' tries! Need to update levels.');
    $finished = TRUE;
    foreach ($this->questionNode->proteus as $objectiveId => $objective) {
      $enterLevel = $objective['enterlevel'];
      $exitLevel = $objective['exitlevel'];
      $userLevels = $this->userObjectives->getObjective($objectiveId);
      $userLevel = 0;
      if ($userLevels) {
        $userLevel = $userLevels['curlevel'];
      }

      $f2 = (min($exitLevel, $userLevel) - $enterLevel) / max(1, $exitLevel - $userLevel);
      $f1 = $tries / ($tries + $f2 + 1);
      $r = $exitLevel - $enterLevel;
      $newLevel = max($userLevel, $exitLevel) - $f1 * $r;
      //dpm('$f2 = ' . $f2 . ' $f1 = ' . $f1 . ' $r = ' . $r);
      //dpm('Updating objective ' . $objectiveId . ' from ' . $userLevel . ' to ' . $newLevel);
      if ($newLevel < $this->targets[$objectiveId]['targetlevel']) {
        $finished = FALSE;
      }
      // The found new level is defined as a float here. In the setLevelForObjective function it is rounded to the desired integer
      $this->userObjectives->setLevelForObjective($objectiveId, $newLevel, $this->node->nid, $this->questionNode->nid, $tries);
    }
    
    $this->userObjectives->storeObjectives();
    
    if ($finished) { // The objectives for THIS question are done, check the rest too!
      if ($this->isCorrect() == 1) {
        $this->userAnswer->setCorrect(1);

        // INSERT INTO {proteus_user_objective_log} SET time=%d, uid=%d, quizid=%d, msg="%s"
        db_insert('proteus_user_objective_log')->fields(array(
          'time' => REQUEST_TIME,
          'uid' => $this->user->uid,
          'tid' => 0,                       // There can be multiple, so don't specify here
          'quizid' => $this->node->nid,
          'questionid' => 0,                // This is not about questions, but the quiz. Nothing to specify
          'newlevel' => 0,                  // Not used in this action
          'levelinc' => 0,                  // Not used in this action
          'tries' => 0,                     // Not used in this action
          'msg' => 'Finished',
        ))->execute();
      }
    }
    
    $this->userAnswer->increaseTries();
    $this->userAnswer->store();
  }


  /**
   * Implements CqListenerQuestionInterface::getExtraFeedbackItems().
   */
  public function getExtraFeedbackItems($caller, $tries) {
    $feedback = array();
    
    if ($caller->onceCorrect()) {
      $nextLink = '';
      
      // For teachers or when enabled through the configuration: print the question score gain.
      if ( user_access(PROTEUS_CREATE) || variable_get('proteus_show_progress', FALSE) ) {
        $header = array('Topic', 'Score gain');
        $rows = array();
  
        foreach ($this->questionNode->proteus as $objectiveId => $objective) {
          $taxterm = taxonomy_term_load($objectiveId);
          $userLevels = $this->userObjectives->getObjective($objectiveId);
          // Question can have multiple objectives (topics) that could not be configured for this quiz. Don't show them
          if ( $taxterm && $userLevels && isset( $this->targets[$objectiveId] ) ) {
            $rows[] = array($taxterm->name,
              $userLevels['curlevel'] - $userLevels['prevcurlevel']
            );
          }
        }
        $nextLink = theme('table', array('header' => $header, 'rows' => $rows /*, 'attributes' => array('id' => 'proteus-objectives')*/));

        $fbItem = new CqFeedback();
        $fbItem->initWithValues($nextLink, 0, 9999);
        $feedback[] = $fbItem;
      }

      // Show the 'next question' button next to the 'submit answer' or 'Reset' button
      $linkLink = url( $this->usedPath, array('query' => array($this->nextLinkName => 1)) );
      $linkText = t('Go to the next question');
      // With the 'input.form-submit.next_question' style in a CSS you can adapt the style as desired
      $nextLink = "<input type='button' id='button_next_cq' class='form-submit next_question' value='$linkText' onclick='window.location.href = \"$linkLink\"'>";
      $fbItem = new CqFeedback();
      $fbItem->initWithValues($nextLink, 0, 9999);
      $fbItem->setBlock('next');    // By using the block feature, the link is positioned next to the submit button (see Proteus.module, hook_view)
      $feedback[] = $fbItem;
    }

    return $feedback;
  }

  
  /**
   * Implements CqQuestionInterface::isCorrect().
   */
  public function isCorrect() {
    if ($this->correct == -1) {
      $realFinished = TRUE;
      foreach ($this->targets as $tid => $target) {
        $userLevels = $this->userObjectives->getObjective($tid);
        if ($userLevels) {
          if ($userLevels['curlevel'] < $target['targetlevel']) {
            $realFinished = FALSE;
            break;
          }
        }
        else {
          $realFinished = FALSE;
          break;
        }
      }

      if ($realFinished) {
        $this->correct = 1;
        if (!$this->onceCorrect()) {
          $this->fireFirstSolutionFound();
        }
        $this->userAnswer->setCorrect(1);
        $this->userAnswer->store();
      }
      else {
        $this->correct = 0;
      }
    }

    return $this->correct;
  }


  /**
   * Implements CqQuestionInterface::onceCorrect().
   */
  public function onceCorrect() {
    return $this->userAnswer->onceCorrect();
  }

  /**
   * Implements CqQuestionInterface::getTries().
   */
  public function getTries() {
    return $this->userAnswer->getTries();
  }

  /**
   * Implements CqQuestionInterface::addExtraFeedback().
   */
  public function addExtraFeedback(CqFeedback $fbItem) {
    $this->questionNode->addExtraFeedback($fbItem);
  }

  /**
   * Implements CqQuestionInterface::reset().
   */
  public function reset() {
    $this->resetUserQuestions();
  }

  /**
   * Returns the UserAnswer of this question
   *
   * @return object
   *   cqUserAnswer object.
   */
  public function &getUserAnswer() {
    return $this->userAnswer;
  }

  /**
   * Implements CqQuestionInterface::addListener().
   */
  public function addListener(CqListenerQuestionInterface &$listener) {
    $this->listeners[] = & $listener;
  }

  /**
   * Fire an event because the user found the first solution.
   */
  public function fireFirstSolutionFound() {
    foreach ($this->listeners as $listener) {
      $listener->FirstSolutionFound($this->userAnswer->getTries());
    }
  }

}
