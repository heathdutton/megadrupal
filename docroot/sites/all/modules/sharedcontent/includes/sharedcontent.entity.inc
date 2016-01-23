<?php

/**
 * @file
 * Shared Content entities definitions.
 */

/**
 * Class for sharedcontent_index entity.
 */
class SharedContentIndex extends Entity {

  /**
   * Entity id.
   */
  public $id;

  /**
   * Name of the connection the index record was retrieved from.
   */
  public $connection_name;

  /**
   * The record origin.
   *
   * This defines the bundle of the entity with "local" val as default.
   */
  public $origin;

  /**
   * The records uuid.
   *
   * Globally unique id of the indexed record.
   */
  public $uuid;

  /**
   * The records parent uuid.
   *
   * UUID of a possible parent record.
   */
  public $uuid_parent;

  /**
   * Id of the indexed entity.
   */
  public $entity_id;

  /**
   * Type of the indexed entity.
   */
  public $entity_type;

  /**
   * Bundle of the indexed entity.
   */
  public $entity_bundle;

  /**
   * Title of the indexed entity.
   */
  public $title;

  /**
   * Language of the indexed entity.
   */
  public $language;

  /**
   * Id if the translation set if nay.
   */
  public $translationset_id;

  /**
   * Set of keywords describing the indexed entity.
   *
   * The keywords are stored unstructured delimited by space.
   */
  public $keywords;

  /**
   * Set of tags describing the indexed entity.
   *
   * The tags are stored unstructured delimited by space.
   */
  public $tags;

  /**
   * Url where the indexed entity can be viewed.
   */
  public $url;

  /**
   * Status of index record.
   *
   * States if the index record an be linked or displayed or if the indexed
   * content was deleted.
   */
  public $status;

  /**
   * Timestamp of the time the indexed entity was created.
   */
  public $entity_created;

  /**
   * Timestamp of the time the indexed entity was changed the last.
   */
  public $entity_changed;

  /**
   * Timestamp of the time the index record was created.
   */
  public $created;

  /**
   * Timestamp of the time the index record was changed the last.
   */
  public $changed;

  /**
   * Accessibility value. 0 if not accessible, 1 if public accessible and 2 if restricted.
   */
  public $accessibility;

  /**
   * Exposed attributes
   *
   * A list of attributes that are meant to be exposed other systems.
   *
   * @var array
   */
  private static $exposed_attributes = array(
    'uuid',
    'entity_id',
    'entity_type',
    'entity_bundle',
    'title',
    'keywords',
    'tags',
    'language',
    'translationset_id',
    'status',
    'url',
    'parent_uuid',
    'entity_created',
    'entity_changed',
  );


  /**
   * Merges data to this object
   *
   * @param array $data
   *    Associative array with the data to be merge to this object.
   * @return bool
   *    TRUE if the merge led to a data change, FALSE otherwise.
   */
  public function merge(array $data) {
    $updated = FALSE;
    foreach ($data as $key => $value) {
      if (in_array($key, SharedContentIndex::$exposed_attributes)
        && $this->$key != $value
      ) {
        $this->$key = $value;
        $updated = TRUE;
      }
    }
    return $updated;
  }

  /**
   * Exposed attributes
   *
   * Get an object containing just the attributes that are meant to be
   * exposed to other systems.
   *
   * @return stdClass
   */
  public function getExposedAttributes() {
    $exposed = new stdClass();
    foreach (SharedContentIndex::$exposed_attributes as $attribute) {
      $exposed->$attribute = $this->$attribute;
    }
    return $exposed;
  }

  /**
   * Override Entity.
   */
  function defaultUri() {
    return array('path' => $this->url);
  }
}

/**
 * Class for sharedcontent_assignment entity.
 */
class SharedContentAssignment extends Entity {

  /**
   * Entity id..
   */
  public $id;

  /**
   * Id of source content.
   */
  public $source_id;

  /**
   * uuid of source content..
   */
  public $source;

  /**
   * Id of target content.
   */
  public $target_id;

  /**
   * uuid of target content..
   */
  public $target;

  /**
   * The record origin.
   *
   * This defines the bundle of the entity with "local" val as default.
   */
  public $origin;

  /**
   * Linking status..
   */
  public $status;

  /**
   * Base URL of the system that reported this linking..
   */
  public $url;

  /**
   * Timestamp of the time the linking was created the last..
   */
  public $created;

  /**
   * Timestamp of the time the linking was changed the last..
   */
  public $changed;
}
