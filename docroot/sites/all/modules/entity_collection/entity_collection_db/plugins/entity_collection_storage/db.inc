<?php
/**
 * @file
 * Sample entity_collection storage database implementation.
 */

/**
 * Entity collection storage backend using the database.
 */
class EntityCollectionStorageDB extends EntityCollectionStorage {
  
  const CACHE_BIN = 'cache_entity_collection_storage_db';

  /**
   * Save an entity collection
   * @see EntityCollectionStorageInterface::save().
   */
  public function save(EntityCollection $collection, EntityCollectionTreeNode $tree, $contexts = array(), $position = 0, $depth = 0) {
    if (isset($collection->contexts) && empty($contexts)) {
      $contexts = $collection->contexts;
    }
    $record = $this->getRecord($collection, $tree, $contexts, $position, $depth);
    if ($record) {
      $primary_keys = array();
      if ($this->itemExists($collection, $record->eid, $record->entity_type, $contexts)) {
        $primary_keys = $this->getPrimaryKeys();
      }
      drupal_write_record('entity_collection_storage', $record, $primary_keys);
      EntityCollectionStorageDB::invalidateCache($collection, $contexts);
    }

    $pos = 0;
    foreach ($tree->getChildren() as $child_entity) {
      $this->save($collection, $child_entity, $contexts, $pos++, ($depth + 1));
    }
  }

  /**
   * Settings for this storage engine.
   */
  function bundleSettingsForm($bundle, &$form, &$form_state) {
    $form['store_entity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Store full entity'),
      '#default_value' => isset($bundle->settings['store_entity']) ? $bundle->settings['store_entity'] : FALSE,
      '#description' => t('Store the full entity in the entity collection table.
       This is performant in some cases, but it also requires you to set up your
       cache invalidation settings.'),
    );

    $form['enable_caching'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Storage Caching'),
      '#default_value' => isset($bundle->settings['enable_caching']) ? $bundle->settings['enable_caching'] : FALSE,
      '#description' => t(''),
    );

  }

