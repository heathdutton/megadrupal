<?php

/**
 * @file
 * Description of LinearCaseclass
 *
 * @author hylke
 */
class LinearCase implements CqListenerQuestionInterface, CqQuestionInterface {

  /**
   * The Drupal node object of the node that is this linearCase
   *
   * @var object
   */
  private $node;

  /**
   * The Drupal user object of the user accessing this LinearCase.
   *
   * @var object
   */
  private $user;

  /**
   * The UserAnswer object used to store user data for this case.
   *
   * @var CqUserAnswer
   */
  private $userAnswer;

  /**
   * An associative array of data to store in the userAnswer.
   *
   * @var array
   *  - pgNids: array of int - The node id's the user is allowed to access.
   */
  private $data;

  /**
   * The node in the case that the user is currently viewing.
   *
   * @var object
   */
  private $currentNode;

  /**
   * The nid of the currently selected node
   *
   * @var int
   */
  private $lnid = 0;

  /**
   * The node id of the next node in the case.
   *
   * @var int
   *   or boolean FALSE if not set.
   */
  private $nextId = FALSE;

  /**
   * The node id of the previous node in the case.
   *
   * @var int
   *   or boolean FALSE if not set.
   */
  private $prevId = FALSE;

  /**
   * Is the user allowed to continue on from the current question. If -1 the
   * checks have not been done yet.
   *
   * @var int
   */
  private $nextAllowed = -1;

  /**
   * The base-path of this case. Used to generate all url's
   *
   * @var string
   */
  private $path;

  /**
   * The url of the link to the next page in the case.
   *
   * @var string
   */
  private $nextLinkUrl = '';

  /**
   * The url of the link to the previous page in the case.
   *
   * @var string
   */
  private $prevLinkUrl = '';

  /**
   * Constructs a new LinearCase.
   *
   * @param object $node
   *   The Drupal node object of this case.
   * @param object $user
   *   The Drupal user object of the user accessing this case.
   */
  public function __construct(&$node, &$user) {
    $this->node = & $node;
    $this->user = & $user;
    $this->userAnswer = new CqUserAnswerDefault($node->nid, $user->uid);
    $this->data = $this->userAnswer->getAnswer();
    if (!isset($this->data['pgNids'])) {
      $this->data['pgNids'] = array();
    }
    $this->path = 'node/' . $this->node->nid;
  }

  /**
   * Convertes a link in the book array into a node id.
   *
   * @param array $book_link
   *   An entry in the book array.
   *
   * @return int
   *   The node id of that entry.
   */
  private function nodeIdFromBookLink($book_link) {
    $nid = 0;
    if ($book_link['router_path'] == 'node/%') {
      $nid = (int) drupal_substr($book_link['link_path'], 5);
    }
    if (!$nid) {
      drupal_set_message(t('LinearCase::nodeIdFromBookLink: not a node?'));
      //dpm($book_link);
      //ddebug_backtrace();
    }
    return $nid;
  }

  /**
   * Finds the id of the first node in the case.
   *
   * @return int
   *   The node id of the first node in the case.
   */
  public function findFirstNodeID() {
    // drupal_set_message('LinearCase::findFirstNodeID');
    $nid = 0;
    $flat_menu = book_get_flat_menu($this->node->book);
    $entry = next($flat_menu);
    if ($entry) {
      $nid = $this->nodeIdFromBookLink($entry);
    }
    if ($nid && !in_array($nid, $this->data['pgNids'])) {
      $this->addToList($nid);
      $this->store();
    }
    return $nid;
  }

  /**
   * Adds a node to the list of nodes the user is allowed to see.
   *
   * @param int $nid
   *   The node id to add.
   */
  public function addToList($nid) {
    $this->data['pgNids'][] = $nid;
  }

