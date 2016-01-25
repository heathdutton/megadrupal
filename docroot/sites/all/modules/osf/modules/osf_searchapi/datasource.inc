<?php

use \StructuredDynamics\osf\framework\Subject;

/**
 * @file
 * Contains the SearchApiStructDataSourceController class, which extends the SearchApiExternalDataSourceController class
 * and provides methods for the OSF items in search results.
 */

class SearchApiStructDataSourceController extends SearchApiExternalDataSourceController {

  /**
   * Return information on the ID field for this controller's type.
   *
   * This implementation will return a field named "id" of type "string". This
   * can also be used if the item type in question has no IDs.
   *
   * @return array
   *   An associative array containing the following keys:
   *   - key: The property key for the ID field, as used in the item wrapper.
   *   - type: The type of the ID field. Has to be one of the types from
   *     search_api_field_types(). List types ("list<*>") are not allowed.
   */
  public function getIdFieldInfo() {
    return array(
      'key' => 'id',
      'type' => 'string',
    );
  }

  /**
   * Load items of the type of this data source controller.
   */
  public function loadItems(array $ids) {
    $results = entity_load('resource_type', $ids);

    foreach ($ids as $uri) {
      $object = $results[$uri];

      // TODO: Run through a mapper to get the relevent fields exposed to Search API to add to this result object
      $result = array(
        'id' => $uri,
        'type' => 'resource_type',
        'title' => !empty($object->preflabel[LANGUAGE_NONE][0]['value']) ? $object->preflabel[LANGUAGE_NONE][0]['value'] : '',
        'user' => '',
        'excerpt' => !empty($object->description[LANGUAGE_NONE][0]['value']) ? $object->description[LANGUAGE_NONE][0]['value'] : '',
        'language' => LANGUAGE_NONE,
      );

      $items[$uri] = $result; 
    } 

    return $items;
  }

  /**
   * Helper method that can be used by subclasses to specify the property
   * information to use when creating a metadata wrapper.
   *
   * For most use cases, you will have to override this method to provide the
   * real property information for your item type.
   *
   * @return array
   *   Property information as specified by hook_entity_property_info().
   *
   * @see hook_entity_property_info()
   */
  protected function getPropertyInfo() {
    $info['property info']['id'] = array(
      'label' => t('ID'),
      'type' => 'string',
    );

    return $info;
  }

  /**
   * Get the unique ID of an item.
   *
   * Always returns 1.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either the unique ID of the item, or NULL if none is available.
   */
  public function getItemId($item) {
    return 1;
  }

  /**
   * Get a human-readable label for an item.
   */
  public function getItemLabel($item) {
    return !empty($item['title']) ? $item['title'] : NULL;
  }

  /**
   * Get a URL at which the item can be viewed on the web.
   *
   * Always returns NULL.
   *
   * @param $item
   *   An item of this controller's type.
   *
   * @return
   *   Either an array containing the 'path' and 'options' keys used to build
   *   the URL of the item, and matching the signature of url(), or NULL if the
   *   item has no URL of its own.
   */
  public function getItemUrl($item) {
    return !empty($item['link']) ? $item['link'] : NULL;;
  }


  /* DON'T NEED - RUNNING READ-ONLY */
  public function startTracking(array $indexes) {
    return;
  }
  public function stopTracking(array $indexes) {
    return;
  }
  public function trackItemInsert(array $item_ids, array $indexes) {
    return;
  }
  public function trackItemChange($item_ids, array $indexes, $dequeue = FALSE) {
    return;
  }
  public function trackItemIndexed(array $item_ids, SearchApiIndex $index) {
    return;
  }
  public function trackItemDelete(array $item_ids, array $indexes) {
    return;
  }
  public function getChangedItems(SearchApiIndex $index, $limit = -1) {
    return array();
  }
  public function getIndexStatus(SearchApiIndex $index) {
    return array(
      'indexed' => 0,
      'total' => 0,
    );
  }
}
