<?php

/**
 * @file
 * Definition of SalsaController.
 */


/**
 * Generic Salsa API controller.
 */
class SalsaController extends EntityAPIController implements EntityAPIControllerInterface {

  protected $salsaType;

  /**
   * A id mapping list.
   *
   * Map a requested salsa object ID to a different one. A typical use case is
   * trying to connect to a different organisation (e.g. a staging environment)
   * that has not the same ids.
   *
   * @var type
   */
  protected $mappings = array();

  public function __construct($entityType) {
    parent::__construct($entityType);
    $this->salsaType = str_replace('salsa_', '', $entityType);
    $mappings = variable_get('salsa_entity_id_mappings', array());
    if (isset($mappings[$this->salsaType])) {
      $this->mappings = $mappings[$this->salsaType];
    }
  }

  /*
   * Overridden EntityAPIController::query().
   */
  public function query($ids = array(), $conditions = array(), $revision_id = NULL) {
    if (!empty($ids)) {
      $conditions[$this->salsaType . '_KEY'] = $ids;
    }

    // Apply ID mapping.
    if (!empty($conditions[$this->salsaType . '_KEY'])) {

      if (!is_array($conditions[$this->salsaType . '_KEY'])) {
        $conditions[$this->salsaType . '_KEY'] = array($conditions[$this->salsaType . '_KEY']);
      }
      foreach ($conditions[$this->salsaType . '_KEY'] as &$id) {
        // @todo: Support mappings for conditions with #operators.
        if (is_scalar($id)) {
          if (isset($this->mappings[$id])) {
            $id = $this->mappings[$id];
          }
        }
      }
    }

    // Handle pseudo conditions.
    $limit = NULL;
    $orderBy = array();
    if (isset($conditions['#limit'])) {
      $limit = $conditions['#limit'];
      unset($conditions['#limit']);
    }
    if (isset($conditions['#order'])) {
      $orderBy= $conditions['#order'];
      unset($conditions['#order']);
    }

    try {
      $objects = salsa_api()->getObjects($this->salsaType, $conditions, $limit, array(), $orderBy);
    }
    catch (SalsaQueryException $e) {
      // Something went wrong with the salsa API call, log the error and return
      // an empty result set.
      watchdog_exception('salsa entity load', $e);
      return array();
    }
    $entities = array();
    // Salsa entities always have a entity class.
    $entity_class = $this->entityInfo['entity class'];
    foreach ($objects as $id => $object) {
      if (!empty($id)) {
        $values = (array)$object;
        // Filter out unecessary stuff.
        foreach ($values as $value_key => $value) {
          if (strpos($value_key, 'BOOLVALUE') !== FALSE) {
            unset($values[$value_key]);
          }
        }

        // Revert a possible ID mapping.
        $original_id = array_search($id, $this->mappings) ?: $id;
        $values[$this->idKey] = $original_id;
        $values['key'] = $original_id;
        $entities[$original_id] = new $entity_class($values, $this->entityType);
      }
    }

    return $entities;
  }

