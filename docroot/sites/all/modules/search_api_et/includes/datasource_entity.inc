<?php

/**
 * @file
 * Contains the SearchApiEtDatasourceController class.
 */

/**
 * Provides multilingual versions of all entity types.
 */
class SearchApiEtDatasourceController extends SearchApiEntityDataSourceController {

  /**
   * Overrides SearchApiEntityDataSourceController::$table.
   *
   * Needed because we have a string ID, instead of a numerical one.
   *
   * @var string
   */
  protected $table = 'search_api_et_item';

  /**
   * {@inheritdoc}
   */
  public function getIdFieldInfo() {
    return array(
      'key' => 'search_api_et_id',
      'type' => 'string',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function loadItems(array $ids) {
    $item_languages = array();
    foreach ($ids as $id) {
      // This method might receive two different types of item IDs depending on
      // where it is being called from. For example, when called from
      // search_api_index_specific_items(), it will receive multilingual IDs
      // (with language prefix, like en_2). On the other hand, when called from
      // a processor (for example from SearchApiHighlight::getFulltextFields()),
      // the IDs won't be multilingual (no language prefix), just standard
      // entity IDs instead. Therefore we need to account for both cases here.
      // Case 1 - language prefix is in item ID.
      if (strpos($id, '_') !== FALSE) {
        list($langcode, $entity_id) = explode('_', $id, 2);
        $item_languages[$entity_id][] = $langcode;
      }
      // Case 2 - no language prefix in item ID.
      else {
        $item_languages[$id][] = NULL;
      }
    }
    $entities = entity_load($this->entityType, array_keys($item_languages));

    // If some items could not be loaded, remove them from tracking.
    if (count($entities) != count($item_languages)) {
      $unknown = array_keys(array_diff_key($item_languages, $entities));
      if ($unknown) {
        $deleted = array();
        foreach ($unknown as $entity_id) {
          foreach ($item_languages[$entity_id] as $langcode) {
            $deleted[] = "{$langcode}_{$entity_id}";
          }
        }
        search_api_track_item_delete($this->type, $deleted);
      }
    }

    // Now arrange them according to our IDs again, with language.
    $items = array();
    foreach ($item_languages as $entity_id => $langs) {
      if (!empty($entities[$entity_id])) {
        foreach ($langs as $lang) {
          // Following on the two cases described above, we should return
          // the same item IDs (with or without language prefix) as received.
          $id = !empty($lang) ? "{$lang}_{$entity_id}" : $entity_id;
          $entity = clone $entities[$entity_id];
          $entity->search_api_et_id = $id;
          $entity->language = $lang;
          $items[$id] = $entity;
        }
      }
    }

    return $items;
  }

  /**
   * {@inheritdoc}
   */
  public function getMetadataWrapper($item = NULL, array $info = array()) {
    // Since this is usually called with a "property info alter" callback
    // already in place (and only one value is allowed), we have to call
    // the existing callback from within our own callback to make it work.
    $property_info_alter = isset($info['property info alter']) ? $info['property info alter'] : NULL;
    $callback = new SearchApiEtPropertyInfoAlter($property_info_alter);
    $info['property info alter'] = array($callback, 'propertyInfoAlter');

    // If the item isn't the object and a multilingual id is provided
    // extract the entity id to load and wrap the entity.
    if (!is_object($item) && is_scalar($item) && strpos($item, '_') !== FALSE) {
      list($item_language, $item) = explode('_', $item);
    }

    $wrapper = entity_metadata_wrapper($this->entityType, $item, $info);

    // If the item's language is set, let's set it on all wrapper fields,
    // so that their translated values get indexed.
    if (!empty($item->search_api_language)) {
      // Set language on the wrapper as a whole.
      $wrapper->language($item->search_api_language);
      // Also try to set language on all wrapper fields, recursively.
      if (!empty($item->search_api_index)) {
        $this->setLanguage($wrapper, $item->search_api_index->options['fields'], $item->search_api_language);
      }
    }

    return $wrapper;
  }

  /**
   * Sets language of specific fields on an EntityMetadataWrapper object.
   *
   * This is essentially a copy of search_api_extract_fields(), just slightly
   * adapted to set language on the wrapper fields instead of extracting them.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapper on which fields to set language on.
   * @param array $fields
   *   The fields to set language on, as stored in an index. I.e., the array
   *   keys are field names, the values are arrays with at least a "type" key
   *   present.
   * @param array $langcode
   *   A code of the language to set to wrapper fields.
   *
   * @return array
   *   The $fields array with additional "value" and "original_type" keys set.
   *
   * @see SearchApiEtDatasourceController::getMetadataWrapper()
   * @see SearchApiEtDatasourceController::setLanguage()
   */
  protected function setLanguage($wrapper, $fields, $langcode) {
    // If $wrapper is a list of entities, we have to aggregate their field values.
    $wrapper_info = $wrapper->info();
    if (search_api_is_list_type($wrapper_info['type'])) {
      foreach ($fields as &$info) {
        $info['value'] = array();
        $info['original_type'] = $info['type'];
      }
      unset($info);
      try {
        foreach ($wrapper as $w) {
          $nested_fields = $this->setLanguage($w, $fields, $langcode);
          foreach ($nested_fields as $field => $info) {
            if (isset($info['value'])) {
              $fields[$field]['value'][] = $info['value'];
            }
            if (isset($info['original_type'])) {
              $fields[$field]['original_type'] = $info['original_type'];
            }
          }
        }
      }
      catch (EntityMetadataWrapperException $e) {
        // Catch exceptions caused by not set list values.
      }
      return $fields;
    }

    $nested = array();
    foreach ($fields as $field => $info) {
      $pos = strpos($field, ':');
      if ($pos === FALSE) {
        if (isset($wrapper->$field) && method_exists($wrapper->$field, 'language')) {
          $wrapper->$field->language($langcode);
        }
      }
      else {
        list($prefix, $key) = explode(':', $field, 2);
        $nested[$prefix][$key] = $info;
      }
    }
    foreach ($nested as $prefix => $nested_fields) {
      if (isset($wrapper->$prefix)) {
        $nested_fields = $this->setLanguage($wrapper->$prefix, $nested_fields, $langcode);
        foreach ($nested_fields as $field => $info) {
          $fields["$prefix:$field"] = $info;
        }
      }
      else {
        foreach ($nested_fields as &$info) {
          $info['value'] = NULL;
          $info['original_type'] = $info['type'];
        }
      }
    }
    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function getItemId($item) {
    return isset($item->search_api_et_id) ? $item->search_api_et_id : NULL;
  }

  /**
   * Overrides SearchApiEntityDataSourceController::startTracking().
   *
   * Reverts the behavior to always use getAllItemIds(), instead of taking a
   * shortcut via "base table".
   *
   * This method will also be called when the multilingual configuration of an
   * index changes, to take care of new and/or out-dated IDs.
   */
  public function startTracking(array $indexes) {
    if (!$this->table) {
      return;
    }

    // We first clear the tracking table for all indexes, so we can just insert
    // all items again without any key conflicts.
    // @TODO: Update the tracking table instead of removing and re-adding items.
    // Clearing the tracking table first is standard Search API behavior, it
    // would be nicer though to update its content instead, by adding missing
    // item IDs and removing those that shouldn't be there anymore.
    $this->stopTracking($indexes);

    $operations = array();

    // Find out number of all entities to be processed.
    foreach ($indexes as $index) {
      $entity_type = $index->getEntityType();
      $entity_info = entity_get_info($entity_type);

      if (!empty($entity_info['base table'])) {
        // Assumes that all entities use the "base table" property and the
        // "entity keys[id]" in the same way as the default controller.
        $id_field = $entity_info['entity keys']['id'];
        $table = $entity_info['base table'];

        // We could also use a single insert (with a JOIN in the nested query),
        // but this method will be mostly called with a single index, anyways.
        // Select all entity ids.
        $query = db_select($table, 't');
        $query->addField('t', $id_field);
        $item_ids = $query->execute()->fetchCol();

        if (empty($item_ids)) {
          continue;
        }
      }
      else {
        // In the absence of a 'base table', load the entities.
        $query = new EntityFieldQuery();
        $entities = $query->entityCondition('entity_type', $entity_type)
          ->execute();

        if (empty($entities)) {
          continue;
        }

        $item_ids = array_keys($entities[$entity_type]);
      }

      $steps = ceil(count($item_ids) / $index->options['cron_limit']);

      for ($step = 0; $step < $steps; $step++) {
        $operations[] = array(
          'search_api_et_batch_queue_entities',
          array($index, $item_ids, $step),
        );
      }
    }

    // This might be called both from web interface as well as from drush.
    $t = drupal_is_cli() ? 'dt' : 't';

    $batch = array(
      'title' => $t('Adding items to the index queue'),
      'operations' => $operations,
      'finished' => 'search_api_et_batch_queue_entities_finished',
      'progress_message' => $t('Completed about @percentage% of the queueing operation.'),
      'file' => drupal_get_path('module', 'search_api_et') . '/search_api_et.batch.inc',
    );
    batch_set($batch);

    if (drupal_is_cli()) {
      // Calling drush_backend_batch_process() to start batch execution directly
      // from here doesn't work for some unknown reason, so we need to call it
      // from a shutdown function instead.
      drupal_register_shutdown_function('search_api_et_shutdown_batch_process');
    }
    else {
      batch_process();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function trackItemChange($item_ids, array $indexes, $dequeue = FALSE) {
    // If this method was called from _search_api_index_reindex(), $item_ids
    // will be set to FALSE, which means we need to reindex all items, so no
    // need for any other processing below.
    if ($item_ids === FALSE) {
      parent::trackItemChange($item_ids, $indexes, $dequeue);
      return;
    }

    // We assume here that we got entity ids in $item_ids, not proper item ids,
    // so first let's try to convert them to real multilingual item ids.
    $multilingual_item_ids = array();
    foreach ($indexes as $index) {
      $multilingual_item_ids += $this->getAllIndexItemIds($index, $item_ids);
    }
    // Once we have real multilingual item ids, we can do proper tracking.

    // There are situations where trackItemChange() is being called, but the
    // item ids do not exist in the search_api_et_item table yet. To be sure
    // that we have items to mark for update, let's try to insert them first.
    // But then again, if they already exist in the database, we must delete
    // them first before being able to re-insert them, otherwise we'll end up
    // with Integrity constraint violation  PDOException.
    parent::trackItemDelete($multilingual_item_ids, $indexes);
    parent::trackItemInsert($multilingual_item_ids, $indexes);

    // Finally, we can mark those items for an update.
    parent::trackItemChange($multilingual_item_ids, $indexes, $dequeue);
  }

  /**
   * Retrieves all item IDs for a given index, and, optionally, given entity IDs.
   *
   * Is used instead of SearchApiAbstractDataSourceController::getAllItemIds(),
   * since available items depend on the index configuration.
   *
   * @param SearchApiIndex $index
   *   The index for which item IDs should be retrieved.
   *
   * @return array
   *   An array with all item IDs for a given index, with keys and values both
   *   being the IDs.
   */
  public function getAllIndexItemIds(SearchApiIndex $index, $item_ids = FALSE) {
    // Try to load entities from provided $item_ids.
    $entities = entity_load($this->entityType, $item_ids);
    // If entities were loaded successully, it means that $item_ids do not have
    // language prefix in them, so they are not really item ids, rather just
    // entity ids, and we need to convert them to proper item ids.
    if (!empty($entities)) {
      $item_ids = array();
      foreach ($entities as $id => $entity) {
        foreach (search_api_et_item_languages($entity, $this->entityType, $index) as $lang) {
          $lang_id = "{$lang}_{$id}";
          $item_ids[$lang_id] = $lang_id;
        }
      }
    }
    // If no entities were loaded, it might mean that $item_ids might already
    // be multilingual, and in this case let's just return them.
    return $item_ids;
  }

}