  /**
   * Sets the node id of the currently selected node. It first checks if the
   * user is already allowed to go to that node. If not, it selects the node
   * furthest in the case that the user can go to.
   *
   * @param int $nid
   *   The sugested node id to select.
   */
  public function selectNode($nid = 0) {
    if ($nid) {
      if (!in_array($nid, $this->data['pgNids'])) {
        $nnid = end($this->data['pgNids']);
        drupal_set_message(
            t('You can not access that page (%target) yet, sending you to (%redirect) instead.', array(
          '%target' => $nid,
          '%redirect' => $nnid,
                )
            )
        );
        $nid = $nnid;
      }
      $this->setSelectedNid($nid);
    }
  }

  /**
   * Sets the node id of the currently selected node.
   *
   * @param int $nid
   *   The node id to set.
   */
  private function setSelectedNid($nid) {
    if ((int) $nid && $nid != $this->node->nid) {
      $this->lnid = (int) $nid;
    }
  }

  /**
   * Returns the currently selected node. If none selected yet, selects and
   * loads the first node of the case.
   *
   * @return object
   *   The currently selected node.
   */
  public function getNode() {
    if (!$this->currentNode) {
      if (!$this->lnid) {
        $this->lnid = $this->findFirstNodeID();
      }
      if ($this->lnid) {
        $this->currentNode = node_load($this->lnid);
        if (isset($this->currentNode->question)) {
          $this->currentNode->question->addListener($this);
        }
      }
    }

    return $this->currentNode;
  }

  /**
   * Prepare a node for viewing and return the result of node_view.
   * This removes the book navigation from the node.
   *
   * @param object $node
   *   The node to view
   *
   * @return string
   *   The result of node_view($node)
   */
  public function viewNode($node) {
    dpm($node);

    $node->caseBook = $node->book;
    unset($node->book);
    $node->title = NULL;
    return node_view($node);
  }

  /**
   * Returns the base-path of this case.
   *
   * @return String
   *   The base path.
   */
  public function getPath() {
    return $this->path;
  }

  /**
   * Returns the path to the next page of the case, relative to the current
   * page.
   *
   * @return String
   *   The path to the next page.
   */
  public function getNextPath() {
    $nextId = $this->getNextId();
    if ($nextId) {
      $this->nextLinkUrl = $this->path . '/goto/' . $nextId;
    }
    return $this->nextLinkUrl;
  }

  /**
   * Returns the path to the previous page of the case, relative to the current
   * page.
   *
   * @return String
   *   The path to the previous page.
   */
  public function getPrevPath() {
    $prevId = $this->getPrevId();
    if ($prevId && $prevId != $this->node->nid) {
      $this->prevLinkUrl = $this->path . '/goto/' . $prevId;
    }
    return $this->prevLinkUrl;
  }

  /**
   * Returns the node id of the next page of the case.
   *
   * @return int
   *   The node id of the next page.
   */
  public function getNextId() {
    if (!$this->nextId && $this->canContinue()) {
      if (isset($this->currentNode->book)) {
        $nextLink = book_next($this->currentNode->book);
      }
      else {
        $nextLink = book_next($this->currentNode->caseBook);
      }
      if ($nextLink) {
        $this->nextId = $this->nodeIdFromBookLink($nextLink);
        if (!in_array($this->nextId, $this->data['pgNids'])) {
          $this->addToList($this->nextId);
          $this->store();
        }
      }
    }
    return $this->nextId;
  }

  /**
   * Returns the node id of the previous page of the case.
   *
   * @return int
   *   The node id.
   */
  public function getPrevId() {
    if (!$this->prevId) {
      $prevLink = book_prev($this->currentNode->book);
      $this->prevId = $this->nodeIdFromBookLink($prevLink);
    }
    return $this->prevId;
  }

  /**
   * Check if the user is allowed to continue to the next page. A user is not
   * allowed to continue if the current page is a blocking content type that
   * has not been completed yet.
   *
   * @return boolean
   *   TRUE if the user can continue, FALSE otherwise.
   */
  public function canContinue() {
    //drupal_set_message('LinearCase::canContinue');
    if ($this->nextAllowed == -1) {
      if ($this->checkNodeAccess($this->currentNode->nid) < 2) {
        $this->nextAllowed = 1;
      }
      else {
        $this->nextAllowed = 0;
      }
    }
    return ($this->nextAllowed > 0);
  }

