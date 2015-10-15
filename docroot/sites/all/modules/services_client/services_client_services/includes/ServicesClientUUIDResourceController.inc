<?php

/**
 * @file
 * Custom controller to allow handling requests with UUIDs.
 */

class ServicesClientUUIDResourceController extends ServicesEntityResourceController {

  /**
   * Implements ServicesResourceControllerInterface::access().
   */
  public function access($op, $args) {
    if ($op == 'index') {
      // Access is handled per-entity by index().
      return TRUE;
    }
    if ($op == 'create') {
      list($entity_type) = $args;
      // Workaround for bug in Entity API node access.
      // @todo remove once https://drupal.org/node/1780646 lands.
      if ($entity_type === 'node') {
        return isset($args['type']) ? node_access('create', $args['type']) : FALSE;
      }
      // Create an entity from the data and pass this to entity_access(). This
      // allows us to check per-bundle creation rights.
      $entity = entity_create($entity_type, $args);
      $result = entity_access($op, $entity_type, $entity);
      // NULL result means that entity type don't have access check function.
      return ($result !== NULL ? $result : TRUE);
    }
    else {
      // Retrieve, Delete, Update.
      list($entity_type, $entity_id) = $args;
      $entity_id = $this->normalizeEntityId($entity_type, $entity_id);

      $entity = entity_load_single($entity_type, $entity_id);

      // Pass the entity to the access control.
      $result = entity_access($op, $entity_type, $entity ? $entity : NULL);
      // NULL result means that entity type don't have access check function.
      return ($result !== NULL ? $result : TRUE);
    }
  }

  /**
   * Implements ServicesResourceControllerInterface::retrieve().
   */
  public function retrieve($entity_type, $entity_id, $fields, $revision) {
    // Retrieve local entity id.
    $entity_id = $this->normalizeEntityId($entity_type, $entity_id);
    if (empty($entity_id)) {
      services_error('Entity or revision not found', 404);
    }
    return parent::retrieve($entity_type, $entity_id, $fields, $revision);
  }

  /**
   * Implements ServicesResourceControllerInterface::update().
   */
  public function update($entity_type, $entity_id, array $values) {
    // Get local entity id.
    $entity_id = $this->normalizeEntityId($entity_type, $entity_id);
    if (empty($entity_id)) {
      services_error('Entity or revision not found', 404);
    }
    // Merge entity id with saved values.
    $info = entity_get_info($entity_type);
    $values[$info['entity keys']['id']] = $entity_id;
    // Run save action.
    return parent::update($entity_type, $entity_id, $values);
  }

  /**
   * Implements ServicesResourceControllerInterface::delete().
   */
  public function delete($entity_type, $entity_id) {
    // Retrieve local entity id.
    $entity_id = $this->normalizeEntityId($entity_type, $entity_id);
    if (empty($entity_id)) {
      services_error('Entity or revision not found', 404);
    }
    parent::delete($entity_type, $entity_id);
  }

  /**
   * Implements ServicesResourceControllerInterface::index().
   */
  public function index($entity_type, $fields, $parameters, $page, $pagesize, $sort, $direction) {
    // Make sure the pagesize is not too large.
    $max_pagesize = variable_get('services_entity_max_pagesize', 100);
    $pagesize = ($max_pagesize < $pagesize) ? $max_pagesize : $pagesize;

    // Build an EFQ based on the arguments.
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', $entity_type)
        ->range($page * $pagesize, $pagesize);

    if (!empty($parameters)) {
      foreach ($parameters as $field => $value) {
        $this->propertyQueryOperation($entity_type, $query, 'Condition', $field, $value);
      }
    }
    if ($sort != '') {
      $direction = ($direction == 'DESC') ? 'DESC' : 'ASC'; // Ensure a valid direction
      $this->propertyQueryOperation($entity_type, $query, 'OrderBy', $sort, $direction);
    }

    $result = $query->execute();

    if (empty($result)) {
      return services_error(t('No entities found.'), 404);
    }
    // Convert to actual entities.
    $entities = entity_load($entity_type, array_keys($result[$entity_type]));

    foreach ($entities as $id => $entity) {
      $access = entity_access('view', $entity_type, $entity);
      if ($access !== FALSE) {
        // Users get special treatment to remove sensitive data.
        if ($entity_type == 'user') {
          // Use the helper that Services module already has.
          services_remove_user_data($entity);
        }

        $return[] = $this->limit_fields($entity, $fields);
      }
    }

    // The access check may have resulted in there being no entities left.
    if (empty($return)) {
      return services_error(t('No entities found.'), 404);
    }

    return $return;
  }

  /**
   * Normalize entity id, if in UUID format, convert to local id.
   *
   * @param string $entity_type
   *   Entity type.
   *
   * @param string $entity_id
   *   Entity id.
   *
   * @return int
   *   Local entity id.
   */
  protected function normalizeEntityId($entity_type, $entity_id) {
    if ($this->validUUID($entity_id)) {
      if (function_exists('uuid_find')) {
        $info = entity_get_info($entity_type);
        $entity_id = uuid_find($entity_id, $info['base table'], $info['entity keys']['id'], 'uuid');
      }
      else {
        $result = entity_get_id_by_uuid($entity_type, array($entity_id));
        $entity_id = reset($result);
      }
    }

    return $entity_id;
  }

  /**
   * Check if id is valid UUID string.
   *
   * @param string $id
   *   Checked ID param.
   *
   * @return bool
   *   TRUE if valid.
   */
  protected function validUUID($id) {
    return preg_match('/^[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}$/i', $id);
  }
}
