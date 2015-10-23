<?php
/**
 * @file
 * XCMetadataEntityGroup class definition
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class XCMetadataEntityGroup extends XCMetadata {
  public $type;
  public $title;
  public $description;
  public $module;
  public $primary_entity = array();
  public $entities = array();
  public $fields = array();
  public $attributes = array();
  public $namespaces = array();

  public function __construct($type, $module = '') {
    $qualified = _object_to_qualified_name(array('type' => $type), 'record');
    $_cached = xc_metadata_cache_get('record', $type);
    if (!empty($_cached)) {
      $object = $_cached;
    }
    elseif (!empty($type)) {
      $sql = "SELECT type, title, description, module, primary_entity, entities,
              fields, attributes, namespaces
              FROM {xc_metadata_entity_group}
              WHERE type = ':type'";
      if (!empty($module)) {
        $sql .= " AND module = ':module'";
      }

      $object = db_query($sql, array(':type'=>$type,':module'=> $module))->fetchObject();
      if (!empty($object)) {
        $object->primary_entity = unserialize($object->primary_entity);
        $object->entities = unserialize($object->entities);
        $object->fields = unserialize($object->fields);
        $object->attributes = unserialize($object->attributes);
        $object->namespaces = unserialize($object->namespaces);
        xc_metadata_cache_set('record', $type, $object);
      }
    }

    if (!empty($object)) {
      $this->type = $object->type;
      $this->title = $object->title;
      $this->description = $object->description;
      $this->module = $object->module;
      $this->primary_entity = $object->primary_entity;
      $this->entities = $object->entities;
      $this->fields = $object->fields;
      $this->attributes = $object->attributes;
      $this->set = TRUE;
    }
    else {
      $this->type = $type;
      $this->module = $module;
    }
  }

  public function get_entities() {
    return xc_metadata_entity_group_get_entities($this);
  }

  public function get_fields() {
    return xc_metadata_entity_group_get_fields($this);
  }

  public function get_attributes() {
    return xc_metadata_entity_group_get_attributes($this);
  }

  public function get_namespaces() {
    return xc_metadata_entity_group_get_attributes($this);
  }

  public function set_entities() {
    xc_metadata_entity_group_set_entities($this);
  }

  public function set_fields() {
    xc_metadata_entity_group_set_fields($this);
  }

  public function set_attributes() {
    xc_metadata_entity_group_set_attributes($this);
  }

  public function set_namespaces() {
    xc_metadata_entity_group_set_attributes($this);
  }

  /**
   * Set to the databse the record type definition either by inserting or
   * updating the properties of this object.
   *
   * @see xc_metadata_entity_group_set()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_entity_group_set($this, $reset, $force);
  }

  /**
   * Unset from the database the record type definition by deleting any
   * existing defition set to the database definition and removing the object
   * from the cache.
   *
   * @see xc_metadata_entity_group_unset()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before removal; TRUE to remove the
   *    definition regardless of any errors that may occur during the process
   */
  public function unset_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_entity_group_unset($this, $reset, $force);
  }
}