  /**
   * Checks the type of a node, and if it's a question, whether is was answered
   * correctly or not.
   *
   * @param int $nodeId
   *   The id of the node to check
   *
   * @return int
   *   0 - Not a question
   *   1 - A question and answered correctly.
   *   2 - A question but not answered correctly.
   */
  public function checkNodeAccess($nodeId) {
    if ($nodeId == $this->lnid) {
      $node = $this->getNode();
    }
    else {
      $node = node_load($nodeId);
    }
    if ($node) {
      if (isset($node->question)) {
        if ($node->question->onceCorrect()) {
          //drupal_set_message('LinearCase::checkNodeAccess: node ' . $nodeId . ' is a question and answered correct.');
          return 1;
        }
        else {
          //drupal_set_message('LinearCase::checkNodeAccess: node ' . $nodeId . ' is a question but not answered correct yet.');
          return 2;
        }
      }
      elseif ($node->type == 'webform') {
        // Not very nice, but we have no other way to check as far as I could find out.
        $submission_count = db_query('SELECT count(*) FROM {webform_submissions} WHERE nid = :nid AND uid = :uid', array(':nid' => $node->nid, ':uid' => $this->user->uid))->fetchField();
        if ($submission_count == 0) {
          //drupal_set_message('LinearCase::checkNodeAccess: node ' . $nodeId . ' is a webform and not answered.');
          return 2;
        }
        else {
          //drupal_set_message('LinearCase::checkNodeAccess: node ' . $nodeId . ' is a webform and answered.');
          return 1;
        }
      }
      else {
        //drupal_set_message('LinearCase::checkNodeAccess: node ' . $nodeId . ' is a ' . $node->type . '.');
      }
    }
    return 0;
  }

  /**
   * Converts the book-tree into a case-tree.
   *
   * @return array
   *   A tree ready for rendering.
   */
  public function getTree() {
    if (isset($this->node->book)) {
      $bookData = $this->node->book;
    }
    else {
      $bookData = $this->node->caseBook;
    }

    $curNode = $this->getNode();
    $curNodeBook = $curNode->caseBook;
    $tree = menu_tree_all_data($bookData['menu_name']);

    // TODO: This should probably be cached?
    //dpm($this->data);
    list($last, $endFound) = $this->fixTreeLinks($tree, $curNodeBook, 1);

    if (!$endFound && !$this->userAnswer->onceCorrect()) {
      // There is no end, case finished!
      $this->userAnswer->setCorrect(1);
    }

    $lastUrl = url($this->path . '/goto/' . $last . '/i');
    $selector = "a[href='" . $lastUrl . "']";
    drupal_add_js('jQuery(document).ready(function() {
      jQuery("' . $selector . '").css("color","gray");
      jQuery("' . $selector . '").attr("href","javascript:void(0);");
    });', array('type' => 'inline', 'scope' => JS_DEFAULT));