  /*
   * Overridden EntityAPIController::delete().
   */
  public function delete($ids) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }

    $query_info = array(
      '#script' => 'delete',
      '#tables' => $this->salsaType,
      '#fields' => $ids,
    );
    salsa_api_query($query_info);

    foreach ($entities as $id => $entity) {
      $this->invoke('delete', $entity);
    }
  }

  /*
   * Overridden EntityAPIController::save().
   */
  public function save($entity) {
    $this->invoke('presave', $entity);
    $fields = array();
    $info = entity_get_property_info($this->entityType);
    $property_keys = array_keys($info['properties']);
    // Always allow organization_key and key, which seems to be required
    // sometimes.
    $property_keys[] = 'organization_KEY';
    $property_keys[] = 'key';
    foreach ($property_keys as $key) {
      if (property_exists($entity, $key)) {
        // @todo: Find a better place to put this.
        if ($key == 'Response') {
          $fields['Response_Question_' . $entity->questionnaire_question_KEY] = $entity->$key;
        }
        else {
          $fields[$key] = $entity->$key;
        }

        // The date format that Salsa send is not understood by them.
        // Convert all dates back into a format in the original timezone, but
        // without timezone information.
        if (isset($info['properties'][$key]['type']) && $info['properties'][$key]['type'] == 'date' && !empty($fields[$key])) {
          $fields[$key] = salsa_entity_convert_date($fields[$key]);
        }
        // Boolean values have to be saved as TRUE or FALSE.
        if (isset($info['properties'][$key]['type']) && $info['properties'][$key]['type'] == 'boolean') {
          if ($entity->$key === 'true') {
            $fields[$key] = TRUE;
          }
          if ($entity->$key === 'false') {
            $fields[$key] = FALSE;
          }
        }
        // Integer values have to be saved as 1 or 0.
        if (isset($info['properties'][$key]['type']) && $info['properties'][$key]['type'] == 'integer') {
          if ($entity->$key === 'true') {
            $fields[$key] = 1;
          }
          if ($entity->$key === 'false') {
            $fields[$key] = 0;
          }
        }
      }
      else {
        // Provide a default value for Date_Created.
        if ($key == 'Date_Created') {
          // @todo: Add a timezone setting to enforce the timezone.
          $fields[$key] =  salsa_entity_convert_date('NOW');
        }
      }
    }
    // Support saving/passing additional fields to /save which are not
    // properties.
    if (!empty($entity->additional)) {
      $fields += $entity->additional;
    }

    // Prevent Your password does not match error for supporters.
    // This does not delete the password, it just does not update it.
    unset($fields['Password']);

    // Check if this is a new entity.
    $key = $this->salsaType . '_KEY';
    $is_new = (!isset($entity->$key));

    // Save the entity.
    $returned_key = salsa_api()->save($this->salsaType, $fields);

    // Update caches.
    if (!$is_new) {
      DrupalDefaultEntityController::resetCache(array($entity->$key));
    }

    // Update with calculated/updated data, e.g. keys, modification dates,
    // default values and so on.
    $updated_entities = $this->load(array($returned_key));
    $updated_entity = reset($updated_entities);
    foreach ($updated_entity as $key => $value) {
      $key = trim($key);
      $entity->$key = $value;
    }

    // Invoke the insert or update hook.
    if ($is_new) {
      $this->invoke('insert', $entity);
      return SAVED_NEW;
    }
    else {
      $this->invoke('update', $entity);
      return SAVED_UPDATED;
    }
  }

  /**
   * Overrides DrupalDefaultEntityControlle::cacheGet().
   *
   * Fixes a bug when trying to load all entities multiple times.
   */
  protected function cacheGet($ids, $conditions = array()) {
    $entities = array();
    // Load any available entities from the internal cache.
    if (!empty($this->entityCache)) {
      if ($ids) {
        $entities += array_intersect_key($this->entityCache, array_flip($ids));
      }
      // If loading entities only by conditions, fetch all available entities
      // from the cache. Entities which don't match are removed later.
      // CHANGED: Also load all entities when the cache is complete.
      elseif ($conditions || $this->cacheComplete) {
        $entities = $this->entityCache;
      }
    }

    // Exclude any entities loaded from cache if they don't match $conditions.
    // This ensures the same behavior whether loading from memory or database.
    if ($conditions) {
      foreach ($entities as $key => $entity) {
        $entity_values = (array) $entity;
        foreach ($conditions as $condition_key => $condition_value) {
          if (is_array($condition_value)) {
            if (!isset($entity_values[$condition_key])) {
              unset($entities[$key]);
            }
            elseif (!in_array($entity_values[$condition_key], $condition_value)) {
              unset($entities[$key]);
            }
          }
          elseif (!isset($entity_values[$condition_key]) || $entity_values[$condition_key] != $condition_value) {
            unset($entities[$key]);
          }
        }
      }
    }
    return $entities;
  }

}