  /**
   * Append a new item to the entity_collection.
   * @param string $name
   * @param EntityCollectionTreeNode $item
   */
  public function appendItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array()) {
    // Fetch the last position.
    $query = db_select('entity_collection_storage', 'rs');
    $query->addField('rs', 'position');
    $query->orderBy('position', 'DESC');
    $query->condition('parent', isset($item->parent) && isset($item->parent->entity_id) ? $item->parent->entity_id : 0);
    $position = $query->execute()->fetchField();

    $this->save($collection, $item, $contexts, ($position + 1));
  }
  
  /**
   * Prepend a new item to the entity_collection on the top.
   * @param string $name
   * @param EntityCollectionTreeNode $item
   */
  public function prependItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array()) {
    $this->save($collection, $item, $contexts, 0);
    // TODO: Reorder other items?
  }

  /**
   * @see EntityCollectionStorageInterface::getRecord().
   */
  protected function getRecord(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array(), $position = 0, $depth = 0) {
    if (isset($item->content)) {
      $record = new stdClass();
      $record->name = $collection->name;
      if (!empty($collection->settings->store_entity)) {
        $record->entity = serialize($item->content);
      }
      $record->entity_type = $item->type;
      $record->eid = $item->entity_id;
      $record->depth = isset($item->parent) && isset($item->parent->key) ? $depth : 0;
      $record->style = $item->style;
      $record->parent = isset($item->parent) && isset($item->parent->key) ? $item->parent->key : 0;
      $record->position = ($position >= 0) ? $position : 0;
      $record->contexts = $this->serializeContexts($contexts);
      return $record;
    }
    return FALSE;
  }

  /**
   * Check if an item already exists in the database.
   */
  protected function itemExists(EntityCollection $collection, $entity_id, $type, $contexts = array()) {
    return db_select('entity_collection_storage', 'rs')
      ->fields('rs', array('eid'))
      ->condition('eid', $entity_id)
      ->condition('name', $collection->name)
      ->condition('contexts', $this->serializeContexts($contexts))
      ->execute()->fetchField();
  }

  /**
   * Execute an EntityCollectionQuery
   * @param EntityCollectionQuery a query to execute.
   */
  public function executeQuery(EntityCollectionQuery $query) {
    $db_query = db_select('entity_collection_storage', 'ecs')
      ->fields('ecs', array('name'));
    $bundle_conditions = $query->getBundleConditions();
    $entity_conditions = $query->getEntityConditions();
    $collection_conditions = $query->getCollectionConditions();
    $contexts = $query->getContexts();
    if (!empty($bundle_conditions)) {
      $db_query->innerJoin('entity_collection_collections', 'ec', 'ec.name = ecs.name');
      foreach ($bundle_conditions as $condition) {
        $db_query->condition('ec.bundle', $condition['value'], $condition['operator']);
      }
    }
    if (!empty($contexts)) {
      $db_query->condition('ecs.contexts', $this->serializeContexts($contexts));
    }
    foreach ($entity_conditions as $condition) {
      $and_condition = db_and()
        ->condition('ecs.entity_type', $condition['entity_type'], '=')
        ->condition('ecs.eid', $condition['entity_ids'], $condition['operator']);
      $db_query->condition($and_condition);
    }
    foreach ($collection_conditions as $condition) {
      $db_query->condition('ecs.name', $condition['value'], $condition['operator']);
    }
    return $db_query->execute()->fetchCol();
  }

  /**
   * Tries to fetch the entity collection from the cache
   */
  public function retrieveCached(EntityCollection $collection, $contexts = NULL) {
    $cache_cid = 'ec_storage_' . $collection->name . '_' . $this->serializeContexts($contexts);

    if ( $cache = cache_get($cache_cid, EntityCollectionStorageDB::CACHE_BIN) ) {
      if ( is_a($cache->data, 'EntityCollectionTreeNode') ) {
        watchdog('entity_collection_db', "Loaded collection from cache: @collection", array('@collection'=>$collection->name), WATCHDOG_DEBUG);
        return $cache->data;
      } else {
        watchdog('entity_collection_db', "Corrupted cache for: @collection. Data is not an instance of EntityCollectionTreeNode.", array('@collection'=>$collection->name), WATCHDOG_WARNING);
      }
    }

    return NULL;
  }
  
  /**
   * Stores the given collection with the correct
   */
  protected function storeCached(EntityCollection $collection, EntityCollectionTreeNode $tree, $contexts = NULL) {
    $contexts = EntityCollectionStorage::getBundleStorage($collection->bundle)->serializeContexts($contexts);
    $cache_cid = 'ec_storage_' . $collection->name . '_' . $contexts;
    return cache_set($cache_cid, $tree, EntityCollectionStorageDB::CACHE_BIN);
  }
  
  /**
   * Invalidates the cache for a collection + context
   */
  public static function invalidateCache(EntityCollection $collection, $contexts = NULL) {
    $contexts = EntityCollectionStorage::getBundleStorage($collection->bundle)->serializeContexts($contexts);
    $cache_cid = 'ec_storage_' . $collection->name . '_' . $contexts;
    watchdog('entity_collection_db', "Clearing cache for @collection", array('@collection'=>$collection->name), WATCHDOG_INFO);
    cache_clear_all($cache_cid, EntityCollectionStorageDB::CACHE_BIN);
  }

  /**
   * Retrieve a entity_collection.
   * @param string $name
   */
  public function retrieve(EntityCollection $collection, $contexts = NULL) {
    $bundle = entity_collection_bundle_load($collection->bundle);

    if (empty($contexts) && isset($collection->contexts)) {
      $contexts = $collection->contexts;
    }

    if ( !empty($bundle->settings['enable_caching']) && ($tree = $this->retrieveCached($collection, $contexts)) ) {
      return $tree;
    }

    // Serialize the context arguments into a string.
    $query = db_select('entity_collection_storage', 'rs')
      ->fields('rs')
      ->condition('name', $collection->name, '=')
      ->orderBy('depth')
      ->orderBy('position')
      ->orderBy('rsid', 'DESC'); // So if two items have the same weight the latest one goes on top

    if (isset($contexts)) {
      $query->condition('contexts', $this->serializeContexts($contexts));
    }

    $result = $query->execute();
    $tree = new EntityCollectionTreeNode();
    $bundle = entity_collection_bundle_load($collection->bundle);
    foreach ($result as $value) {
      $entity = NULL;
      if (!empty($value->parent)) {
        $node = $tree->getChild($value->parent);
      }
      if (!empty($value->style)) {
        $value->style = unserialize($value->style);
      }
      if (empty($node)) {
        $node = $tree;
      }
      if (!empty($bundle->settings['store_entity'])) {
        $entity = unserialize($value->entity);
      }
      $node->addChild($value->entity_type, $value->eid, $entity, $value->style, $value->rsid, $value->position);
    }

    // Load all entities
    // and make sure we only do as few entity_loads as possible.
    if (empty($bundle->settings['store_entity'])) {
      $nodes = $tree->getFlat();
      $entities = array();
      foreach ($nodes as $node) {
        $entities[$node->type][$node->key] = $node->entity_id;
      }
    }

    foreach ($entities as $type => $ids) {
      $loaded_entities = entity_load($type, $ids);
      foreach ($ids as $key => $id) {
        $nodes[$tree->getChildKey($type, $id)]->content = $loaded_entities[$id];
      }
    }

    if ( !empty($bundle->settings['enable_caching']) ) {
      EntityCollectionStorageDB::storeCached($collection, $tree, $contexts);
    }
    return $tree;
  }

  /**
   * Serialize contexts so that they form a materialized path as a string.
   */
  protected function serializeContexts($contexts) {
    if (empty($contexts)) {
      return '';
    }
    $serialized = array();
    foreach ($contexts as $context) {
      if (isset($context->argument)) {
        $serialized[] = $context->argument;
      }
    }
    return implode(':', $serialized);
  }

  /**
   * Gets the primary keys used to identify an item in the collections
   */
  public function getPrimaryKeys() {
    return array('eid', 'name', 'contexts');
  }
  

  /**
   * @see EntityCollectionStorageInterface::delete().
   */
  public function delete(EntityCollection $collection, $entity_type, array $entries, $contexts = NULL) {
    $query = db_delete('entity_collection_storage')
      ->condition('name', $collection->name)
      ->condition('entity_type', $entity_type)
      ->condition('eid', $entries);
    if (!empty($contexts)) {
      $query->condition('contexts', $this->serializeContexts($contexts));
    }
    $query->execute();
    EntityCollectionStorageDB::invalidateCache($collection, $contexts);
  }

  /**
   * Drop an entire entity_collection.
   * @param type $names
   */
  public function drop(EntityCollection $collection) {
    db_delete('entity_collection_storage')
      ->condition('name', $collection->name)
      ->execute();
  }
  
  public function saveBreakpoint(EntityCollection $collection, $breakpoint_value, $contexts = array()) {
    $context_hash = $this->serializeContexts($contexts);
    $breakpoints = $this->_loadAllBreapoints($collection, $contexts);
    $breakpoints[$context_hash] = $breakpoint_value;
    $this->_saveAllBreapoints($collection, $breakpoints);
  }

  public function getBreakpoint(EntityCollection $collection, $contexts = array()) {
    $context_hash = $this->serializeContexts($contexts);
    $breakpoints = $this->_loadAllBreapoints($collection, $contexts);

    return isset($breakpoints[$context_hash]) ? $breakpoints[$context_hash] : FALSE;
  }
  
  private function _loadAllBreapoints(EntityCollection $collection) {
    $var_name = "entity_collection_breakpoints_{$collection->eid}";
    return variable_get($var_name, array());
  }
  
  private function _saveAllBreapoints(EntityCollection $collection, $breakpoints = array()) {
    $var_name = "entity_collection_breakpoints_{$collection->eid}";
    variable_set($var_name, $breakpoints);
  }

}