    $this->store();
    // dpm($tree);
    return $tree;
  }

  /**
   * Checks every node of this book-branch to see if the user is allowed to go
   * there yet. If not, the link of that node is changed.
   *
   * @param array $brach
   *   The branch to check
   * @param array $curBookLink
   *   The book-link of the currently selected node.
   * @param int $level
   *   Current recursion depth
   * @param int $last
   *   The node id of the furthest node found so far that the user was allowed
   *   to go to.
   * @param boolean $endFound
   *   Has the furthest node the user is allowed to navigate to been found yet?
   *
   * @return array
   *   [0]: int - The node id of the furthest node found so far that the user
   *        was allowed to go to.
   *   [1]: boolean - Has the furthest node the user is allowed to navigate to
   *        been found yet?
   */
  private function fixTreeLinks(&$brach, $curBookLink, $level = 1, $last = 0, $endFound = FALSE) {
    foreach ($brach as $id => $item) {
      $nodeId = $this->nodeIdFromBookLink($item['link']);
      //drupal_set_message('fixTreeLinks:: checking node=' . $nodeId . ' last=' . $last);
      if (in_array($nodeId, $this->data['pgNids'])) {
        $last = $nodeId;
      }
      elseif (!$endFound) {
        if ($last && ($this->checkNodeAccess($last) == 2 )) {
          // The last question we can navigate to is an unanswered question.
          // Therefore we can not navigate to this question.
          //dpm('The last question we can navigate to is an unanswered question.');
          $endFound = TRUE;
        }
        else {
          // last was not a question, or an answered one, so we can navigate
          // to this one.
          $this->addToList($nodeId);
          //dpm('The last question we can navigate to is not a question, or an answered one.');
          if ($this->checkNodeAccess($nodeId) == 2) {
            // last was not a question, or an answered one, so we can navigate
            // to this one. This is an unanswered question, so this is the end.
            $last = $nodeId;
            $endFound = TRUE;
            //dpm('This is an unanswered question');
          }
        }
      }
      //drupal_set_message('fixTreeLinks:: done checking node=' . $nodeId . ' last=' . $last);
      if ($endFound && $nodeId != $last) {
        $brach[$id]['link']['href'] = $this->path . '/goto/' . $last . '/i';
      }
      else {
        $brach[$id]['link']['href'] = $this->path . '/goto/' . $nodeId;
      }
      if ($item['below']) {
        if ($item['link']['mlid'] == $curBookLink['p' . $level]) {
          $brach[$id]['link']['in_active_trail'] = TRUE;
        }
        list($last, $endFound) = $this->fixTreeLinks($brach[$id]['below'], $curBookLink, $level + 1, $last, $endFound);
      }
    }
    return array($last, $endFound);
  }

  /**
   * Implements CqListenerQuestionInterface::FirstSolutionFound().
   */
  public function FirstSolutionFound($tries) {
    // User progressed.
    $clientNode = $this->getNode();
    $this->nextId = FALSE;
    $this->nextAllowed = -1;
    $nextId = $this->getNextId();
    if (!$nextId) {
      // There is no next, case finished!
      $this->userAnswer->setCorrect(1);
    }
    $this->store();
  }

  /**
   * Implements CqListenerQuestionInterface::getExtraFeedbackItems().
   */
  public function getExtraFeedbackItems($caller, $tries) {
    //drupal_set_message('LinearCase:getExtraFeedbackItems()');
    $feedback = array();
    // Check if this implements ClosedQuestion
    $nextPath = $this->getNextPath();
    if ($nextPath) {
      $nextLink = l(t('Continue to the next page'), $nextPath);
      $fbItem = new CqFeedback();
      $fbItem->initWithValues('<b>' . $nextLink . '</b>', 0, 9999);
      $feedback[] = $fbItem;
    }
    return $feedback;
  }

  /**
   * Store any changed data in the userAnswer of the case-node.
   */
  private function store() {
    $this->userAnswer->setAnswer($this->data);
    $this->userAnswer->store();
  }

  /**
   * Reset the progression through the case.
   */
  public function reset() {
    foreach ($this->data['pgNids'] as $nid) {
      if ($nid != $this->node->nid) {
        $node = node_load($nid);
        if (isset($node->question) && $node->question instanceof CqQuestionInterface) {
          $node->question->reset();
        }
      }
    }
    $this->data = array();
    $this->data['pgNids'] = array();
    $this->store();
  }

  /**
   * Implements CqQuestionInterface::isCorrect()
   */
  public function isCorrect() {
    return 0;
  }

  /**
   * Implements CqQuestionInterface::onceCorrect()
   */
  public function onceCorrect() {
    return $this->userAnswer->onceCorrect();
  }

  /**
   * Implements CqQuestionInterface::getTries()
   */
  public function getTries() {
    return 0;
  }

  /**
   * Implements CqQuestionInterface::addListener()
   */
  public function addListener(CqListenerQuestionInterface &$listener) {

  }

  /**
   * Returns a reference to the UserAnswer of this question
   *
   * @return object
   *   cqUserAnswer object.
   */
  public function &getUserAnswer() {
    return $this->userAnswer;
  }

}
