<?php

/**
 * @file
 * Contains UserRelationshipsController
 *
 * This is some backwards stuff, the entity controller will be implementing the
 * CRUD methods laid out by user_relationships.module
 */

/**
 * UserRelationshipsController
 * The Entity controller for a relationship.
 */
class UserRelationshipsController extends EntityAPIController {
  public $account;
  public $reason;

  /**
   * Overridden.
   * @see EntityAPIController#__construct()
   */
  public function __construct($entityType = 'user_relationships', $account = NULL) {
    parent::__construct($entityType);

    if (!$account) {
      global $user;
      $account = user_load($user->uid);
    }
    $this->setAccount($account);

    // We force set the bundle key as the user relationship type whilst
    // exportable isn't a full entity type.
    //$this->bundleKey = 'type';
  }

  /**
   * Set the current user account.
   */
  public function setAccount($account) {
    $this->account = $account;
  }

  /**
   * Get the current user account.
   */
  public function getAccount() {
    return $this->account;
  }

  /**
   * Set the reason for deletion.
   */
  public function setReason($reason) {
    $this->reason = $reason;
  }

  /**
   * Get the reason.
   */
  public function getReason() {
    return $this->reason;
  }

  /**
   * Overrides EntityAPIController::buildQuery().
   */
  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    $query = parent::buildQuery($ids, $conditions, $revision_id);

    // Load queries will be from the current users perspective.
    $account = $this->getAccount();
    $query->condition('requester_id', $account->uid)
      ->addMetaData('account', $account);

    return $query;
  }

  /**
   * Request a new relationship with another user.
   */
  public function request($target_user, $type_name, $source_user = NULL) {
    $type = user_relationships_type_load(array('machine_name' => $type_name));
    if (!$type) {
      throw new Exception(t('No such relationship type.'));
    }

    if (!$source_user) {
      $source_user = $this->getAccount();

      if (!$source_user) {
        global $user;
        user_load($user->uid);
      }
    }

    $existing_relationship = user_relationships_load(array('rtid' => $type->rtid, 'between' => array($source_user->uid, $target_user)));
    // If there is already an existing relationship, return it.
    if (!empty($existing_relationship)) {
      return current($existing_relationship);
    }

    $ret = user_relationships_request_relationship($source_user, $target_user, $type);
    if (!$ret) {
      throw new Exception(t('Unknown failure or permission denied'));
    }
    elseif (!is_object($ret)) {
      throw new Exception($ret);
    }

    // Make sure that the type is set.
    if (empty($ret->type)) {
      $ret->type = $type_name;
    }

    return $ret;
  }

  /**
   * Accept/approve a relationship.
   */
  function accept($rid, $account = NULL) {
    if (!$account) {
      global $user;
      $account = user_load($user->uid);
    }

    try {
      $rels = user_relationships_load(array('rid' => $rid , 'requestee_id' => $account->uid, 'approved' => 0));
      if (!$rels || !is_array($rels) || count($rels) != 1) {
        throw new Exception('User relationship load failed');
      }

      $rel = array_shift($rels);
      if ($rel->requestee_id != $account->uid) {
        throw new Exception('Access denied');
      }

      user_relationships_save_relationship($rel, 'approve');
      return $rel;
    }
    catch (Exception $ex) {
      return services_error(t('Error approving relationship: @msg', array('@msg' => $ex->getMessage())));
    }
  }

  /**
   * Overridden.
   * @see EntityAPIController#delete()
   */
  public function delete($ids) {
    if ($account = $this->getAccount()) {
      global $user;
      $account = user_load($user->uid);
    }

    $reason = $this->getReason();

    if (empty($ids) || !$account || !$reason) {
      return FALSE;
    }

    // Do it.
    foreach ($ids as $id) {
      $rels = user_relationships_load(array('rid' => $id , 'user' => $account->uid));
      if (!$rels || !is_array($rels) || count($rels) != 1) {
        throw new Exception('User relationship load failed');
      }

      $rel = array_shift($rels);
      if ($rel->requestee_id != $account->uid && $rel->requester_id != $account->uid) {
        throw new Exception('Access denied');
      }

      user_relationships_delete_relationship($rel, $account, $reason);
    }
  }
}
