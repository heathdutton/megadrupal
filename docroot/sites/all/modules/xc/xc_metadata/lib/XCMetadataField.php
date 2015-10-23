<?php
/**
 * @file
 * XCMetadataField class definition
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class XCMetadataField extends XCMetadata {
  public $name;
  public $namespace;
  public $title;
  public $description;
  public $module;
  public $entities = array();
  public $attributes = array();
  public $type;
  public $size;
  public $max_size;
  public $required;
  public $default_value;
  public $possible_values;

  /**
   * Instantiate an object representating a metadata field type definition.
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
   * @param $name
   *    Field name
   * @param $namespace
   *    Field namespace
   * @param $module
   *    Module machine name
   */
  public function __construct($name, $namespace = '', $module = '') {
    $qualified = _object_to_qualified_name(
      array(
      'name' => $name,
      'namespace' => $namespace,
    ),
      'field'
    );
    $_cached = xc_metadata_cache_get('field', $qualified);
    if (!empty($_cached)) {
      $object = $_cached;
    }
    elseif (!empty($name)) {
      $sql = "SELECT name, namespace, title, description, module, type, size,
                 max_size, required, default_value, possible_values
              FROM {xc_metadata_field}
              WHERE name = '" . $name . "' AND namespace = '". $namespace . "'";
      if (!empty($module)) {
        $sql .= " AND module = '" . $module . "'";
      }

      $object = db_query($sql)->fetchObject();
      if (!empty($object)) {
        $entities = array();
        $attributes = array();
        $sql_entity = "SELECT DISTINCT entity FROM {xc_metadata_field_entities} WHERE field = '" . $qualified . "'";

        $result = db_query($sql_entity);
        foreach ($result as $entity) {
          $entities[] = array('type' => $entity->entity);
        }
        $sql_attr = "SELECT DISTINCT attribute FROM {xc_metadata_field_attributes} WHERE field = '".$qualified."'";

        $result = db_query($sql_attr);
        foreach ($result as $attribute) {
          $attributes[] = _qualified_name_to_array($attribute->attribute, 'attribute');
        }
        $object->entities = $entities;
        $object->attributes = $attributes;
        xc_metadata_cache_set('field', $qualified, $object);
      }
    }

    if (!empty($object)) {
      $this->name = $object->name;
      $this->namespace = $object->namespace;
      $this->title = $object->title;
      $this->description = $object->description;
      $this->module = $object->module;
      $this->entities = $object->entities;
      $this->attributes = $object->attributes;
      $this->type = $object->type;
      $this->size = $object->size;
      $this->max_size = $object->max_size;
      $this->required = $object->required;
      $this->default_value = $object->default_value;
      $this->possible_values = $object->possible_values;
      $this->set = TRUE;
    }
    else {
      $this->name = $name;
      $this->namespace = $namespace;
      $this->module = $module;
    }
  }

  /**
   * Get metadata entity type definitions for all entitys that can contain
   * fields of this type.
   *
   * @see xc_metadata_field_get_entities()
   * @return
   *    An array of XCMetadataEntity objects
   */
  public function get_entities() {
    return xc_metadata_field_get_entities($this);
  }

  /**
   * Get metadata attribute type definitions for all attributes that fields of
   * this type can contain.
   *
   * @see xc_metadata_field_get_attributes()
   * @return
   *    An array of XCMetadataAttribute objects
   */
  public function get_attributes() {
    return xc_metadata_field_get_attributes($this);
  }

  /**
   * Set to the database the relationship definition for existing entity types
   * of entities that can contain fields of this type by inserting or updating
   * the properties of this object.
   *
   * @see xc_metadata_field_set_attributes()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_entities($reset = TRUE, $force = FALSE) {
    xc_metadata_field_set_entities($this, NULL, $reset, $force);
  }

  /**
   * Set to the database the relationship definition for existing attribute
   * types of attributes that fields of this type can contain by inserting or
   * updating the properties of this object.
   *
   * @see xc_metadata_field_set_attributes()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_attributes($reset = TRUE, $force = FALSE) {
    xc_metadata_field_set_attributes($this, NULL, $reset, $force);
  }

  /**
   * Set to the databse the field type definition either by inserting or
   * updating the properties of this object.
   *
   * @see xc_metadata_field_set()
   * @see xc_metadata_field_set_entities()
   * @see xc_metadata_field_set_attributes()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before creation; TRUE to create the
   *    definition regardless of any errors that may occur during the process
   */
  public function set_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_field_set($this, $reset);
    xc_metadata_field_set_entities($this, NULL, $reset, $force);
    xc_metadata_field_set_attributes($this, NULL, $reset, $force);
  }

  /**
   * Unset from the database the field type definition by deleting any
   * existing defition set to the database definition and removing the object
   * from the cache.
   *
   * @see xc_metadata_field_unset()
   * @param $reset
   *    TRUE to clear and rebuild the metadata definitions cache
   * @param $force
   *    FALSE to validate the definition before removal; TRUE to remove the
   *    definition regardless of any errors that may occur during the process
   */
  public function unset_definition($reset = TRUE, $force = FALSE) {
    xc_metadata_field_unset($this, $reset, $force);
  }

  public function get_qualified_name() {
    return _object_to_qualified_name(
      array(
      'name' => $this->name,
      'namespace' => $this->namespace,
    ),
      'field'
    );
  }

}
