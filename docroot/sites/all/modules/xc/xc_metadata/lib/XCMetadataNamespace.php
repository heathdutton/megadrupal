<?php
/**
 * @file
 * XCMetadataNamespace class definition
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class XCMetadataNamespace extends XCMetadata {
  public $prefix;
  public $uri;
  public $title;
  public $description;
  public $module;
  public $fields = array();
  public $attributes = array();

  /**
   * Instantiate an object representating a metadata namespace type definition.
   * The object is created regardless of whether the definition has been "set"
   * to exist in the database.
   *
   * If the definiton has been "set" the constructor will first query and cache
   * the version in the database. Afterwards, unless the cache is cleared or
   * rebuilt, all subsequent calls to instantiate this object will return the
   * version cached during the page load cycle.
   *
   * Note that this version may or may not have been "set" to exist in the
   * database either.
   *
   * If the definition has not been "set", it is still cached. The only
   * difference is that there is no version in the database.
   *
   * This is how the Metadata module handles its objects. All calls to
   * instantiate a definition with similar properties will return a copy (not
   * a reference) of the most updated version of the same definition, if
   * possible.
   *
   * @param $prefix
   *    Namespace prefix
   * @param $uri
   *    Namespace URI
   * @param $module
   *    Module machine name
   */
  public function __construct($prefix, $uri = '', $module = '') {
    $_cached = xc_metadata_cache_get('namespace', $prefix);
    if (!empty($_cached)) {
      $object = $_cached;
    }
    elseif (!empty($prefix)) {
      $sql = "SELECT prefix, uri, title, description, module
              FROM {xc_metadata_namespace}
              WHERE prefix = '".$prefix."'";
      $sql .= empty($uri) ? $uri : " AND uri = '".$uri."'";
      $sql .= empty($module) ? $module : " AND module = '".$module."'";

      $object = db_query($sql)->fetchObject();
      if (!empty($object)) {
        $fields = array();
        $attributes = array();
        foreach (xc_metadata_get_fields() as $field) {
          if ($field['namespace'] == $prefix) {
            $fields[] = $field;
          }
        }
        foreach (xc_metadata_get_attributes() as $attribute) {
          if ($attribute['namespace'] == $prefix) {
            $attributes[] = $attribute;
          }
        }
        $object->fields = $fields;
        $object->attributes = $attributes;
        xc_metadata_cache_set('namespace', $prefix, $object);
      }
    }

    if (!empty($object)) {
      $this->prefix = $object->prefix;
      $this->uri = $object->uri;
      $this->title = $object->title;
      $this->description = $object->description;
      $this->module = $object->module;
      $this->fields = $object->fields;
      $this->attributes = $object->attributes;
      $this->set = TRUE;
    }
    else {
      $this->prefix = $prefix;
      $this->uri = $uri;
      $this->module = $module;
    }
  }

  /**
   * Get metadata field type definitions for all fields within namespaces of
   * this type.
   *
   * @see xc_metadata_namespace_get_attributes()
   * @return
   *    An array of XCMetadataField objects
   */
  public function get_fields() {
    return xc_metadata_namespace_get_fields($this);
  }

  /**
   * Get metadata attribute type definitions for all attributes within
   * namespaces of this type.
   *
   * @see xc_metadata_namespace_get_attributes()
   * @return
   *    An array of XCMetadataAttribute objects
   */
  public function get_attributes() {
    return xc_metadata_namespace_get_attributes($this);
  }

  /**
   * Set to the databse the namespace type definition either by inserting or
   * updating the properties of this object.
   *
   * @see xc_metadata_namespace_set()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_namespace_set($this, $reset, $force);
  }

  /**
   * Unset from the database the namespace type definition by deleting any
   * existing defition set to the database definition and removing the object
   * from the cache.
   *
   * @see xc_metadata_namespace_unset()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before removal; TRUE to remove the
   *    definition regardless of any errors that may occur during the process
   */
  public function unset_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_namespace_unset($this, $reset, $force);
  }
}
