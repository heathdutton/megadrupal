<?php

/**
 * @file
 * Contains ComstackFriendsFriendsResource__1_0.
 */

class ComstackFriendsFriendsResource__1_0 extends \ComstackFriendsRestfulBase {
  /**
   * Overrides \RestfulEntityBase::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      // Listings.
      '' => array(
        \RestfulInterface::GET => 'getList',
        \RestfulInterface::POST => 'newRequest',
      ),
      // A specific entity.
      '^([\d]+)$' => array(
        \RestfulInterface::GET => 'viewEntity',
        \RestfulInterface::DELETE => 'deleteEntity',
      ),
      // Actions against a specific relationship.
      '^([\d]+)\/approve$' => array(
        \RestfulInterface::PUT => 'approveEntity',
      ),
      '^([\d]+)\/reject$' => array(
        \RestfulInterface::PUT => 'rejectEntity',
      ),
      '^([\d]+)\/cancel' => array(
        \RestfulInterface::PUT => 'cancelEntity',
      ),
    );
  }

  /**
   * Overrides \ComstackFriendsRestfulBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['approved'] = array(
      'property' => 'approved',
    );

    return $public_fields;
  }

  /**
   * Approve a relationship request.
   *
   * @throws RestfulForbiddenException.
   * @throws RestfulException.
   */
  public function approveEntity() {
    // Check access.
    $account = $this->getAccount();
    $bundle = $this->bundle;

    if (!$this->checkEntityAccess('approve')) {
      $params = array('@bundle' => $bundle);
      $this->setHttpHeaders('Status', 403);
      throw new RestfulForbiddenException(format_string('You do not have access to approve @bundle relationships.', $params));
    }

    $entity_id = $this->getEntityID();

    // Approve the relationship.
    try {
      if (entity_get_controller($this->entityType, $account)->accept($entity_id)) {
        $this->clearResourceRenderedCacheEntity($entity_id);
        return $this->viewEntity($entity_id);
      }
    }
    catch (Exception $e) {
      $this->setHttpHeaders('Status', 400);
      throw new RestfulException(format_string('An error occurred, @message.', array('@message' => $e->getMessage())));
    }

    return FALSE;
  }

  /**
   * Reject a relationship request.
   */
  public function rejectEntity() {
    // Check access.
    $account = $this->getAccount();
    $bundle = $this->bundle;

    if (!$this->checkEntityAccess('delete')) {
      $params = array('@bundle' => $bundle);
      $this->setHttpHeaders('Status', 403);
      throw new RestfulForbiddenException(format_string('You do not have access to delete or reject new @bundle relationships.', $params));
    }

    $entity_id = $this->getEntityID();

    // Grab the controller.
    try {
      $this->deleteRelationship($entity_id, 'disapprove');
    }
    catch (Exception $e) {
      $this->setHttpHeaders('Status', 400);
      throw new RestfulException(format_string('An error occurred, @message.', array('@message' => $e->getMessage())));
    }
  }

  /**
   * Delete a relationship.
   */
  public function deleteEntity($entity_id) {
    // Check access.
    $account = $this->getAccount();
    $bundle = $this->bundle;

    if (!$this->checkEntityAccess('delete')) {
      $params = array('@bundle' => $bundle);
      $this->setHttpHeaders('Status', 403);
      throw new RestfulForbiddenException(format_string('You do not have access to delete or reject new @bundle relationships.', $params));
    }

    try {
      $this->deleteRelationship($entity_id, 'remove');
    }
    catch (Exception $e) {
      $this->setHttpHeaders('Status', 400);
      throw new RestfulException(format_string('An error occurred, @message.', array('@message' => $e->getMessage())));
    }
  }

  /**
   * Cancel an unapproved request.
   */
  public function cancelEntity() {
    // Check access.
    $account = $this->getAccount();
    $bundle = $this->bundle;

    if (!$this->checkEntityAccess('delete')) {
      $params = array('@bundle' => $bundle);
      $this->setHttpHeaders('Status', 403);
      throw new RestfulForbiddenException(format_string('You do not have access to delete or reject new @bundle relationships.', $params));
    }

    $entity_id = $this->getEntityID();

    // Grab the controller.
    try {
      $this->deleteRelationship($entity_id, 'cancel');
    }
    catch (Exception $e) {
      $this->setHttpHeaders('Status', 400);
      throw new RestfulException(format_string('An error occurred, @message.', array('@message' => $e->getMessage())));
    }
  }
}
