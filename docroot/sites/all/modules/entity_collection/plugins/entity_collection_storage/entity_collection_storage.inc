<?php
/**
 * @file
 * Interface and abstract class for entity_collection storage.
 */

/**
 * Simple interface for storing entity_collections.
 */
interface EntityCollectionStorageInterface {
  /**
   * Save a particular entity collection.
   * @param EntityCollection $collection
   *   The collection where the content resides.
   * @param EntityCollectionTreeNode $tree
   *   The content to save.
   * @param array $contexts
   *   An array of contexts to use.
   * @param $position
   *   (Optional) The position to save this particular node in the tree to.
   * @param $depth
   *   (Optional) The depth we are on currently.
   */
  public function save(EntityCollection $collection, EntityCollectionTreeNode $tree, $contexts = array(), $position = 0, $depth = 0);

  /**
   * Append an item to a collection
   * @param EntityCollection $collection
   *   The collection to which the content belongs.
   * @param EntityCollectionTreeNode $item
   *   The item to append
   * @param array $contexts
   *   An array of contexts to use when appending the item.
   */
  public function appendItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array());

  /**
   * Prepend an item to a collection
   * @param EntityCollection $collection
   *   The collection to which the content belongs.
   * @param EntityCollectionTreeNode $item
   *   The item to append
   * @param array $contexts
   *   An array of contexts to use when appending the item.
   */
  public function prependItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array());

  /**
   * Retrieve the content of a collectio.
   * @param EntityCollection $collection
   *   The collection to which the content belongs.
   * @param array $contexts
   *   An array of contexts to use.
   */
  public function retrieve(EntityCollection $collection, $contexts = NULL);

  /**
   * Delete an array of entries in a collection
   * @param EntityCollection $collection
   * @param unknown_type $entity_type
   * @param array $entries
   * @param unknown_type $contexts
   */
  public function delete(EntityCollection $collection, $entity_type, array $entries, $contexts = NULL);

  /**
   * Remove all content in the entity collection.
   * @param EntityCollection $collection
   */
  public function drop(EntityCollection $collection);

  /**
   * Execute a EntityCollectionQuery and return the matching
   * EntityCollections.
   * @param EntityCollectionQuery $query
   *  The query to execute.
   */
  public function executeQuery(EntityCollectionQuery $query);

}

/**
 * Abstract class that implements some of the standard storage stuff.
 */
abstract class EntityCollectionStorage implements EntityCollectionStorageInterface {

  /**
   * Get a storage plugin for a specific bundle.
   * @param $bundle
   *   The bundle the storage should be in.
   * @return EntityCollectionStorageInterface
   *   A storage plugin object.
   */
  public static function getBundleStorage($bundle) {
    if (!is_object($bundle)) {
      $bundle = entity_collection_bundle_load($bundle);
    }
    return EntityCollectionStorage::getStorage($bundle->storage);
  }

  /**
   * Get a storage plugin by name.
   * @param name
   *   The name of the storage plugin.
   */
  public static function getStorage($name) {
    ctools_include('plugins');
    $plugin = ctools_get_plugins('entity_collection', 'plugins', $name);
    if (isset($plugin['entity_collection_storage']['class'])) {
      return new $plugin['entity_collection_storage']['class']();
    }
    return FALSE;
  }

  /**
   * Get all storage backends formatted for #options
   * @return array
   *   An array of storage backends.
   */
  public static function getStorageOptions() {
    $storage_plugins = ctools_get_plugins('entity_collection', 'plugins');
    // Get all available storage backends.
    $storage_options = array();
    foreach ($storage_plugins as $name => $info) {
      if (isset($info['title']) && isset($info['entity_collection_storage'])) {
        $storage_options[$name] = $info['title'];
      }
    }
    return $storage_options;
  }
}
