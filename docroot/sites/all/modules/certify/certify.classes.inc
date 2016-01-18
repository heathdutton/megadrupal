<?php

/**
 * @file 
 * certify.classes.inc php file
 * Classes for the Drupal module Certify.
 */

abstract class CertifyCondition {

  public $cid; // certificate node
  public $vid; // certificate node revision
  public $condtype; // condition type
  public $condnode; // condition node
  public $condnodev; // condition node revision

  public $mynode;

  public $started;
  public $completed;

  public $gotpoints;
  public $gotpointscapped;
  public $totalpoints;
  public $requiredpoints;

  public function __construct($dbobj) {
    $this->cid = $dbobj->cid;
    $this->vid = $dbobj->vid;
    $this->condtype = $dbobj->condtype;
    $this->condnode = $dbobj->condnode;
    $this->condnodev = $dbobj->condnodev;
    $this->mynode = NULL;
  }

  public static function getViewHeader() {
    return "";
  }

  public abstract function fillProgress($uid = 0);

  public function loadConditionNode() {
    if (!$this->mynode) {
      $this->mynode = node_load(NULL, $this->condnodev);
    }
    
    return $this->mynode;
  }

  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  public function certify_delete($node) { }

}

/**
 * CLASS Certify
 */

class Certify {

  public $conditions;
  private $status;

  /**
   * General constructor for the Certify class
   */
  public function __construct($node) {
    $condtypes = certify_conditiontypes();
    $this->conditions = array();
    $rs = db_query("SELECT cc.cid, cc.vid, cc.condtype, cc.condnode, n.vid AS condnodev FROM {certify_conditions} cc LEFT JOIN {node} n ON n.nid = cc.condnode WHERE cc.vid = :node_revision_id", array(':node_revision_id' => $node->vid));
    foreach ($rs as $data) {
      if (isset($condtypes["certify_" . $data->condtype])) { // If we have a class registered
        $classname = $condtypes["certify_" . $data->condtype];
        $this->conditions[] = new $classname($data);
      }
    }
    $this->status = FALSE;
  }

  public function delete($node) {
    foreach ($this->conditions as &$cond) {
      $cond->certify_delete($node);
    }

  }

  /**
   * Returns an assosiative array with condition types as keys, and each value an array of those conditions
   */
  public function getConditionsByType() {
    $ret = array();
    foreach ($this->conditions as &$cond) {
      if (!isset($ret[$cond->condtype])) {
        $ret[$cond->condtype] = array();
      }
      $ret[$cond->condtype][] = $cond;
    }
    return $ret;
  }

  /**
   * For a given node, return array of nids for a specific condition type (or all)
   */
  public static function getConditionsNidsFromNode($node, $type = NULL) {
    if (!isset($node->certificate)) {
      return array();
    }
    else {
      return $node->certificate->getConditionsNids($type);
    }
  }

  /**
   * Return array of nids for a specific condition type (or all)
   */
  public function getConditionsNids($type = NULL) {
    $conds = array();

    foreach ($this->conditions as &$cond) {
      if ($type && $type == $cond->condtype) {
        $conds[] = $cond->condnode;
      }
      elseif (!$type) {
        $conds[] = $cond->condnode;
      }
    }
    return $conds;
  }

  /**
   * Returns the status object for this certificate
   * If the $uid passed is different from the logged in user, the content is not cached.
   */
  public function getStatus($uid = 0, $notcurrent = FALSE) {
    global $user;

    if (!$uid) {
      $uid = $user->uid;
    }

    if ($this->status && $uid == $user->uid) {
      return $this->status;
    }

    $status = (object) array(
      'started' => FALSE,
      'completed' => TRUE,
      'gotpoints' => 0,
      'gotpointscapped' => 0,
      'totalpoints' => 0,
      'requiredpoints' => 0,
      'progress' => 0,
    );

    foreach ($this->conditions as &$condition) {
      $condition->fillProgress($uid);
      $status->gotpoints += $condition->gotpoints;
      $status->gotpointscapped += $condition->gotpointscapped;
      $status->totalpoints += $condition->totalpoints;
      $status->requiredpoints += $condition->requiredpoints;
      if ($condition->started) {
        $status->started = TRUE;
      }
      if (!$condition->completed) {
        $status->completed = FALSE;
      }
    }

    if ($status->requiredpoints) {
      $status->progress = floor($status->gotpointscapped * 100 / $status->requiredpoints);
    }

    if ($status->progress > 100) { // Should not really be possible, but better safe than sorry
      $status->progress = 100;
    }

    if ($uid == $user->uid) {
      $this->status = $status;
    }
    return $status;

  }

  /**
   * Handles a change from a condition
   *
   * @param $user
   *  The user that got the change
   */
  public function scoreChanged($uid = NULL) {
    global $user;

    if (!$uid) {
      $uid = $user->uid;
    }

    // NOP
  }

  /**
   *  Function used to collect an array of certificate nodes
   *
   * @param $nids
   *   An array of nids to fetch. If not given, all available certificates are fetched.
   * @param $uid
   *   An user ID to filter by. This goes largely ignored for now.
   *
   * @return
   *   Returns an array of certificate nodes, with vid (revision id) as key.
   */
  public static function getCertificates($nids = NULL, $uid = NULL) {
    global $user;

    $certificates = array();

    if (!$uid) {
      $uid = $user->uid;
    }

    if (!is_array($nids)) {
      // Populate certificates
      $cres = db_query("SELECT vid FROM {node} n WHERE n.type = 'certificate'");
      if (!$cres) {
        return $certificates;
      }
      foreach ($cres as $cert) {
        $certnode = node_load(NULL, $cert->vid);
        if ($certnode) {
          $certificates[$cert->vid] = $certnode;
        }
      }
    }
    else {
      foreach ($nids as $nid) {
        $cert = node_load($nid);
        if ($cert) {
          $certificates[$cert->vid] = $cert;
        }
      }
    }

    return $certificates;
  }
}

/**
 * Certify (certify://) stream wrapper class.
 *
 * Provides storage for certificates.
 */
class CertifyCertificateStreamWrapper extends DrupalLocalStreamWrapper {
  /**
   * Implements abstract public function getDirectoryPath().
   */
  public function getDirectoryPath() {
    return variable_get('certify_certificate_path', '/var/drupal-content/certify');
  }
  
  /**
   * Overrides getExternalUrl().
   */
  public function getExternalUrl() {
    return url('/', array('absolute' => TRUE));
  }
}
