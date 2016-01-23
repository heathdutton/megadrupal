<?php

/**
 * @file
 * Defines entity class and controller class for MongoDB documents
 */

/**
 * A class for MongoDB backed entities
 */
class MongoEntity extends Entity {

  /**
   * {@inheritdoc}
   */
  public function __construct(array $values, $entity_type = NULL) {

    if (empty($entity_type)) {
      throw new Exception('Cannot create an instance of Entity without a specified entity type.');
    }
    $this->entityType = $entity_type;
    $this->setUp();

    // Set initial values.
    foreach ($values as $key => $value) {
      $this->{$key} = $value;
    }

  }

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();
    $this->labelKey = isset($this->entityInfo['entity keys']['label']) ? $this->entityInfo['entity keys']['label'] : NULL;
  }

  /**
   * A convenience function for entity forms. This may be removed in the future.
   * @todo Would it make more sense to put this in MongoEntityController?
   */
  public function hook_form($form, &$form_state) {

    // @todo don't show name field if no name
    // @todo check why nameKey isn't populated
    if (!empty($this->labelKey)) {
      $form[$this->labelKey] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => isset($this->{$this->labelKey}) ? $this->{$this->labelKey} : '',
        '#weight' => -100,
      );
    }

    field_attach_form($this->entityType, $this, $form, $form_state);

    $form['actions'] = array(
      '#type' => 'actions',
      '#weight' => 100,
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
    );

    $form_state['entity'] = $this;

    return $form;

  }

  /**
   * A convenience function for validating entity forms.
   */
  public function hook_form_validate($form, $form_state) {
    field_attach_validate($this->entityType, $this, $form, $form_state);
  }

  /**
   * A convenience function for submitting entity forms.
   */
  public function hook_form_submit($form, $form_state) {

    entity_form_submit_build_entity($this->entityType, $this, $form, $form_state);

    $this->save();
    // @todo redirect
  }

}

/**
 * A controller for CRUD entity operations with MongoDB.
 *
 * @todo Consider overriding DrupalDefaultEntityController instead
 */
class MongoEntityController extends EntityAPIController {

  /**
   * A convenience method for MongoEntityControllers to provide menu callbacks.
   */
  public static function hook_menu() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function __construct($entity_type) {

    // DrupalDefaultEntityController
    $this->entityType = $entity_type;
    $this->entityInfo = entity_get_info($entity_type);
    $this->entityCache = array();
    $this->hookLoadArguments = array();
    $this->idKey = $this->entityInfo['entity keys']['id'];

    // Check if the entity type supports revisions.
    if (!empty($this->entityInfo['entity keys']['revision'])) {
      $this->revisionKey = $this->entityInfo['entity keys']['revision'];
    }
    else {
      $this->revisionKey = FALSE;
    }

    // EntityAPIController
    // Check if the entity type supports static caching of loaded entities.
    $this->cache = !empty($this->entityInfo['static cache']);

    if (isset($this->entityInfo['bundle of'])) {
      $info = entity_get_info($this->entityInfo['bundle of']);
      $this->bundleKey = $info['bundle keys']['bundle'];
    }
    $this->defaultRevisionKey = !empty($this->entityInfo['entity keys']['default revision']) ? $this->entityInfo['entity keys']['default revision'] : 'default_revision';

    // MongoEntityController
    $this->nameKey = isset($this->entityInfo['entity keys']['name']) ? $this->entityInfo['entity keys']['name'] : NULL;
    $this->bundleKey = isset($this->entityInfo['entity keys']['bundle']) ? $this->entityInfo['entity keys']['bundle'] : NULL;
    $this->labelKey = isset($this->entityInfo['entity keys']['label']) ? $this->entityInfo['entity keys']['label'] : NULL;
    $this->collectionName = isset($this->entityInfo['collection']) ? $this->entityInfo['collection'] : $this->entityType;

  }

