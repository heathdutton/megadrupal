<?php

/**
 * @file
 * Contains ComstackFriendsRestfulBase.
 */

class ComstackFriendsRestfulBase extends \ComstackRestfulEntityBase {
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
    );
  }

  /**
   * Overrides \RestfulEntityBase::getQueryForList().
   *
   * Only expose conversations which haven't been deleted.
   */
  public function getQueryForList() {
    $query = parent::getQueryForList();
    $account = $this->getAccount();

    // Grab relationships from the current users point of view.
    $query->propertyCondition('requester_id', $account->uid);

    return $query;
  }

  /**
   * Overrides \RestfulDataProviderEFQ::getQueryCount().
   */
  public function getQueryCount() {
    $query = parent::getQueryCount();
    $account = $this->getAccount();

    // Grab relationships from the current users point of view.
    $query->propertyCondition('requester_id', $account->uid);

    return $query->count();
  }

  /**
   * Overrides \RestfulEntityBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    // Reorder things.
    $id_field = $public_fields['id'];
    unset($public_fields['id']);

    $public_fields['type'] = array(
      'callback' => array('\RestfulManager::echoMessage', array('relationship')),
    );

    $public_fields['id'] = $id_field;

    $public_fields['relationship_type'] = array(
      'property' => 'type',
    );

    $public_fields['user'] = array(
      'property' => 'requestee_id',
      'resource' => array(
        'user' => array(
          'name' => 'cs/users',
          'full_view' => TRUE,
        ),
      ),
    );

    $public_fields['created'] = array(
      'property' => 'created',
      'process_callbacks' => array(
        'date_iso8601',
      ),
    );

    $public_fields['changed'] = array(
      'property' => 'changed',
      'process_callbacks' => array(
        'date_iso8601',
      ),
    );

    // Remove fields we don't want.
    unset($public_fields['label']);
    unset($public_fields['self']);

    return $public_fields;
  }

  /**
   * Check whether or not a user can perform an action against the API.
   *
   * @param $op
   *   The operation. Allowed values are "view", "request", "approve" and
   *   "delete".
   * @param $entity_type
   *   The entity type.
   * @param $entity
   *   The entity object.
   *
   * @return bool
   *   TRUE or FALSE based on the access. If no access is known about the entity
   *   return NULL.
   */
  protected function checkEntityAccess($op, $entity_type = NULL, $entity = NULL) {
    $account = $this->getAccount();
    $bundle = $this->bundle;

    switch ($op) {
      case 'view':
        return user_access("view own $bundle relationships", $account);
        break;

      case 'request':
        return user_access("can request $bundle relationships", $account);
        break;

      case 'approve':
        return user_access("can have $bundle relationships", $account);
        break;

        // Delete is synonymous with reject.
      case 'delete':
        return user_access("delete $bundle relationships", $account);
        break;
    }
  }

  /**
   * Create a new relationship request.
   */
  public function newRequest() {
    // Check access.
    $account = $this->getAccount();
    $bundle = $this->bundle;

    if (!$this->checkEntityAccess('request')) {
      $params = array('@bundle' => $bundle);
      $this->setHttpHeaders('Status', 403);
      throw new RestfulForbiddenException(format_string('You do not have access to request new @bundle relationships.', $params));
    }

    $request_data = $this->getRequestData();

    // Validate the request has all the data we need.
    if (empty($request_data['user']) || !empty($request_data['user']) && !ctype_digit((string) $request_data['user'])) {
      $this->setHttpHeaders('Status', 400);
      throw new \RestfulBadRequestException(format_string("When requesting a relationship with someone you need to send a valid user ID. @data", array('@data' => print_r($request_data, TRUE) )));
    }

    try {
      $relationship = entity_get_controller($this->entityType, $account)->request($request_data['user'], $bundle);
    }
    catch (Exception $e) {
      $this->setHttpHeaders('Status', 400);
      throw new RestfulException(format_string('An error occurred, @message.', array('@message' => $e->getMessage())));
    }

    if ($relationship) {
      $this->setHttpHeaders('Status', 201);
    }

    return $this->viewEntity($relationship->rid);
  }

  /**
   * Delete a relationship given a reason.
   */
  public function deleteRelationship($rid, $reason) {
    $account = $this->getAccount();
    $controller = entity_get_controller($this->entityType, $account);
    $controller->setReason($reason);
    $controller->delete(array($rid));
  }

  /**
   * Delete or reject a relationship.
   *
   * "reason" should be one of: cancel, disapprove, remove.
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
}
