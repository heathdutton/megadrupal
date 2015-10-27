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
   * Overrides \ComstackRestfulEntityBase::isValidEntity().
   */
  protected function isValidEntity($op, $entity_id) {
    $entity_type = $this->entityType;

    $params = array(
      '@id' => $entity_id,
      '@resource' => $this->getPluginKey('label'),
    );

    if (!$entity = user_relationships_entity__load($entity_id, $this->getAccount())) {
      if (!$this->isListRequest()) {
        throw new RestfulNotFoundException(format_string('The entity ID @id for @resource does not exist.', $params));
      }
      else {
        return FALSE;
      }
    }

    list(,, $bundle) = entity_extract_ids($entity_type, $entity);

    $resource_bundle = $this->getBundle();
    if ($resource_bundle && $bundle != $resource_bundle) {
      throw new RestfulUnprocessableEntityException(format_string('The entity ID @id is not a valid @resource.', $params));
    }

    if ($this->checkEntityAccess($op, $entity_type, $entity) === FALSE) {

      if ($op == 'view' && !$this->getPath()) {
        // Just return FALSE, without an exception, for example when a list of
        // entities is requested, and we don't want to fail all the list because
        // of a single item without access.
        return FALSE;
      }

      // Entity was explicitly requested so we need to throw an exception.
      throw new RestfulForbiddenException(format_string('You do not have access to entity ID @id of resource @resource', $params));
    }

    return TRUE;
  }

  /**
   * Overrides \ComstackRestfulEntityBase::viewEntity().
   */
  public function viewEntity($id) {
    $entity_id = $this->getEntityIdByFieldId($id);
    $request = $this->getRequest();

    $cached_data = $this->getRenderedCache($this->getEntityCacheTags($entity_id));
    if (!empty($cached_data->data)) {
      return $cached_data->data;
    }

    if (!$this->isValidEntity('view', $entity_id)) {
      return;
    }

    $entity = user_relationships_entity__load($entity_id, $this->getAccount());
    $wrapper = entity_metadata_wrapper($this->entityType, $entity);
    $wrapper->language($this->getLangCode());
    $values = array();

    $limit_fields = !empty($request['fields']) ? explode(',', $request['fields']) : array();

    foreach ($this->getPublicFields() as $public_field_name => $info) {
      if ($limit_fields && !in_array($public_field_name, $limit_fields)) {
        // Limit fields doesn't include this property.
        continue;
      }

      $value = NULL;

      if ($info['create_or_update_passthrough']) {
        // The public field is a dummy one, meant only for passing data upon
        // create or update.
        continue;
      }

      if ($info['callback']) {
        $value = static::executeCallback($info['callback'], array($wrapper));
      }
      else {
        // Exposing an entity field.
        $property = $info['property'];
        $sub_wrapper = $info['wrapper_method_on_entity'] ? $wrapper : $wrapper->{$property};

        // Check user has access to the property.
        if ($property && !$this->checkPropertyAccess('view', $public_field_name, $sub_wrapper, $wrapper)) {
          continue;
        }

        if (empty($info['formatter'])) {
          if ($sub_wrapper instanceof EntityListWrapper) {
            // Multiple values.
            foreach ($sub_wrapper as $item_wrapper) {
              $value[] = $this->getValueFromProperty($wrapper, $item_wrapper, $info, $public_field_name);
            }
          }
          else {
            // Single value.
            $value = $this->getValueFromProperty($wrapper, $sub_wrapper, $info, $public_field_name);
          }
        }
        else {
          // Get value from field formatter.
          $value = $this->getValueFromFieldFormatter($wrapper, $sub_wrapper, $info);
        }

        // Force the PHP variable type, if the type has been set in the field
        // or property info.
        if (!is_null($value)) {
          $item_info = $sub_wrapper->info();
          $item_type = !empty($item_info['type']) ? $item_info['type'] : NULL;

          if (isset($info['wrapper_method']) && $info['wrapper_method'] === 'getIdentifier' || $item_type === 'integer') {
            $value = (int) $value;
          }
          elseif ($item_type === 'boolean') {
            $value = (boolean) $value;
          }
          elseif ($item_type === 'decimal') {
            $value = (float) $value;
          }
        }
      }

      if ($value && $info['process_callbacks']) {
        foreach ($info['process_callbacks'] as $process_callback) {
          $value = static::executeCallback($process_callback, array($value));
        }
      }

      $values[$public_field_name] = $value;
    }

    $this->setRenderedCache($values, $this->getEntityCacheTags($entity_id));
    return $values;
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
      $controller = entity_get_controller($this->entityType);
      $controller->setAccount($account);
      $relationship = $controller->request($request_data['user'], $bundle);
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
    $controller = entity_get_controller($this->entityType);
    $controller->setAccount($account);
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
