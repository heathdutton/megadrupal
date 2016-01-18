<?php

/**
 * @file
 * Defines and processes a CollectiveAccess object
 */

class CollectiveAccessObject extends CollectiveAccessBaseItem {

  /**
   * Create a new CollectiveAccessObject
   * @param CollectiveAccessInstance $ca -object connecting to CollectiveAccess installation
   * @param int $object_id - id of ca_object to retrieve
   * @param array $options - array with options. Possible values:
   *   * 'skeleton' => TRUE/FALSE (default FALSE) - if enabled, object can only be used as skeleton. No data will be loaded automatically.
   *   * 'object_representation_image_versions' => array() - array of image presets to retrieve
   *   * 'list_labels' => TRUE/FALSE - if enabled, fetch preferred labels for list items
   */
  public function __construct(CollectiveAccessInstance $ca, $id, $options = array()) {
    parent::__construct($ca, $id, $options);
    $this->data['object_id'] = $id;
    // set defaults for $options
    if (!isset($options['list_labels'])) {
      $options['list_labels'] = TRUE;
    }
    $this->options = $options;

    if (!isset($options['skeleton']) || $options['skeleton'] != TRUE) {
      $this->fetchData();
      // Get the configured image versions if available, or all if not available
      if (isset($options['object_representation_image_versions'])) {
        $this->fetchObjectRepresentations($options['object_representation_image_versions']);
      }
      else {
        $this->fetchObjectRepresentations();
      }
    }
  }
  

  /**
   *
   * GetLabels
   */
  public function getLabels($mode = 'preferred') {
    return parent::getLabels($mode, 'ca_objects');
  }

  /**
   * Overrides fetchData from parent
   *
   * This function calls the CollectiveAccess webservice and creates
   * the general properties on this object corresponding to thise fields
   * in CollectiveAccess
   */
  public function fetchData() {
    return parent::fetchData('ca_objects');
  }
  
  
  /**
   * Fetch available object representation image URLs from the web service
   * Retrieves all image versions, or the user-configured set of image versions (see constructor)
   */
  protected function fetchObjectRepresentations($object_representation_versions = NULL) {
    if (!$object_representation_versions) {
      return;
    }

    $images = $this->fetchObjectRepresentations_execute($object_representation_versions);
    if ($images) {
      $more_images = array();
      foreach ($images as $id => $imageinfo) {
        // get a list of image presets
        $presets = array_keys($imageinfo['urls']);
        if ($presets) {
          foreach ($presets as $preset) {
            $imgarr[$preset] = array(
              'tag' => $imageinfo['tags'][$preset],
              'url' => $imageinfo['urls'][$preset],
              'info' => $imageinfo['info'][$preset],
              'dimensions' => $imageinfo['dimensions'][$preset],
              'path' => $imageinfo['paths'][$preset],
            );
          }

          // add image info to object, grouped by preset
          if ($imageinfo['is_primary'] == '1') {
            $this->primary_image = $imgarr;
          }
          else {
            $more_images[] = $imgarr;
          }
        }
      }
      //dsm('Images for ' .  $this->data['object_id']);
      //dsm($images);
      //dsm($more_images);
      $this->more_images = $more_images;
    }
  }

  /**
   * Helper function to retrieve image information for the current object
   */
  private function fetchObjectRepresentations_execute($object_representation_versions = NULL) {
    if (isset($object_representation_versions)) {
      $params = array('object_id' => $this->object_id, 'versions' => $object_representation_versions);
    }
    else {
      $params = array('object_id' => $this->object_id,  'versions' => NULL);
    }
    $data = $this->collectiveaccess_instance->execute('ItemInfo', 'getObjectRepresentations', $params);
    if (!empty($data)) {
      return $data;
    }
    return FALSE;
  }
  
  public function getType() {
    return 'ca_objects';
  }
}

/**
 * CollectiveAccessObjectBuilder helps building lists of CollectiveAccessObject objects
 */
class CollectiveAccessObjectBuilder {
  protected $collectiveaccess_instance;

  /**
   * Constructor for CollectiveAccessObjectBuilder
   */
  public function __construct(CollectiveAccessInstance $ca) {
    $this->collectiveaccess_instance = $ca;
  }

  /**
   * Get a list of object IDs that changed since given timestamp
   * @return array of object IDs
   */
  public function getLastChangedObjectIDs($time = 0) {
    return CollectiveAccessBaseItemBuilder::getLastChangedItemIDs(
      $this->collectiveaccess_instance,
      'ca_objects',
      $time
    );
  }

  /**
   * Get object IDs that have a relationship with the supplied collection id(s).
   */
  public function getObjectIDsByCollection($collection_ids) {
    $object_ids = array();
    // convert integer to expected array format
    if (is_numeric($collection_ids)) {
      $collection_ids = array($collection_ids);
    }

    foreach ($collection_ids as $collection_id) {
      $params = array(
        'type' => 'ca_collections',
        'item_id' => $collection_id,
        'related_type' => 'ca_objects',
      );
      $result = $this->collectiveaccess_instance->execute('ItemInfo', 'getRelationships', $params);
      $new_object_ids = array();
      if ($result) {
        foreach ($result as $id => $object) {
          $new_object_ids[] = $object['object_id'];
        }
        $object_ids = array_merge($object_ids, $new_object_ids);
      }
    }
    return $object_ids;
  }
}
