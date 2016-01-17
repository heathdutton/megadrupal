<?php
/**
 * @file
 * Sample entity_collection storage database implementation.
 */

/**
 * Entity collection storage backend using the database.
 */
class EntityCollectionStorageLimitDB extends EntityCollectionStorageDB {

  const FROM_TOP = 0;
  const FROM_BOTTOM = 1;

  /**
   * Settings for this storage engine.
   */
  function bundleSettingsForm($bundle, &$form, &$form_state) {
    parent::bundleSettingsForm($bundle, $form, $form_state);

    $form['store_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Storage limit'),
      '#default_value' => isset($bundle->settings['store_limit']) ? $bundle->settings['store_limit'] : 50,
      '#description' => t('Maximum number of entities to store.'),
    );
  }

  /**
   * Append a new item to the entity_collection.
   * @param string $name
   * @param EntityCollectionTreeNode $item
   */
  public function appendItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array()) {
    // First remove the elements in excess
    self::removeExceedingItems($collection, self::FROM_TOP, TRUE);
    // Then add the new item
    parent::appendItem($collection, $item, $contexts);
  }

  /**
   * Prepend a new item to the entity_collection on the top.
   * @param string $name
   * @param EntityCollectionTreeNode $item
   */
  public function prependItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array()) {
    // First remove the elements in excess
    self::removeExceedingItems($collection, self::FROM_BOTTOM, TRUE);
    // Then add the new item
    parent::prependItem($collection, $item, $contexts);
    // TODO: Reorder other items?
  }

  /**
   * Removes the elements that are exceeding the limit
   * @param EntityCollection $collection 
   * @param integer $removeFrom 
   * @param boolean $beforeInsert 
   * @author Marlon
   */
  public function removeExceedingItems(EntityCollection $collection, $removeFrom = EntityCollectionStorageLimitDB::FROM_TOP, $beforeInsert = FALSE) {
    $settings = $this->getStorageSettings($collection->bundle);
    $offset = $settings['store_limit'];

    // If we're removing items before inserting a new one make room for it.
    if ( $beforeInsert && $offset > 0 ) {
      $offset--;
    }

    // FIXME: What happens if we remove an item with some children?
    $last_query = db_select('entity_collection_storage', 'rs');
    $last_query->fields('rs', array('eid', 'entity_type'))
      ->condition('name', $collection->name)
      ->orderBy('depth', 'ASC')
      ->range($offset, 10000);

    if ( $removeFrom == self::FROM_TOP ) {
      $last_query->orderBy('position', 'DESC');
    }
    // else if ( $removeFrom == self::FROM_BOTTOM ) {
    else {
      $last_query->orderBy('position', 'ASC');
    }

    $last_entry_result = $last_query->execute();
    foreach ($last_entry_result as $entity) {
      $this->delete($collection, $entity->entity_type, array($entity->eid));
    }
  }


  private function getStorageSettings($name) {
    $query = db_select('entity_collection_bundles', 'cb');
    $query->fields('cb', array('settings'));
    $query->condition('name', $name);
    
    if($result = $query->execute()->fetchField()) {
      return !is_array($result) ? unserialize($result) : $result;
    }

  }
}