  /**
   * {@inheritdoc}
   */
  public function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    // @todo fix MongoEntityLoader
    return NULL;
  }

  /**
   * Gets entities from the static cache.
   * DrupalDefaultEntityController::cacheGet returns an empty array if both $ids and $conditions are empty.
   * This fixes that.
   *
   * @param $ids
   *   If not empty, return entities that match these IDs.
   * @param $conditions
   *   If set, return entities that match all of these conditions.
   *
   * @return
   *   Array of entities from the entity cache.
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
      else {
        $entities = $this->entityCache;
      }
    }

    // Exclude any entities loaded from cache if they don't match $conditions.
    // This ensures the same behavior whether loading from memory or database.
    if ($conditions) {
      foreach ($entities as $entity) {
        $entity_values = (array) $entity;
        if (array_diff_assoc($conditions, $entity_values)) {
          unset($entities[$entity->{$this->idKey}]);
        }
      }
    }
    return $entities;
  }

  /**
   * {@inheritdoc}
   */
  public function load($ids = array(), $conditions = array()) {

    $entities = array();

    if (!empty($ids)) {
      $ids = array_map('intval', array_filter($ids));
    }
    $revision_id = FALSE;

    // Create a new variable which is either a prepared version of the $ids
    // array for later comparison with the entity cache, or FALSE if no $ids
    // were passed. The $ids array is reduced as items are loaded from cache,
    // and we need to know if it's empty for this reason to avoid querying the
    // database when all requested entities are loaded from cache.
    $passed_ids = is_array($ids) ? array_flip($ids) : FALSE;

    // Try to load entities from the static cache.
    if ($this->cache && !$revision_id) {
      $entities = $this->cacheGet($ids, $conditions);
      // If any entities were loaded, remove them from the ids still to load.
      if ($passed_ids) {
        $ids = array_keys(array_diff_key($passed_ids, $entities));
      }
    }

    if (!($this->cacheComplete && $ids === FALSE && !$conditions) && ($ids === FALSE || $ids || $revision_id)) {

      $queried_entities = array();

      $collection = mongodb_collection($this->collectionName);

      if ($passed_ids === FALSE && !$conditions) {
        $cursor = $collection->find();
      }
      else {

        $find = array();

        if (!empty($ids)) {
          if (count($ids) == 1) {
            $find['_id'] = current($ids);
          }
          else {
            $find['_id']['$in'] = $ids;
          }
        }
        elseif (!empty($conditions)) {
          foreach ($conditions as $key => $value) {
            // @todo better typing of properties
            $find[$key] = $value;
          }
        }

        if (!empty($find)) {
          $cursor = $collection->find($find);
        }

      }

      if (isset($cursor)) {
        foreach ($cursor as $document) {
          $id = $document['_id'];
          $queried_entities[$id] = $this->buildEntity($document);
        }
      }

    }

    // Pass all entities loaded from the database through $this->attachLoad(),
    // which attaches fields (if supported by the entity type) and calls the
    // entity type specific load callback, for example hook_node_load().
    if (!empty($queried_entities)) {
      $this->attachLoad($queried_entities);
      $entities += $queried_entities;
    }

    if ($this->cache) {
      // Add entities to the cache.
      if (!empty($queried_entities)) {
        $this->cacheSet($queried_entities);

        // Remember if we have cached all entities now.
        if (!$conditions && $ids === FALSE) {
          $this->cacheComplete = TRUE;
        }
      }
    }

    // Ensure that the returned array is ordered the same as the original
    // $ids array if this was passed in and remove any invalid ids.
    if ($passed_ids && $passed_ids = array_intersect_key($passed_ids, $entities)) {
      foreach ($passed_ids as $id => $value) {
        $passed_ids[$id] = $entities[$id];
      }
      $entities = $passed_ids;
    }

    return $entities;

  }

  /**
   * {@inheritdoc}
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {

    list($entity_id, $revision_id, $bundle) = entity_extract_ids($this->entityType, $entity);

    field_attach_presave($this->entityType, $entity);

    $this->invoke('presave', $entity);

    // Create a new object.
    $document = $this->buildDocument($entity);
    $collection = mongodb_collection($this->collectionName);

    if ($entity->is_new) {
      $collection->insert($document, array('w' => 1));
      $this->resetCache(array($entity->_id));
      $this->invoke('insert', $entity);
      $return = SAVED_NEW;
      $op = 'insert';
    }
    else {
      $collection->save($document, array('w' => 1));
      $this->resetCache(array($entity->_id));
      $this->invoke('update', $entity);
      $return = SAVED_UPDATED;
      $op = 'update';
    }

    if ($this->entityInfo['revisionable'] && !empty($entity->is_revision)) {
      $document->_id = $document->_revision;
      $document->_entity_id = $document->_id;
      unset($document->_revision);
      $revision_collection = mongodb_collection($this->collectionName, 'revision');
      $revision_collection->insert($document, array('w' => 1));
    }

    // Save fields.
    $function = "field_attach_$op";
    $function($this->entityType, $entity);

    unset($entity->is_new);
    unset($entity->original);
    return $return;

  }

  /**
   * Implements EntityAPIControllerInterface.
   */
  public function delete($ids) {

    $entities = $ids ? $this->load($ids) : FALSE;

    $ids = array_map('intval', $ids);

    $collection = mongodb_collection($this->collectionName);
    $collection->remove(array('_id' => array('$in' => $ids)));

    // Reset the cache as soon as the changes have been applied.
    $this->resetCache($ids);

    // @todo load entities (seriously?) and invoke delete for all entities
    foreach ($entities as $id => $entity) {
      $this->invoke('delete', $entity);
    }
  }

  /**
   * Helper function to convert entity data to MongoDB document structure.
   *
   * When saving a MongoEntity to the collection, all of the entity's public
   * variables (found with get_class_vars) are written to the document, as well
   * as a reduced array of its fields (borrowed from
   * mongo_field_storage.module). A unique numeric _id is generated and if the
   * entity has a label key, that property is saved as well.
   */
  public function buildDocument($entity) {

    $entity->is_new = !empty($entity->is_new) || empty($entity->_id);

    list($entity_id, $revision_id, $bundle) = entity_extract_ids($this->entityType, $entity);
    $bundle = $bundle ? $bundle : $this->entityType;

    // Generate a (numeric) id for new entities.
    // Drupal requires (or strongly recommends) numeric ids for entities,
    // preventing us from using Mongo's standard ObjectId.

    $info = mongodb_collection('mongo_entity_info');
    $info_fields = array();
    if ($entity->is_new) {
      $info_fields['last_id'] = 1;
    }
    if ($this->entityInfo['revisionable']) {
      $info_fields['last_revision'] = 1;
    }
    if (!empty($info_fields)) {
      $entity_info = $info->findAndModify(array('entity_type' => $this->entityType), array('$inc' => $info_fields), $info_fields, array('new' => TRUE, 'upsert' => TRUE));
      if (isset($entity_info['last_id'])) {
        $entity->_id = $entity_info['last_id'];
      }
      if (isset($entity_info['last_revision'])) {
        $entity->_revision = $entity_info['last_revision'];
      }
    }

    // Get the list of fields for this entity type and bundle.
    $fields = field_info_instances($this->entityType, $bundle);

    // Use property info to cast values before saving.
    $properties = array();
    $property_info = entity_get_property_info($this->entityType);
    if (isset($property_info['properties'])) {
      $properties += $property_info['properties'];
    }
    if (isset($property_info['bundles'][$bundle]['properties'])) {
      $properties += $property_info['bundles'][$bundle]['properties'];
    }
    // Get the list of defined class properties, to store with this entity.
    $properties = array_merge(get_class_vars(get_class($entity)), $properties);

    // Create a new object.
    $document = new stdClass();

    // @todo nameKey is overwriting _id
    if ($this->nameKey) {
      $document->{$this->nameKey} = $entity->{$this->nameKey};
    }
    if ($this->bundleKey) {
      $document->{$this->bundleKey} = $entity->{$this->bundleKey};
    }
    if ($this->labelKey) {
      $document->{$this->labelKey} = $entity->{$this->labelKey};
    }

    // Write properites.
    foreach ($properties as $property_name => $info) {

      // Ignore fields and empty properties
      if (empty($info['field']) && isset($entity->$property_name)) {
        // If the class property has a corresponding property info entry,
        // use the 'type' attribute to cast this value before writing
        // to MongoDB.
        // This includes 'properties' that are embedded entities.
        $type = isset($info['type']) ? $info['type'] : NULL;

        // If this is a defined property
        if ($type) {
          if (preg_match('/^list\<(.+)\>$/', $type, $matches)) {
            $type = $matches[1];
            $values = array();

            foreach ($entity->$property_name as $value) {
              $values[] = $this->getDocumentPropertyValue($entity, $property_name, $type, $value);
            }
            $document->$property_name = $values;
          } else {
            $document->$property_name = $this->getDocumentPropertyValue($entity, $property_name, $type, $entity->$property_name);
          }
        }
        // If this is a class property
        else {
          if (is_array($entity->$property_name)) {
            $values = array();
            foreach ($entity->$property_name as $key => $value) {
              $values[$key] = $this->getDocumentPropertyValue($entity, $property_name, $type, $value);
            }
            $document->$property_name = $values;
          }
          else {
            $document->$property_name = $this->getDocumentPropertyValue($entity, $property_name, $type, $entity->$property_name);
          }
        }
      }
    }

    // Write fields
    // From mongodb_field_storage.module:
    // Add the fieldapi fields to the new object.
    if (!empty($fields)) {
      foreach ($fields as $field_name => $field) {
        $field = field_info_field($field_name);

        $field_values = array();
        if (isset($entity->$field_name)) {
          foreach ($entity->$field_name as $language => $values) {
            // According to field.test, we should not save anything for NULL.
            if (!empty($values[0])) {
              if ($field['cardinality'] == 1) {
                $field_values = $this->getDocumentFieldValue($entity, $field, $values[0], $language);
              }
              else {
                // Collapse deltas.
                $values = array_values($values);
                if ($field['cardinality'] > 1 && count($values) > $field['cardinality']) {
                  throw new MongodbStorageException(t('Invalid delta for @field_name, not saving @entity_type @entity_id', array(
                    '@field_name' => $field_name,
                    '@entity_type' => $this->entityType,
                    '@entity_id' => $entity_id,
                  )));
                }
                // @todo handle embedded entities
                foreach ($values as $delta => $column_values) {
                  $field_values[] = $this->getDocumentFieldValue($entity, $field, $column_values, $language);
                }
              }
            }
          }
        }
        $document->$field_name = empty($field_values) ? NULL : $field_values;
      }
    }

    $document->_id = intval($entity->_id);
    if (isset($entity->_revision)) {
      $document->_revision = intval($entity->_revision);
    }

    return $document;

  }

  function getDocumentPropertyValue($entity, $property_name, $type, $value) {

    switch ($type) {
      case 'text':
        return strval($value);
        break;
      case 'integer':
        return intval($value);
        break;
      case 'decimal':
        return floatval($value);
        break;
      case 'boolean':
        return $value ? TRUE : FALSE;
        break;
      case 'date':
        // Convert timestamp to MongoDate
        return new MongoDate($value);
      case 'datetime':
        // Convert datetime (a string) to a MongoDate
        return new MongoDate(strtotime($value));
      case 'blob':
        return new MongoBinData($value, 2);
      default:
        // If this is an embedded entity property..
        if ($controller = _mongo_entity_get_embedded_entity_controller($type)) {

          // Update embedded entity with parent values
          $value->_parent = $entity->_id;
          $value->_collection = $this->entityType;
          $value->_property = $property_name;
          return $controller->buildDocument($value);
        }
        // If this is an entity
        else if (($entity_info = entity_get_info($type))
          && isset($entity_info['entity keys']['id'])
          && ($entity_property_info = entity_get_property_info($type))
          && isset($entity_property_info['properties'][$entity_info['entity keys']['id']])
          && ($real_type = $entity_property_info['properties'][$entity_info['entity keys']['id']]['type'])
          && ($real_type === 'integer' || $real_type === 'text')) {
          return $this->getDocumentPropertyValue($entity, $property_name, $real_type, $value);
        }
        else {
          return $value;
        }
        break;
    }

  }

  function getDocumentFieldValue($entity, $field, $values, $language = LANGUAGE_NONE) {

    $field_name = $field['field_name'];
    $field_values = array();

    // If this is an embedded entity field, build a new document for that
    if ($field['module'] == 'mongo_entity' && $field['type'] == 'embedded_entity') {

      $embedded_entity_type = $field['settings']['entity_type'];
      $embedded_controller = entity_get_controller($embedded_entity_type);

      $field_values = $embedded_controller->buildDocument($values['entity']);

      // Update the entity with the parent info
      $values['entity']->_parent = $entity->_id;
      $values['entity']->_collection = $this->entityType;
      $values['entity']->_field = $field_name;

      unset($values['entity']->is_new);
      unset($values['entity']->original);

    }
    else {
      // Otherwise, write the columns described in the schema for this field
      foreach ($values as $column_name => $column_value) {

        // @todo allow for non-column values
        if (isset($field['columns'][$column_name])) {
          $field_values[$column_name] = _mongo_entity_value($field['columns'][$column_name]['type'], $column_value);
        }

      }

      if ($language != LANGUAGE_NONE) {
        $field_values['_language'] = $language;
      }

    }


    return $field_values;

  }

  /**
   * Helper function to extract Mongo document to an entity.
   *
   * Mainly used to unravel the field structure into its language and delta keys
   * i.e. {field_example: 4} becomes field_example['und'][0]['value'] = 4;
   */
  public function buildEntity($document) {

    // Pre-load the field info, since we're going to be using it a lot.
    // $field_info_cached doesn't actually get used, we only want to load
    // this once.
    static $field_info_cached;
    if (!$field_info_cached) {
      field_info_fields();
      $field_info_cached = TRUE;
    }

    $entity_class = $this->entityInfo['entity class'];

    if ($this->bundleKey && isset($document[$this->bundleKey])) {
      $bundle = $document[$this->bundleKey];
    }
    else {
      $bundle = $this->entityType;
    }

    $fields = field_info_instances($this->entityType, $bundle);

    // Build fields into entity objects
    foreach ($fields as $field_name => $field) {
      $field = field_info_field($field_name);
      $field_values = array();

      if (!empty($document[$field_name])) {
        // Restore the field structure.
        if ($field['cardinality'] == 1) {

          $language = isset($document[$field_name]['_language']) ? $document[$field_name]['_language'] : LANGUAGE_NONE;
          unset($document[$field_name]['_language']);

          $field_values[$language][0] = $this->setEntityFieldValue($document, $field, $document[$field_name]);

        }
        else {
          foreach ($document[$field_name] as $delta => $column_values) {

            $language = LANGUAGE_NONE;
            if (isset($column_values['_language'])) {
              $language = $column_values['_language'];
              unset($column_values['_language']);
            }

            $field_values[$language][$delta] = $this->setEntityFieldValue($document, $field, $column_values);

          }
        }
      }
      $document[$field_name] = $field_values;
    }

    // Build properties into entity objects
    $properties = array();
    $property_info = entity_get_property_info($this->entityType);
    if (isset($property_info['properties'])) {
      $properties += $property_info['properties'];
    }
    if (isset($property_info['bundles'][$bundle]['properties'])) {
      $properties += $property_info['bundles'][$bundle]['properties'];
    }

    foreach ($properties as $property_name => $info) {

      // Ignore fields and empty properties
      if (empty($info['field']) && !empty($document[$property_name])) {

        // If this is an embedded entity property...
        $type = isset($info['type']) ? $info['type'] : NULL;
        $matches = array();

        // Check if this is a list property
        if ($type && preg_match('/^list\<(.+)\>$/', $type, $matches)) {
          $type = $matches[1];
        }

        if ($type && ($embedded_entity_info = entity_get_info($type)) && ($embedded_controller = entity_get_controller($type)) && is_a($embedded_controller, 'MongoEmbeddedEntityController')) {
          // This is an array of entities
          if ($matches) {
            $property_values = array();
            foreach ($document[$property_name] as $values) {
              $property_values[] = $embedded_controller->buildEntity($values + array(
                '_parent' => $document['_id'],
                '_collection' => $this->entityType,
                '_property' => $property_name,
              ));
            }
            $document[$property_name] = $property_values;
          }
          // This is a single entity
          else {
            $values = $document[$property_name];
            $document[$property_name] = $embedded_controller->buildEntity($values + array(
              '_parent' => $document['_id'],
              '_collection' => $this->entityType,
              '_property' => $property_name,
            ));
          }
        }
        else if (isset($info['type']) && $info['type'] == 'date' && is_a($document[$property_name], 'MongoDate')) {
          // Convert MongoDate back to seconds
          // @todo does this need to be microseconds?
          $document[$property_name] = $document[$property_name]->sec;
        }
        else if (isset($info['type']) && $info['type'] == 'datetime' && is_a($document[$property_name], 'MongoDate')) {
        }

      }

      // If empty property set it to NULL.
      if (empty($document[$property_name])) {
        $document[$property_name] = NULL;
      }


    }

    return new $entity_class($document, $this->entityType);

  }

  function setEntityFieldValue($document, $field, $values) {

    $field_name = $field['field_name'];
    $field_values = array();

    if ($field['module'] == 'mongo_entity' && $field['type'] == 'embedded_entity') {
      $embedded_entity_type = $field['settings']['entity_type'];
      $embedded_controller = entity_get_controller($embedded_entity_type);
      // @todo if the cardinality of the field has changed, this can cause a fatal error
      $field_values['entity'] = $embedded_controller->buildEntity($values + array(
        '_parent' => $document['_id'],
        '_collection' => $this->entityType,
        '_field' => $field_name,
      ));

    }
    // Handle datetime fields
    elseif ($field['module'] == 'date' && $field['type'] == 'datetime') {
      // Convert MongoDate to datetime
      foreach ($field['columns'] as $column => $settings) {
        if (isset($values[$column]) && is_a($values[$column], 'MongoDate')) {
          // @todo Check that this converts correctly
          // Some fields may have different granularity or formats
          $values[$column] = date('Y-m-d H:i:s', $values[$column]->sec);
        }
      }
      $field_values = $values;
    }
    else {
      foreach ($field['columns'] as $column => $settings) {
        if (isset($values[$column]) && is_a($values[$column], 'MongoBinData')) {
          $values[$column] = $values[$column]->bin;
        }
      }
      $field_values = $values;
    }

    return $field_values;

  }

  /**
   * Overridden to care about reverted bundle entities and to skip Rules.
   */
  public function invoke($hook, $entity) {

    if ($hook == 'delete') {
      // To ease figuring out whether this is a revert, make sure that the
      // entity status is updated in case the providing module has been
      // disabled.
      if (entity_has_status($this->entityType, $entity, ENTITY_IN_CODE) && !module_exists($entity->{$this->moduleKey})) {
        $entity->{$this->statusKey} = ENTITY_CUSTOM;
      }
      $is_revert = entity_has_status($this->entityType, $entity, ENTITY_IN_CODE);
    }

    // Do not use field attach API!

    // Invoke the hook.
    module_invoke_all($this->entityType . '_' . $hook, $entity);
    // Invoke the respective entity level hook.
    if ($hook == 'presave' || $hook == 'insert' || $hook == 'update' || $hook == 'delete') {
      // Paranoia. Don't let mongodb_field_storage execute its hook. Everyone else is AOK.
      foreach (module_implements('entity_' . $hook) as $module) {
        if ($module != 'mongodb_field_storage') {
          module_invoke($module, 'entity_' . $hook, $entity, $this->entityType);
        }
      }
    }

  }

  /**
   * Overridden.
   * @see DrupalDefaultEntityController::attachLoad()
   *
   * Changed to call type-specific hook with the entities keyed by name if they
   * have one.
   */
  protected function attachLoad(&$queried_entities, $revision_id = FALSE) {

    // Attach fields.
    if ($this->entityInfo['fieldable']) {
      if ($revision_id) {
        field_attach_load_revision($this->entityType, $queried_entities);
      }
      else {
        field_attach_load($this->entityType, $queried_entities);
      }
    }

    // Call hook_entity_load(), for every module except mongodb_field_storage
    foreach (module_implements('entity_load') as $module) {
      if ($module != 'mongodb_field_storage') {
        $function = $module . '_entity_load';
        $function($queried_entities, $this->entityType);
      }
    }
    // Call hook_TYPE_load(). The first argument for hook_TYPE_load() are
    // always the queried entities, followed by additional arguments set in
    // $this->hookLoadArguments.
    // For entities with a name key, pass the entities keyed by name to the
    // specific load hook.
    if ($this->nameKey != $this->idKey) {
      $entities_by_name = entity_key_array_by_property($queried_entities, $this->nameKey);
    }
    else {
      $entities_by_name = $queried_entities;
    }
    $args = array_merge(array($entities_by_name), $this->hookLoadArguments);
    foreach (module_implements($this->entityInfo['load hook']) as $module) {
      call_user_func_array($module . '_' . $this->entityInfo['load hook'], $args);
    }

  }

}

class MongoEntityFeaturesController extends EntityDefaultFeaturesController {

  function revert($module = NULL) {
    if ($defaults = features_get_default($this->type, $module)) {
      foreach ($defaults as $name => $entity) {
        // @todo Delete existing entity?
        entity_save($this->type, $entity);
      }
    }
  }

}
